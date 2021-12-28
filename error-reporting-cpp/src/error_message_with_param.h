#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_WITH_PARAM_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_WITH_PARAM_H

#include "error_message_internal.h"

namespace error_reporting {

    template<int num_parameters, int current_flags = 0>
            class ErrorBuilderParameterChecker {

            public:
                enum {
                    COMPLETE = current_flags == (1 << num_parameters) - 1,
                    };

                template<int pos>
                using NextBuilder = ErrorBuilderParameterChecker<num_parameters, current_flags | (1 << pos)>;
            };

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

#define PARAM(desc, name) desc, name


#define PARAM_DESCR_FUNCTION1(desc, name) desc
#define PARAM_DESCR_FUNCTION2(desc, name, ...) desc, PARAM_DESCR_FUNCTION1(__VA_ARGS__)
#define PARAM_DESCR_FUNCTION3(desc, name, ...) desc, PARAM_DESCR_FUNCTION2(__VA_ARGS__)
#define PARAM_DESCR_FUNCTION4(desc, name, ...) desc, PARAM_DESCR_FUNCTION3(__VA_ARGS__)
#define PARAM_DESCR_FUNCTION5(desc, name, ...) desc, PARAM_DESCR_FUNCTION4(__VA_ARGS__)


#define EXPAND(x) x
#define GET_MACRO(_1_desc, _1_id, _2_descr, _2_id, _3_descr, _3_id, _4_descr, _4_id, _5_descr, _5_id, NAME, ...) NAME
#define PARAM_DESCR_FUNCTION(...) EXPAND(GET_MACRO(__VA_ARGS__, PARAM_DESCR_FUNCTION5, PARAM_DESCR_FUNCTION5, \
PARAM_DESCR_FUNCTION4, PARAM_DESCR_FUNCTION4, \
PARAM_DESCR_FUNCTION3, PARAM_DESCR_FUNCTION3, \
PARAM_DESCR_FUNCTION2, PARAM_DESCR_FUNCTION2, \
PARAM_DESCR_FUNCTION1, PARAM_DESCR_FUNCTION1)(__VA_ARGS__))


#define PARAM_SETTER_FUNCTION(EC, KEY, INDEX) \
using PARAM_##INDEX##_NEXTBUILDER = typename parameter_check::template NextBuilder<INDEX>; \
template<typename T>                                                           \
EC##_builder<PARAM_##INDEX##_NEXTBUILDER> set##KEY(const T & param) { \
const std::string new_message = error_reporting::replace_error_parameter(message, #KEY, param ); \
return EC##_builder<PARAM_##INDEX##_NEXTBUILDER>(new_message); \
}                                         \

#define PARAM_SETTER_FUNCTION1(ec, desc, id) PARAM_SETTER_FUNCTION(ec, id, 0)
#define PARAM_SETTER_FUNCTION2(ec, desc, id, ...) PARAM_SETTER_FUNCTION(ec, id, 1) PARAM_SETTER_FUNCTION1(ec, __VA_ARGS__)
#define PARAM_SETTER_FUNCTION3(ec, desc, id, ...) PARAM_SETTER_FUNCTION(ec, id, 2) PARAM_SETTER_FUNCTION2(ec, __VA_ARGS__)
#define PARAM_SETTER_FUNCTION4(ec, desc, id, ...) PARAM_SETTER_FUNCTION(ec, id, 3) PARAM_SETTER_FUNCTION3(ec, __VA_ARGS__)
#define PARAM_SETTER_FUNCTION5(ec, desc, id, ...) PARAM_SETTER_FUNCTION(ec, id, 4) PARAM_SETTER_FUNCTION4(ec, __VA_ARGS__)

#define PARAM_SETTER_FUNCTIONS(EC, ...) EXPAND(GET_MACRO(__VA_ARGS__, PARAM_SETTER_FUNCTION5, PARAM_SETTER_FUNCTION5, \
PARAM_SETTER_FUNCTION4, PARAM_SETTER_FUNCTION4,  \
PARAM_SETTER_FUNCTION3, PARAM_SETTER_FUNCTION3,  \
PARAM_SETTER_FUNCTION2, PARAM_SETTER_FUNCTION2,  \
PARAM_SETTER_FUNCTION1, PARAM_SETTER_FUNCTION1)(EC, __VA_ARGS__))



#define PARAM_CHECKER(EM, MITIG, KEY) \
static_assert((EM).find("{{"#KEY"}}") != std::string_view::npos || \
(MITIG).find("{{"#KEY"}}") != std::string_view::npos);

#define PARAM_CHECKER_FUNCTION1(em, mt, desc, id) PARAM_CHECKER(em, mt, id)
#define PARAM_CHECKER_FUNCTION2(em, mt, desc, id, ...) PARAM_CHECKER(em, mt, id) PARAM_CHECKER_FUNCTION1(em, mt, __VA_ARGS__)
#define PARAM_CHECKER_FUNCTION3(em, mt, desc, id, ...) PARAM_CHECKER(em, mt, id) PARAM_CHECKER_FUNCTION2(em, mt, __VA_ARGS__)
#define PARAM_CHECKER_FUNCTION4(em, mt, desc, id, ...) PARAM_CHECKER(em, mt, id) PARAM_CHECKER_FUNCTION3(em, mt, __VA_ARGS__)
#define PARAM_CHECKER_FUNCTION5(em, mt, desc, id, ...) PARAM_CHECKER(em, mt, id) PARAM_CHECKER_FUNCTION4(em, mt, __VA_ARGS__)

#define PARAM_CHECKER_FUNCTIONS(EM, MITIG, ...) EXPAND(GET_MACRO(__VA_ARGS__, \
PARAM_CHECKER_FUNCTION5, PARAM_CHECKER_FUNCTION5, \
PARAM_CHECKER_FUNCTION4, PARAM_CHECKER_FUNCTION4,  \
PARAM_CHECKER_FUNCTION3, PARAM_CHECKER_FUNCTION3,  \
PARAM_CHECKER_FUNCTION2, PARAM_CHECKER_FUNCTION2,  \
PARAM_CHECKER_FUNCTION1, PARAM_CHECKER_FUNCTION1)(EM, MITIG, __VA_ARGS__))


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


#define DECLARE_ERROR_BUILDER_CLASS(EC, EM, MITIG, ...)                                             \
class EC##_class : error_message_declaration_internal {                                             \
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
    std::string msg = std::string(error_code_str) + ": " + std::string(message_str);                \
    if (!mitigations_str.empty()) {                                                                 \
        msg.append(" ");                                                                            \
        msg.append(mitigations_str);                                                                \
    }                                                                                               \
    return ErrorBuilder_t(msg);                                                                     \
}                                                                                                   \
};                                                                                                  \

#define DECLARE_ERR_MSG_PARAMS(EC, EM, MITIG, ...)                          \
namespace error_reporting {                                                 \
DECLARE_ERROR_BUILDER(EC, __VA_ARGS__)                                      \
DECLARE_ERROR_BUILDER_CLASS(EC, EM, MITIG, __VA_ARGS__)                     \
}                                                                           \
ERROR_MESSAGE_CLASS_INSTANCE_MODIFIER error_reporting::EC##_class EC;       \



#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_WITH_PARAM_H
