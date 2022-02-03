
#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_PARAM_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_PARAM_H

#include "error-reporting-cpp/src/template/error_message_internal.h"

namespace error_reporting {
    template<typename Param> decltype(auto)  getDesc(){
        typename Param::Desc d;
        return d;
    };

    template<class... Params>
            struct Register;


    template<class T, class... Params>
            struct Register<T, Params...>{
                static void desc(std::vector<std::string> & descriptions){
                    Register<Params...>::desc(descriptions);
                    descriptions.push_back(getDesc<T>().description);
                };
            };

    template<>
    struct Register<>{
        static void desc(std::vector<std::string> & descriptions){}
    };

    template<class... Params> class BaseError {
        error_message_declaration_internal errorMessageDeclarationInternal;
    public:
        const std::string code;
        const std::string message;
        const std::vector<std::string> mitigations;

        BaseError(const std::string & code, const std::string & message,
                  const std::vector<std::string>& mitigations)
            :code(code), message(message), mitigations(mitigations) {
#ifdef ERROR_MESSAGE_COLLECTION
            std::vector<std::string> param_descriptions;
            Register<Params...>::desc(param_descriptions);
            errorMessageDeclarationInternal.setAndRegister(code, message, mitigations, param_descriptions);
#endif //ERROR_MESSAGE_COLLECTION
        };

        std::string str() {
            std::string msg = code + ": " + message;
            for(const auto& mitigation : mitigations) {
                msg.append(" ");
                msg.append(mitigation);
            }
            return msg;
        }


        std::string build(Params... params){
            std::string msg = str();
            (replaceParam(msg, params), ...);
            return msg;
        }
    private:
        template<typename Param>
        void replaceParam(std::string & msg, const Param & param) {
            std::ostringstream os;

            os << "'" << param.value << "'";
            const std::string key_str = std::string("\\{\\{") + getDesc<Param>().name + "\\}\\}";
            std::cout << "replaceParam for:'" << key_str << "' and msg='" << msg << "'" << std::endl;
            msg = std::regex_replace(msg, std::regex(key_str), os.str());
        }
    };


    class ParamDesc{
    public:
        const std::string name;
        const std::string description;

        ParamDesc(std::string name, std::string description)
        :name(name), description(description){};

    };


    class SimpleError : public error_reporting::BaseError<>{
    public:
        SimpleError(std::string code, std::string message, const std::vector<std::string> & mitigations)
            :BaseError(code, message, mitigations) {};
    };
}

#ifdef CREATE_ERROR_MESSAGE_CLASS_INSTANCES
#define CREATE_ERROR_MSG(cls, code, initializer) cls code initializer;
#else
#define CREATE_ERROR_MSG(cls, code, initializer) extern cls code;
#endif


#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_PARAM_H
