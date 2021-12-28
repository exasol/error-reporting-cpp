#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_NO_PARAM_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_NO_PARAM_H

#include "error_message_internal.h"

#define DECLARE_ERROR_BUILDER_CLASS_NO_PARAM(EC, EM, MITIG)                                         \
class EC##_class : error_message_declaration_internal {                                             \
public:                                                                                             \
static constexpr std::string_view  error_code_str = #EC;                                            \
static constexpr std::string_view  message_str = EM;                                                \
MITIG;                                                                                              \
EC##_class() :                                                                                      \
error_message_declaration_internal(error_code_str, message_str, mitigations_str, 0, nullptr){}      \
                                                                                                    \
std::string str() {                                                                                 \
    std::string msg = std::string(#EC) + ": " + std::string(message_str);                           \
    if (!mitigations_str.empty()) {                                                                 \
        msg.append(" ");                                                                            \
        msg.append(mitigations_str);                                                                \
    }                                                                                               \
    return msg;                                                                                     \
}                                                                                                   \
};                                                                                                  \


#define DECLARE_ERR_MSG(EC, EM, MITIG)                                      \
namespace error_reporting {                                                 \
DECLARE_ERROR_BUILDER_CLASS_NO_PARAM(EC, EM, MITIG)                         \
}                                                                           \
ERROR_MESSAGE_CLASS_INSTANCE_MODIFIER error_reporting::EC##_class EC;       \


#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_NO_PARAM_H
