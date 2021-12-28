
#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H

#include <regex>
#include <sstream>
#include <utility>
#include <string_view>

namespace error_reporting {

    struct error_message_declaration_internal {
        explicit error_message_declaration_internal(
                const std::string_view & _error_code,
                const std::string_view & _error_message,
                const std::string_view & _error_mitigations,
                size_t _nr_parameters,
                const char* const* _parameters);
        const std::string_view & error_code;
        const std::string_view & error_message;
        const std::string_view & error_mitigations;
        const size_t nr_parameters;
        const char * const * parameters;
        error_message_declaration_internal * next;
    };

    class error_message_declaration_container {
    public:
        error_message_declaration_container() : root(nullptr) {}

        void register_error_message(error_message_declaration_internal * msg) {
            if (nullptr == root) {
                root = msg;
            } else {
                msg->next = root;
                root = msg;
            }
        }

        std::string collect_error_messages_as_json() {
            std::ostringstream msg_stream;
            error_message_declaration_internal * n = root;
            msg_stream << "[";
            while(n != nullptr) {
                msg_stream << R"({ "code": ")" << n->error_code << R"(" ,  "message": ")" << n->error_message
                << R"(", "parameters" : [)";
                for (size_t idxParam(0); idxParam < n->nr_parameters; idxParam++) {
                    msg_stream << "\"" <<n->parameters[idxParam] << "\",";
                }
                msg_stream << R"(], "mitigations" : ")" << n->error_mitigations << "\"},";
                n = n->next;
            }
            msg_stream << "]";
            return msg_stream.str();
        }

    private:
        error_message_declaration_internal * root;
    };

    extern error_message_declaration_container g_error_message_container;
#ifdef MAIN_ERROR_REPORTING_CPP
    error_message_declaration_container g_error_message_container;
#endif


    inline error_message_declaration_internal::error_message_declaration_internal(
            const std::string_view & _error_code,
            const std::string_view & _error_message,
            const std::string_view & _error_mitigations,
            size_t _nr_parameters,
            const char* const * _parameters) :
            error_code(_error_code),
            error_message(_error_message),
            error_mitigations(_error_mitigations),
            nr_parameters(_nr_parameters),
            parameters(_parameters),
            next(nullptr) {
#ifdef ERROR_MESSAGE_COLLECTION
        g_error_message_container.register_error_message(this);
#endif
    }
}


//Need to allow empty arguments for MITIGATIONS macro:
//Clients can use MITIGATIONS("ABC") or MITIGATIONS()

#define FUNC_CHOOSER(_f0, _f1, ...) _f1
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define CHOOSE_FROM_ARG_COUNT(F, ...) FUNC_RECOMPOSER((__VA_ARGS__, F##_1, ))
#define NO_ARG_EXPANDER(FUNC) ,FUNC ## _0
#define MACRO_CHOOSER(FUNC, ...) CHOOSE_FROM_ARG_COUNT(FUNC, NO_ARG_EXPANDER __VA_ARGS__ (FUNC))

#define GET_MITIGATIONS_MACRO(_1, NAME, ...) NAME
#define MITIGATIONS(...)  MACRO_CHOOSER(MITIGATIONS, __VA_ARGS__)(__VA_ARGS__)

#define MITIGATIONS_1(x) \
static constexpr std::string_view  mitigations_str = (x);

#define MITIGATIONS_0() \
static constexpr std::string_view  mitigations_str = "";


#ifdef MAIN_ERROR_REPORTING_CPP
#define ERROR_MESSAGE_CLASS_INSTANCE_MODIFIER
#else
#define ERROR_MESSAGE_CLASS_INSTANCE_MODIFIER extern
#endif



#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H

