#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_WITH_PARAM_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_WITH_PARAM_H

#include "error_message_internal.h"

/** @file
 * @brief Internal header file for Exasol error-reporting-cpp which implements generation of error-code classes with parameters.
 *
 */

/**
 * @brief Global error reporting namespace.
 *
 * Used to encapsulate error reporting classes.
 */
namespace error_reporting {

    /**
     * @brief Helper class to check invocation of all parameters of a error-code builder class.
     * @tparam num_parameters
     * @tparam current_flags
     */
    template<int num_parameters, int current_flags = 0>
            class ErrorBuilderParameterChecker {

            public:
                enum {
                    COMPLETE = current_flags == (1 << num_parameters) - 1,
                    };

                template<int pos>
                using NextBuilder = ErrorBuilderParameterChecker<num_parameters, current_flags | (1 << pos)>;
            };

    /**
     * Helper function to replace one argument placeholder with it's actual value.
     * @tparam T type of the value. Needs to be compatible with \see std::stringstream.
     * @param message Message containing the placeholder.
     * @param parameter_name Name of the parameter
     * @param value Value to be replaced.
     * @return A string containing the replace.
     *
     * For example, this function replaces the input message "Not enough space on device {{Device}}." with
     * "Not enough space on device '/dev/sda1', if the parameter_name is "Device" and the value is "/dev/sda1".
     */
    template<typename T>
    inline std::string replace_error_parameter(const std::string message, const std::string parameter_name, T value) {
        std::ostringstream os;
        os << "'" << value << "'";
        const std::string key_str = std::string("\\{\\{") + parameter_name + "\\}\\}";
        const std::string new_message =
                std::regex_replace(message, std::regex(key_str), os.str());
        return new_message;
    }
}

/**
 * @internal
 * Helper macro to flatten parameter description and name
 * @endinternal
 */
#define PARAM(desc, name) desc, name


/**
 * @internal
 * Helper macro to retrieve description from arguments.
 * @endinternal
 */
#define PARAM_DESCR_FUNCTION1(desc, name) desc
/**
 * @internal
 * Helper macro to retrieve description from arguments.
 * @endinternal
 */
#define PARAM_DESCR_FUNCTION2(desc, name, ...) desc, PARAM_DESCR_FUNCTION1(__VA_ARGS__)
/**
 * @internal
 * Helper macro to retrieve description from arguments.
 * @endinternal
 */
#define PARAM_DESCR_FUNCTION3(desc, name, ...) desc, PARAM_DESCR_FUNCTION2(__VA_ARGS__)
/**
 * @internal
 * Helper macro to retrieve description from arguments.
 * @endinternal
 */
#define PARAM_DESCR_FUNCTION4(desc, name, ...) desc, PARAM_DESCR_FUNCTION3(__VA_ARGS__)
/**
 * @internal
 * Helper macro to retrieve description from arguments.
 * @endinternal
 */
#define PARAM_DESCR_FUNCTION5(desc, name, ...) desc, PARAM_DESCR_FUNCTION4(__VA_ARGS__)


/**
 * @internal
 * Helper macro to retrieve description from arguments.
 * @endinternal
 */
#define EXPAND(x) x
/**
 * @internal
 * Helper macro to retrieve correct macro depending the number of parameters.
 * @endinternal
 */
#define GET_MACRO(_1_desc, _1_id, _2_descr, _2_id, _3_descr, _3_id, _4_descr, _4_id, _5_descr, _5_id, NAME, ...) NAME
/**
 * @internal
 * Helper macro to retrieve all descriptions as a comma separated list from arguments.
 * @endinternal
 */
#define PARAM_DESCR_FUNCTION(...) EXPAND(GET_MACRO(__VA_ARGS__, PARAM_DESCR_FUNCTION5, PARAM_DESCR_FUNCTION5, \
PARAM_DESCR_FUNCTION4, PARAM_DESCR_FUNCTION4, \
PARAM_DESCR_FUNCTION3, PARAM_DESCR_FUNCTION3, \
PARAM_DESCR_FUNCTION2, PARAM_DESCR_FUNCTION2, \
PARAM_DESCR_FUNCTION1, PARAM_DESCR_FUNCTION1)(__VA_ARGS__))


/**
 * @internal
 * Helper macro to declare the set function for one parameter.
 * It use the @see replace_error_parameter() function to replace the current parameter and returns
 * an object of type parameter_check::NextBuilder. Thus we ensure that all parameter must be set before
 * the invocation of str() is allowed.
 * @endinternal
 */
#define PARAM_SETTER_FUNCTION(EC, KEY, INDEX) \
using PARAM_##INDEX##_NEXTBUILDER = typename parameter_check::template NextBuilder<INDEX>; \
template<typename T>                                                           \
EC##_builder<PARAM_##INDEX##_NEXTBUILDER> set##KEY(const T & param) { \
const std::string new_message = error_reporting::replace_error_parameter(message, #KEY, param ); \
return EC##_builder<PARAM_##INDEX##_NEXTBUILDER>(new_message); \
}                                         \

/**
 * @internal
 * Helper macro to declare the set function for a parameter.
 * @endinternal
 */
#define PARAM_SETTER_FUNCTION1(ec, desc, id) PARAM_SETTER_FUNCTION(ec, id, 0)
/**
 * @internal
 * Helper macro to declare the set function for a parameter.
 * @endinternal
 */
#define PARAM_SETTER_FUNCTION2(ec, desc, id, ...) PARAM_SETTER_FUNCTION(ec, id, 1) PARAM_SETTER_FUNCTION1(ec, __VA_ARGS__)
/**
 * @internal
 * Helper macro to declare the set function for a parameter.
 * @endinternal
 */
#define PARAM_SETTER_FUNCTION3(ec, desc, id, ...) PARAM_SETTER_FUNCTION(ec, id, 2) PARAM_SETTER_FUNCTION2(ec, __VA_ARGS__)
/**
 * @internal
 * Helper macro to declare the set function for a parameter.
 * @endinternal
 */
#define PARAM_SETTER_FUNCTION4(ec, desc, id, ...) PARAM_SETTER_FUNCTION(ec, id, 3) PARAM_SETTER_FUNCTION3(ec, __VA_ARGS__)
/**
 * @internal
 * Helper macro to declare the set function for a parameter.
 * @endinternal
 */
#define PARAM_SETTER_FUNCTION5(ec, desc, id, ...) PARAM_SETTER_FUNCTION(ec, id, 4) PARAM_SETTER_FUNCTION4(ec, __VA_ARGS__)

/**
 * @internal
 * Helper macro to declare the set functions for all parameters.
 * @endinternal
 */
#define PARAM_SETTER_FUNCTIONS(EC, ...) EXPAND(GET_MACRO(__VA_ARGS__, PARAM_SETTER_FUNCTION5, PARAM_SETTER_FUNCTION5, \
PARAM_SETTER_FUNCTION4, PARAM_SETTER_FUNCTION4,  \
PARAM_SETTER_FUNCTION3, PARAM_SETTER_FUNCTION3,  \
PARAM_SETTER_FUNCTION2, PARAM_SETTER_FUNCTION2,  \
PARAM_SETTER_FUNCTION1, PARAM_SETTER_FUNCTION1)(EC, __VA_ARGS__))


/**
 * @internal
 * Helper macro to declare static_assert which verifies consistency between the placeholders in the error-message/mitigations
 * and parameters ids.
 * @endinternal
 */
#define PARAM_CHECKER(EM, MITIG, KEY) \
static_assert((EM).find("{{"#KEY"}}") != std::string_view::npos || \
(MITIG).find("{{"#KEY"}}") != std::string_view::npos);

/**
 * @internal
 * Helper macro to check consistency of a parameter.
 * @endinternal
 */
#define PARAM_CHECKER_FUNCTION1(em, mt, desc, id) PARAM_CHECKER(em, mt, id)
/**
 * @internal
 * Helper macro to check consistency of a parameter.
 * @endinternal
 */
#define PARAM_CHECKER_FUNCTION2(em, mt, desc, id, ...) PARAM_CHECKER(em, mt, id) PARAM_CHECKER_FUNCTION1(em, mt, __VA_ARGS__)
/**
 * @internal
 * Helper macro to check consistency of a parameter.
 * @endinternal
 */
#define PARAM_CHECKER_FUNCTION3(em, mt, desc, id, ...) PARAM_CHECKER(em, mt, id) PARAM_CHECKER_FUNCTION2(em, mt, __VA_ARGS__)
/**
 * @internal
 * Helper macro to check consistency of a parameter.
 * @endinternal
 */
#define PARAM_CHECKER_FUNCTION4(em, mt, desc, id, ...) PARAM_CHECKER(em, mt, id) PARAM_CHECKER_FUNCTION3(em, mt, __VA_ARGS__)
/**
 * @internal
 * Helper macro to check consistency of a parameter.
 * @endinternal
 */
#define PARAM_CHECKER_FUNCTION5(em, mt, desc, id, ...) PARAM_CHECKER(em, mt, id) PARAM_CHECKER_FUNCTION4(em, mt, __VA_ARGS__)

/**
 * @internal
 * Helper macro to check consistency of all parameters.
 * @endinternal
 */
#define PARAM_CHECKER_FUNCTIONS(EM, MITIG, ...) EXPAND(GET_MACRO(__VA_ARGS__, \
PARAM_CHECKER_FUNCTION5, PARAM_CHECKER_FUNCTION5, \
PARAM_CHECKER_FUNCTION4, PARAM_CHECKER_FUNCTION4,  \
PARAM_CHECKER_FUNCTION3, PARAM_CHECKER_FUNCTION3,  \
PARAM_CHECKER_FUNCTION2, PARAM_CHECKER_FUNCTION2,  \
PARAM_CHECKER_FUNCTION1, PARAM_CHECKER_FUNCTION1)(EM, MITIG, __VA_ARGS__))


/**
 * @internal
 * Helper macro to declare class for error-code builder, which repalces incrementally the parameters with it's actual values.
 * If all parameters are set, the function str() can be called to get final error string.
 * This is achieved by using template class @see parameter_check which contains a bitset for all parameters.
 * Only if all bits are set the check std::conditional<> will return a std::string, and not a void.
 * @endinternal
 */
#define DECLARE_ERROR_BUILDER(EC, ...)                                                      \
template<typename parameter_check>                                                          \
class EC##_builder {                                                                        \
private:                                                                                    \
std::string message;                                                                        \
\
public:                                                                                     \
\
explicit EC##_builder(const std::string msg): message(msg) {}                               \
\
PARAM_SETTER_FUNCTIONS(EC, __VA_ARGS__)                                                     \
\
\
typename std::conditional<parameter_check::COMPLETE, std::string, void>::type str() {       \
return message;                                                                             \
}                                                                                           \
};                                                                                          \

/**
 * @internal
 * Helper macro to declare error-code class with parameters.
 * It contains several constexpr declaration to ensure complile time consistency between declaration and usage of
 * an error-code. Also it registers itself to the global linked list of error-codes.
 * @endinternal
 */
#define DECLARE_ERROR_BUILDER_CLASS(EC, EM, MITIG, ...)                                             \
class EC##_class : protected error_message_declaration_internal {                                   \
public:                                                                                             \
static constexpr std::string_view  error_code_str = #EC;                                            \
static constexpr std::string_view  message_str = EM;                                                \
MITIG;                                                                                              \
constexpr static const char * const gEC_PARAMS[] = {PARAM_DESCR_FUNCTION(__VA_ARGS__)};             \
constexpr static size_t num_parameter = sizeof(gEC_PARAMS) / sizeof(const char*);                   \
EC##_class() :                                                                                      \
error_message_declaration_internal(error_code_str, message_str, mitigations_str, num_parameter, gEC_PARAMS){   \
PARAM_CHECKER_FUNCTIONS(message_str, mitigations_str, __VA_ARGS__);                                 \
}                                                                                                   \
                                                                                                    \
using ErrorBuilder_t =                                                                              \
EC##_builder<error_reporting::ErrorBuilderParameterChecker<num_parameter, 0>>;                      \
ErrorBuilder_t build() {                                                                            \
    return ErrorBuilder_t(str());                                                                   \
}                                                                                                   \
};                                                                                                  \

/**
 * @brief Declares a global error-code with parameters.
 * @param EC The error code identifier. This is used as the global variable name for the error code.
 * @param EM The error message will be used to construct the error string and to create the error catalog.
 * @param MITIG Optional mitigations. Need to be constructed with the \link MITIGATIONS() \endlink macro.
 * @param ... Parameter list in the form: PARAM("Description of P1", P1), PARAM("Description of P2", P2)
 *
 * Use this macro to declare a new error-code.
 * You can later create the error code when needed with
 * @code {.cpp}
 * EC.build().setP1(1).setP2(2).str()
 * @endcode
 */
#define DECLARE_ERR_MSG_PARAMS(EC, EM, MITIG, ...)                          \
namespace error_reporting {                                                 \
DECLARE_ERROR_BUILDER(EC, __VA_ARGS__)                                      \
DECLARE_ERROR_BUILDER_CLASS(EC, EM, MITIG, __VA_ARGS__)                     \
}                                                                           \
ERROR_MESSAGE_CLASS_INSTANCE_MODIFIER error_reporting::EC##_class EC;       \



#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_WITH_PARAM_H
