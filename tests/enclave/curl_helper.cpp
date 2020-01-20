// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include <curl/curl.h>

#include <mbedtls/pk.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/x509_crl.h>
#include <mbedtls/ssl.h>

#include "ca_certificates.h"

#include "curl_helper.h"

namespace tests {

static bool verbose_curl;

void CurlInit(bool verbose) {
  verbose_curl = verbose;
  curl_global_sslset(CURLSSLBACKEND_MBEDTLS, nullptr, nullptr);
  curl_global_init(CURL_GLOBAL_ALL);
}

void CurlCleanup() {
  curl_global_cleanup();
}

static size_t CurlWriteCallback(void* ptr, size_t size, size_t nmemb, std::string* s) {
  s->append(reinterpret_cast<char*>(ptr), size * nmemb);
  return size * nmemb;
}

static CURLcode CurlSslCtxCallback(CURL* curl, void* sslctx, void* parm) {
  (void)parm;
  (void)curl;
  mbedtls_ssl_config* cfg = reinterpret_cast<mbedtls_ssl_config*>(sslctx);

  int r = mbedtls_x509_crt_parse(cfg->ca_chain,
                                 _etc_ssl_certs_ca_certificates_crt,
                                 _etc_ssl_certs_ca_certificates_crt_len);

  return r != 0 ? CURLE_ABORTED_BY_CALLBACK : CURLE_OK;
}

std::string Curl(const std::string& url,
                 const std::string& body,
                 const std::map<std::string, std::string>& headers,
                 HttpMethod method) {
  std::string response;

  CURL* curl = curl_easy_init();
  if (curl == nullptr) {
    throw std::runtime_error("Error initializing curl");
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  if (!body.empty()) {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
  }
  if (method == HttpMethod::GET && !body.empty()) {
    method = HttpMethod::POST;
  }
  if (method == HttpMethod::GET) {
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  } else if (method == HttpMethod::POST) {
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
  } else if (method == HttpMethod::PUT) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  } else if (method == HttpMethod::DELETE) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  }
  curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose_curl ? 1L : 0L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

  curl_easy_setopt(curl, CURLOPT_CAINFO, NULL);
  curl_easy_setopt(curl, CURLOPT_CAPATH, NULL);
  curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
  curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, CurlSslCtxCallback);

  struct curl_slist* curl_headers = nullptr;
  for (auto& item : headers) {
    std::string header = item.first + ": " + item.second;
    curl_headers = curl_slist_append(curl_headers, header.c_str());
  }
  if (curl_headers) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
  }

  auto result = curl_easy_perform(curl);
  curl_slist_free_all(curl_headers);

  int64_t response_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

  curl_easy_cleanup(curl);

  if (verbose_curl) {
    std::cout << "CURL: URL=" << url << std::endl;
    std::cout << "CURL: Request body=" << body << std::endl;
    std::cout << "CURL: Response body=" << response << std::endl;
    std::cout << "CURL: Response code=" << response_code << std::endl;
  }

  if (result != CURLE_OK) {
    const char* err_s = curl_easy_strerror(result);
    throw CurlOtherError(url, err_s, result);
  }
  if (response_code >= 400) {
    throw CurlHTTPError(url, response_code);
  }

  return response;
}

}  // namespace tests
