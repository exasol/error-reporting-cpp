# Considerations
During the exploration of possible implementation the following assumptions/requirements have been found:
1. The error message declaration should be globally visible (In order to build the error catalog). This means, the error declaration must be implemented as a global variable (Must not be a member variable)
2. Because of point 1, collection the error message information must not cause any heap allocations
3. Because of point 1, the error message declaration and instantiation will occur on 2 different places in the code: Consistency checks must be implemented to verify correct instantiation of an error message (Parameter match!) 

## Proposal 1 - Declare a global variable

### Summary

`error-reporting-cpp` declares macros which can be used to define a global variable for each error code.
This global variable automatically registers to the global error catalog during instantiation,
and provides infos about:
- error code
- error message
- mitigations
- parameter descriptions

The error catalog is implemented as a linked list (one error message declaration pointing to the next one => Requirements 2.)
Also, the number of parameters are stored as a template parameter (Requirement 3).

You can use the global variable to instantiate an error message. You need to add exactly the same number of parameters, as passed as parameter descriptions.

For messages, without parameters, there is a separate, simpler macro.

### Example 1 - no parameters

#### Declaration
```c++

DECLARE_ERR_MSG(EC_DB_1, "Out of memory", MITIGATIONS("Buy more RAM", "Do abc"));
```
This marco expands to the following code snippet:
```c++
error_message_declaration EC_DB_1("EC_DB_1", "Out of memory", {"Buy more RAM"})
```

#### Usage
```c++
const std::string res = EC_DB_1.str(); // => "[EC_DB_1] - Out of memory"
```

### Example 2 - with parameters
#### Declaration
```c++
DECLARE_ERR_MSG_PARAMS(EC_DB_2, "hd error {{Dummy1}} , {{Dummy2}}",
                       MITIGATIONS("Buy new disk"),PARAMS("Description for Dummy1", "Description for Dummy2"));
```
This marco expands to the following code snippet:
```c++
namespace error_reporting {
namespace EC_DB_2_params {
const char *gEC_PARAMS[] = {"Description for Dummy1", "Description for Dummy2"};
}
}
error_message_declaration_with_param<2> EC_DB_2("EC_DB_2",
                                                 "hd error {{Dummy1}} , {{Dummy2}}",
                                                 {"Buy new disk"},
                                                 error_reporting::EC_DB_2_params::gEC_PARAMS)
```

The template parameter 2 is based on the size of the previously defined static array ```gEC_PARAMS```
(sizeof(error_reporting::EC##_params::g##EC_PARAMS) / sizeof(const char*)
It is used for the consistency check during instantiation of the error. 
#### Usage
```c++
const std::string res = EC_DB_2.build().param("Dummy1", 1).param("Dummy2", "bla").str();
//=>[EC_DB_2] : hd error 1 , bla
```
Note that *not* invoking param() 2x will cause a compiler error here!

### Pro

#### Possibility of separating error message declaration and usage

The global variables can be defined in one C++ file and used (by using macro EXTERN_ERR_MSGS())) in another C++ file.
Because the classes for all error message declaration are the same (defined in ```error-reporting-cpp``), this is possible.

For example, if one wants to declare all error message declarations in one C++ file, this approach would provide this.

### Contra

#### Insufficient consistency check

The consistency check only checks the number of parameters at compile time (linker time when using the extern macros), but not the names of the parameters.
Changing the parameter names at the declaration, will cause an inconsistency during instantiation, which only will be detected whenever the error occurs.


## Proposal 2 - Declare a a class and a global variable

### Summary

`error-reporting-cpp` declares macros which can be used to define a new class plus a global variable of that class for each error code.
This global variable automatically registers to the global error catalog during instantiation,
and provides infos about:
- error code
- error message
- mitigations
- parameter descriptions

The error catalog is implemented as a linked list (one error message declaration pointing to the next one => Requirements 2.)

You can use the global variable to instantiate an error message. The class provides methods for setting the previously declared parameters.
Not setting all parameters will cause a compiler error.

For messages, without parameters, there is a separate, simpler macro.

### Example 1 - no parameters

_Same as proposal 1_

### Example 2 - with parameters
#### Declaration
```c++
DECLARE_ERR_MSG_PARAMS(EC_DB_2, "hd error {{Dummy1}} , {{Dummy2}}",
                       MITIGATIONS("Buy new disk"),PARAMS("Description for Dummy1", "Description for Dummy2"));
```
This marco expands to the following (*incomplete*) code snippet:
```c++
namespace error_reporting {
struct EC_DB_2_class {
    public:
    EC_DB_2_class() {
        static const const char *gEC_PARAMS[] = {"Description for Dummy1", "Description for Dummy2"};
            register_error_code();
    }

    template<typename T>        
    void setDummmy1(const T & param) {
        ...
    }

    template<typename T>
    void setDummmy2(const T & param) {
    ...
    }
    ...
};
}
//Instantiation of global variable!!!
error_reporting::EC_DB_2_class EC_DB_2;
```

#### Usage
```c++
const std::string res = EC_DB_2.build().setDummmy1(1).setDummmy2("bla").str();
//=>[EC_DB_2] : hd error 1 , bla
```
Note that ```setDummmy1()``` and ```setDummmy2()``` needs to be invoked, otherwise it will cause a compile error.

### Pro

#### Detailed consistency check

The parameter names will be verified during compilation. Changing a parameter in the declaration without adjusting instantiation will cause a compile error. 

### Contra

#### Declaration and Instantiation must be in the same C++ file

As this approach declares new classes, it's not possible to declare an error code in one C++ file, and instantiate it in other file.



## Error catalog creation

In both approaches, the global variable ```g_error_message_container``` can be used to collect all available error codes:
```c++
const std::string res = g_error_message_container.collect_error_messages_as_json();
```
=> Returns
```json
[{ "code": "EC_DB_1" ,  "message": "Out of memory", "parameters" : [], "mitigations" : ["Buy more RAM",]},{ "code": "EC_DB_2" ,  "message": "hd error {{Dummy1}} , {{Dummy2}}", "parameters" : ["Description for Dummy1","Description for Dummy2",], "mitigations" : ["Buy new disk",]},]
```

