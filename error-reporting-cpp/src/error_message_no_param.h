#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_NO_PARAM_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_NO_PARAM_H

#include "error_message_internal.h"


/** @file
 * @brief Internal header file for Exasol error-reporting-cpp which implements generation of error-code classes without parameters.
 *
 */

/**
 * @internal
 * @brief Generates the class definition of a specific error codes.
 *
 * Creates a class body with the error-code and error-message as constexpr static members of the class.
 * The class will automatically register itself to the global linked list in the constructor.
 * @endinternal
 */
#define DECLARE_ERROR_BUILDER_CLASS_NO_PARAM(EC, EM, MITIG)                                         \
class EC##_class : public error_message_declaration_internal {                                      \
public:                                                                                             \
static constexpr std::string_view  error_code_str = #EC;                                            \
static constexpr std::string_view  message_str = EM;                                                \
MITIG;                                                                                              \
EC##_class() :                                                                                      \
error_message_declaration_internal(error_code_str, message_str, mitigations_str, 0, nullptr){}      \
                                                                                                    \
};                                                                                                  \

/**
 * @brief Declares a global error-code without parameters.
 * @param EC The error code identifier. This is used as the global variable name for the error code.
 * @param EM The error message will be used to construct the error string and to create the error catalog.
 * @param MITIG Optional mitigations. Need to be constructed with the \link MITIGATIONS() \endlink macro.
 *
 * Use this macro to declare a new error-code.
 * You can later create the error code when needed with
 * @code {.cpp}
 * EC.str()
 * @endcode
 */
#define DECLARE_ERR_MSG(EC, EM, MITIG)                                      \
namespace error_reporting {                                                 \
DECLARE_ERROR_BUILDER_CLASS_NO_PARAM(EC, EM, MITIG)                         \
}                                                                           \
ERROR_MESSAGE_CLASS_INSTANCE_MODIFIER error_reporting::EC##_class EC;       \


#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_NO_PARAM_H
