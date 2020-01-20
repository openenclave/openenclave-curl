// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/enclave.h>

#include "tests_t.h"
#include "curl_helper.h"

namespace tests {

// Default parameters, can be overridden during signing.
OE_SET_ENCLAVE_SGX(
    1,           /* ProductID */
    1,           /* SecurityVersion */
    true,        /* AllowDebug */
    60960,       /* HeapPageCount */
    13107,       /* StackPageCount */
    2);          /* TCSCount */

extern "C" void enclave_run_tests() {
    oe_load_module_host_socket_interface();
    oe_load_module_host_resolver();
    CurlInit();

    std::string response = Curl("https://www.microsoft.com/en-us/");
    if (response.empty()) {
        throw std::runtime_error("response empty");
    }

    CurlCleanup();
}

} // namespace tests
