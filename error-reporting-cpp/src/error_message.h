
#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_H

namespace error_reporting {

    struct error_message_declaration {
        explicit error_message_declaration(
                const char* _error_code,
                const char* _error_message,
                std::initializer_list<const char*> _error_mitigations,
                std::initializer_list<const char*> _parameters);
        const char* error_code;
        const char* error_message;
        const size_t nr_error_mitigations;
        const char* const * error_mitigations;
        const size_t nr_parameters;
        const char* const * parameters;
        error_message_declaration * next;

    };

    class error_message_declaration_container {
    public:
        error_message_declaration_container() : root(nullptr) {}

        void register_error_message(error_message_declaration * msg) {
            error_message_declaration * n = root;

            if (nullptr == root) {
                root = msg;
            } else {
                while (n->next != nullptr) {
                    n = n->next;
                }
                n->next = msg;
            }
        }

        std::string collect_error_messages_as_json() {
            std::ostringstream msg_stream;
            error_message_declaration * n = root;
            msg_stream << "[";
            while(n != nullptr) {
                msg_stream << R"({ "code": ")" << n->error_code << R"(" ,  "message": ")" << n->error_message
                    << R"(", "parameters" : [)";
                for (size_t idxParam(0); idxParam < n->nr_parameters; idxParam++) {
                    msg_stream << "\"" <<n->parameters[idxParam] << "\",";
                }
                msg_stream << "], \"mitigations\":[";
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
        error_message_declaration * root;
    };

    extern error_message_declaration_container g_error_message_container;
#ifdef MAIN_ERROR_REPORTING_CPP
    error_message_declaration_container g_error_message_container;
#endif


    inline error_message_declaration::error_message_declaration(
            const char* _error_code,
            const char* _error_message,
            std::initializer_list<const char*> _error_mitigations,
            std::initializer_list<const char*> _parameters) :
            error_code(_error_code),
            error_message(_error_message),
            nr_error_mitigations(_error_mitigations.size()),
            error_mitigations(_error_mitigations.begin()),
            nr_parameters(_parameters.size()),
            parameters(_parameters.begin()) {
        g_error_message_container.register_error_message(this);
    }

#define DECLARE_PARAMETER_1(name, p1) const std::array<const char*, 1> g_parameter_##name = { p1 };
#define DECLARE_PARAMETER_2(name, p1, p2) const std::array<const char*, 2> g_parameter_##name = { p1, p2 };
#define DECLARE_PARAMETER_3(name, p1, p2, p3) const std::array<const char*, 3> g_parameter_##name = { p1, p2, p3 };
#define DECLARE_PARAMETER_4(name, p1, p2, p3, p4) const std::array<const char*, 4> g_parameter_##name = { p1, p2, p3, p4 };
#define DECLARE_PARAMETER_5(name, p1, p2, p3, p4, p5) const std::array<const char*, 5> g_parameter_##name = { p1, p2, p3, p4, p5 };
#define DECLARE_PARAMETER_6(name, p1, p2, p3, p4, p5, p6) const std::array<const char*, 6> g_parameter_##name = { p1, p2, p3, p4, p5, p6 };
#define DECLARE_PARAMETER_7(name, p1, p2, p3, p4, p5, p6, p7) const std::array<const char*, 7> g_parameter_##name = { p1, p2, p3, p4, p5, p6, p7 };
#define DECLARE_PARAMETER_8(name, p1, p2, p3, p4, p5, p6, p7, p8) const std::array<const char*, 8> g_parameter_##name = { p1, p2, p3, p4, p5, p6, p7, p8 };
#define DECLARE_PARAMETER_9(name, p1, p2, p3, p4, p5, p6, p7, p8, p9) const std::array<const char*, 9> g_parameter_##name = { p1, p2, p3, p4, p5, p6, p7, p8, p9 };
#define DECLARE_PARAMETER_10(name, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) const std::array<const char*, 10> g_parameter_##name = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

#   define GET_ARG_COUNT(...) INTERNAL_GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#   define INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, count, ...) count

#define ERR_ARRAY(...) { __VA_ARGS__ }
#define DECLARE_ERROR_CODE(error_code, error_message, mitigations, parameters) \
error_message_declaration g_##error_code { \
    #error_code, \
    error_message, \
    mitigations, \
    parameters \
    };

//    template<int flags = 0>
//    class ErrorBuilder {
//
//        template<int anyflags>
//        friend class NodeBuilder;
//        enum Flags {
//            Description,
//            Name,
//            Value,
//            TotalFlags
//        };
//
//    public:
//        template<int anyflags>
//        ErrorBuilder(const ErrorBuilder<anyflags>& cpy) : m_buildingNode(cpy.m_buildingNode) {};
//
//        template<int pos>
//        using NextBuilder = ErrorBuilder<flags | (1 << pos)>;
//
//        //The && at the end is import so you can't do b.withDescription() where b is a lvalue.
//        NextBuilder<Description> withDescription( const char* desc ) && {
//            m_buildingNode.description = desc;
//            return *this;
//        }
//        //other with* functions etc...
//
//        //needed so that if you store an incomplete builder in a variable,
//        //you can easily create a copy of it. This isn't really a problem
//        //unless you have optional values
//        ErrorBuilder<flags> operator()() & {
//            return ErrorBuilder<flags>(*this);
//        }
//
//        //Implicit cast from node builder to node, but only when building is complete
//        explicit operator typename std::conditional<flags == (1 << TotalFlags) - 1, error_message, void>::type() {
//            return m_buildingNode;
//        }
//    private:
//        error_message m_buildingNode;
//    };
//
//    template<typename ...Args>
//    struct parameters {
//    };
//
//    //template<typename ...Params>
//    struct error_message_description {
//        const char *code;
//        const char *message;
//      //  typedef parameters<Params...> tParameters;
//    };



//    class error_builder {
//    public:
//        error_builder(const char *code, const char *message) : code(code) {}
//
//    private:
//        const char *code;
//        const char *message;
//        std::ostringstream msg_stream;
//    };



}

#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_H
