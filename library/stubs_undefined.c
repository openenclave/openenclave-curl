// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <mbedtls/pk.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/x509_crl.h>

// The following functions are defined in Open Enclave's headers but lack a corresponding implementation.
// curl uses them in code paths that are not required for the enclave build.
// By defining them here we avoid linker errors, however this may lead to multiple definition issues later on
// if another library also defines the same definitions.
// TODO raise issue in Open Enclave to include stub definitions

int mbedtls_x509_crt_parse_file( mbedtls_x509_crt *chain, const char *path )
{
  (void)(chain);
  (void)(path);
  puts("FATAL: mbedtls_x509_crt_parse_file() stub called, path:");
  puts(path);
  abort();
}

int mbedtls_x509_crt_parse_path( mbedtls_x509_crt *chain, const char *path )
{
  (void)(chain);
  (void)(path);
  puts("FATAL: mbedtls_x509_crt_parse_path() stub called, path:");
  puts(path);
  abort();
}

int mbedtls_pk_parse_keyfile( mbedtls_pk_context *ctx, const char *path, const char *password )
{
  (void)ctx;
  (void)path;
  (void)password;
  puts("FATAL: mbedtls_pk_parse_keyfile() stub called, path:");
  puts(path);
  abort();
}

int mbedtls_x509_crl_parse_file( mbedtls_x509_crl *chain, const char *path )
{
  (void)chain;
  (void)path;
  puts("FATAL: mbedtls_x509_crl_parse_file() stub called, path:");
  puts(path);
  abort();
}
