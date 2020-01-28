// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <vector>
#include <map>
#include <exception>

namespace tests {

class CurlError : public std::runtime_error {
 public:
  explicit CurlError(const std::string& msg, const std::string& url)
      : std::runtime_error("Curl error: " + msg + " (" + url + ")"), url(url) {}

  const std::string url;
};

class CurlHTTPError : public CurlError {
 public:
  explicit CurlHTTPError(const std::string& url, int status_code)
      : CurlError("HTTP code " + std::to_string(status_code), url), status_code(status_code) {}

  const int status_code;
};

class CurlOtherError : public CurlError {
 public:
  explicit CurlOtherError(const std::string& msg, const std::string& url, int error_code)
      : CurlError(msg, url), error_code(error_code) {}

  const int error_code;
};

enum class HttpMethod { GET,
                        POST,
                        PUT,
                        DELETE };

void CurlInit(bool verbose = false);

void CurlCleanup();

std::string Curl(const std::string& url,
                 const std::string& body = "",
                 const std::map<std::string, std::string>& headers = {},
                 HttpMethod method = HttpMethod::GET);

}  // namespace tests