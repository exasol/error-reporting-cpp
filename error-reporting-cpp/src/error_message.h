
#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_H

#include <regex>
#include <sstream>
#include <utility>

namespace error_reporting {

    struct error_message_declaration_internal {
        explicit error_message_declaration_internal(
                const char* _error_code,
                const char* _error_message,
                std::initializer_list<const char*> _error_mitigations,
                size_t _nr_parameters,
                const char* const* _parameters);
        const char* error_code;
        const char* error_message;
        const size_t nr_error_mitigations;
        const char* const * error_mitigations;
        const size_t nr_parameters;
        const char * const * parameters;
        error_message_declaration_internal * next;
    };

    class error_message_declaration_container {
    public:
        error_message_declaration_container() : root(nullptr) {}

        void register_error_message(error_message_declaration_internal * msg) {
            error_message_declaration_internal * n = root;

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
                msg_stream << "], \"mitigations\" : [";
                for (size_t idxMitigation(0); idxMitigation < n->nr_error_mitigations; idxMitigation++) {
                    msg_stream << "\"" << n->error_mitigations[idxMitigation] << "\",";
                }
                msg_stream << "]},";
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
            const char* _error_code,
            const char* _error_message,
            std::initializer_list<const char*> _error_mitigations,
            size_t _nr_parameters,
            const char* const * _parameters) :
            error_code(_error_code),
            error_message(_error_message),
            nr_error_mitigations(_error_mitigations.size()),
            error_mitigations(_error_mitigations.begin()),
            nr_parameters(_nr_parameters),
            parameters(_parameters),
            next(nullptr) {
        g_error_message_container.register_error_message(this);
    }

    struct error_message_declaration : error_message_declaration_internal {
        explicit error_message_declaration(
                const char* _error_code,
                const char* _error_message,
                std::initializer_list<const char*> _error_mitigations) :
                error_message_declaration_internal( _error_code, _error_message, _error_mitigations,
                                                    0, nullptr){}
        std::string str() {
            std::ostringstream os;
            os << "[" << error_code << "] - " << error_message;
            return os.str();
        }
    };

    template<int COUNTER>
    class ErrorBuilder {
    public:
        explicit ErrorBuilder(const error_message_declaration_internal& _src)  {
            std::ostringstream os;
            os << "[" << _src.error_code << "] : " << _src.error_message;
            message = os.str();
        };

        explicit ErrorBuilder(std::string  str)
        : message(std::move(str)) {};

        template<typename T>
        ErrorBuilder<COUNTER - 1> param( const std::string key, const T & param ) && {
            static_assert(COUNTER > 0);
            std::ostringstream os;
            os << param;
            const std::string key_str = std::string("\\{\\{") + key + "\\}\\}";
            const std::string new_message =
                    std::regex_replace(message, std::regex(key_str), os.str());
            return ErrorBuilder<COUNTER - 1>(new_message);
        }

        //Implicit cast from error builder to string, but only when building is complete
        typename std::conditional<0 == COUNTER, std::string, void>::type str() {
            return message;
        }

    private:
        std::string message;
    };

    template<size_t N_PARAM>
    struct error_message_declaration_with_param : error_message_declaration_internal {
        explicit error_message_declaration_with_param(
                const char* _error_code,
                const char* _error_message,
                std::initializer_list<const char*> _error_mitigations,
                const char ** _parameters) :
                error_message_declaration_internal( _error_code, _error_message, _error_mitigations,
                                                    N_PARAM, _parameters){}

        ErrorBuilder<N_PARAM> build() {
            return ErrorBuilder<N_PARAM>(*this);
        }
    };
}

#define DECLARE_ERR_MSG(EC, EM, MITIGATIONS) error_message_declaration EC(#EC, EM, MITIGATIONS)
#define PARAMS(...) { __VA_ARGS__ }
#define MITIGATIONS(...) { __VA_ARGS__ }
#define DECLARE_ERR_MSG_PARAMS(EC, EM, MITIGATIONS, PARAMS) \
namespace error_reporting { namespace EC##_params {          \
const char* g##EC_PARAMS[] = PARAMS;                        \
} }                                                         \
error_message_declaration_with_param<sizeof(error_reporting::EC##_params::g##EC_PARAMS) / sizeof(const char*)> EC(#EC, EM, MITIGATIONS, error_reporting::EC##_params::g##EC_PARAMS)

#define EXTERN_ERR_MSGS(EC) extern error_message_declaration_with_param<NUM_PARAM> EC
#define EXTERN_ERR_MSG_PARAMS(EC, NUM_PARAM) extern error_message_declaration EC

#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_H

