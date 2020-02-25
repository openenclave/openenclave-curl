# openenclave-curl

[![Bors enabled](https://bors.tech/images/badge_small.svg)](https://app.bors.tech/repositories/22874)

## How to use

The Open Enclave variant of libcurl has to be built as separate project, that is,
it cannot be included in CMake projects directly via `add_subdirectory`.
See the <CMakeLists.txt> file on how to do this by using the superbuild pattern.
Note that only the `library/` folder should be built when using this library in
other projects.

After the library is build it can be referenced via the `openenclave-curl::libcurl` target.
See the `tests/` project for details on how to do that.

Note that this build of libcurl uses mbedtls as provided by Open Enclave,
it does not rely on OpenSSL. It does not make any filesystem accesses either,
and so trusted CAs must be manually included in the build, as they are not fetched
from the default store. Refer to the `tests/` project for an example of how to do that.

## Development

To build the library and build/run tests simply build the root CMake project, e.g.

```sh
mkdir build
cd build
cmake ..
make
```

Contributing
------------

This project welcomes contributions and suggestions. All contributions to this repository
must adhere to the terms of the [Developer Certificate of Origin (DCO)](https://developercertificate.org/).

This project follows a [Code of Conduct](docs/CodeOfConduct.md) adapted from the
[Contributor Covenant v1.4](https://www.contributor-covenant.org).

Licensing
=========

This project is released under the
[MIT License](https://github.com/openenclave/openenclave/blob/master/LICENSE).
