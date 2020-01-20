// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <iostream>

#include <openenclave/host.h>

#include "tests_u.h"

int main(int argc, const char* argv[])
{
    oe_result_t result;
    int ret = 1;
    oe_enclave_t* enclave = NULL;

    uint32_t flags = OE_ENCLAVE_FLAG_DEBUG;

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " enclave_image_path" << std::endl;
        goto exit;
    }

    // Create the enclave
    result = oe_create_tests_enclave(
        argv[1], OE_ENCLAVE_TYPE_AUTO, flags, NULL, 0, &enclave);
    if (result != OE_OK)
    {
        std::cerr << "oe_create_tests_enclave(): result=" << result 
                  << " (" << oe_result_str(result) << ")" << std::endl;
        goto exit;
    }

    // Call into the enclave
    result = enclave_run_tests(enclave);
    if (result != OE_OK)
    {
        std::cerr << "calling into enclave_run_tests failed: result=" << result 
                  << " (" << oe_result_str(result) << ")" << std::endl;
        goto exit;
    }

    ret = 0;

exit:
    // Clean up the enclave if we created one
    if (enclave)
        oe_terminate_enclave(enclave);

    return ret;
}