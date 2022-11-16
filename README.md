# C++ Error Reporting

[![Build Status](https://github.com/exasol/error-reporting-cpp/actions/workflows/build.yaml/badge.svg)](https://github.com/exasol/error-reporting-cpp)
[![codecov.io](https://codecov.io/github/exasol/error-reporting-cpp/coverage.svg?branch=main)](https://codecov.io/github/exasol/error-reporting-cpp?branch=main)


This project contains a C++-Builder for Exasol error messages.
The invocations of the Builder can be implemented by the Executables which use this library. 

## Usage

Unlike the error-reporting in other languages (for example [error-reporting-java](https://github.com/exasol/error-reporting-java)), the C++ variant requires two steps:
* The error code definition
* The error code instantiation

### Simple Messages

#### Error code definition 
```c++
#include "error_message.h"
DECLARE_ERR_MSG(EC_TEST_1, "Something went wrong.", MITIGATIONS());
```

#### Error code instantiation
```c++
const std::string error = EC_TEST_1.str();
```

Result: `E-TEST-1: Something went wrong.`

### Parameters

You can specify placeholders in the message and replace them with parameters values, as follows:

#### Error code definition
```c++
#include "error_message.h"
DECLARE_ERR_MSG_PARAMS(EC_TEST_2, "Unknown input: {{input}}.", MITIGATIONS(), PARAM("The illegal user input", Input));
```

The first of parameter of `PARAM` will be used to fill out the error catalog. The second parameter (`Input`) will be used to create an interface for setting the particular value.
The builder automatically quotes parameters.

#### Error code instantiation

```c++
const std::string error = EC_TEST_2.build().setInput("unknown").str();
```

Result: `E-TEST-2: Unknown input: 'unknown'.`

### Mitigations
The mitigations describe actions the user can take to resolve the error. Here is an example of a mitigation definition:

#### Error code definition
```c++
#include "error_message.h"
DECLARE_ERR_MSG(EC_TEST_2, "Not enough space on device.", MITIGATIONS("Delete something."));
```

#### Error code instantiation

```c++
const std::string error = EC_TEST_2.str();
```

Result: `E-TEST-2: Not enough space on device. Delete something.`

You can use parameters in mitigations too.

#### Error code definition
```c++
#include "error_message.h"
DECLARE_ERR_MSG_PARAMS(EC_TEST_2, "Not enough space on device {{Device}}.", MITIGATIONS("Delete something from {{Device}}."), PARAM("name of the device", Device));
```

#### Error code instantiation

```c++
const std::string error = EC_TEST_2.build().setDevice("/dev/sda1").str();
```

Result: `E-TEST-2: Not enough space on device '/dev/sda1'. Delete something from '/dev/sda1'.``

## Important

We recommend declaring the error-codes in one (or more) header file(s), which are then included by the cpp files which need to throw errors.
In your main cpp file you need to declare the preprocessor constant `MAIN_ERROR_REPORTING_CPP` **before** including the error-code header file(s). 

Background: The global variables will be declared in this cpp file.


## Error code collection

If you want to enable the error code collection (for example in a separate executable), you also need to declare the preprocessor constant `ERROR_MESSAGE_COLLECTION` **before** including the main header file `error_message.h`.
In case you use a separate executable for error code collection, you should add `ERROR_MESSAGE_COLLECTION` as a compiler flag: `-DERROR_MESSAGE_COLLECTION`.

The you can retrieve a JSON string containing the definition of all error codes, used in the particular executable:
```c++
#include "error_message.h"
DECLARE_ERR_MSG(EC_TEST_1, "Something went wrong.", MITIGATIONS());
const std::string res = g_error_message_container.collect_error_messages_as_json();
```

Result: `[{ "code": "EC_TEST_1", "message": "Something went wrong.", "parameters" : [], "mitigations" : ""}]`

## Additional Resources

* [Changelog](doc/changes/changelog.md)
* [Doxygen docu](https://exasol.github.io/error-reporting-cpp)
