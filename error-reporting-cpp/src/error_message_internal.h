
#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H

#include <regex>
#include <sstream>
#include <utility>
#include <string_view>

/** @file
 * @brief Internal header file for Exasol error-reporting-cpp
 *
 * Includes classes and macros used to store the internal linked list of error codes.
 *
 */


/**
 * @brief Global error reporting namespace.
 *
 * Used to encapsulate error reporting classes.
 */
namespace error_reporting {

    /**
     * @brief Internal struct, representing a node of the internal linked list of error code messages.
     *
     * Contains all information for a particular error code, which are required to build the error collection JSON.
     * Also contains a pointer to the next element of the linked list.
     */
    struct error_message_declaration_internal {
        explicit error_message_declaration_internal(
                const std::string_view & _error_code,
                const std::string_view & _error_message,
                const std::string_view & _error_mitigations,
                size_t _nr_parameters,
                const char* const* const _parameters);

        std::string str();
        const std::string_view & error_code;
        const std::string_view & error_message;
        const std::string_view & error_mitigations;
        const size_t nr_parameters;
        const char * const * const parameters;
        error_message_declaration_internal * next;
    };

    /**
     * @brief Global error reporting container.
     *
     * Contains the root node of the internal error code linked list, which is used to build the error collection JSON.
     * Defines a method to build the JSON from the internal linked list, \see error_message_declaration_container::collect_error_messages_as_json()
     */
    class error_message_declaration_container {
    public:
        /**
         * @brief Constructor.
         */
        error_message_declaration_container() : root(nullptr) {}

        /**
         * @brief Adds a new node to the internal linked list of error-codes.
         */
        void register_error_message(error_message_declaration_internal * msg) {
            if (nullptr == root) {
                root = msg;
            } else {
                msg->next = root;
                root = msg;
            }
        }

        /**
         * @brief Collects all elements in the list as JSON object.
         *
         * @return A string containing all registered error-codes in the following format:
         *          @code{.json}
         *          [{ "code": "EC_1" ,  "message": "Error Message 1",
                       "parameters" : ["Description of Parameter1", "Description of Parameter2"],
                       "mitigations" : "Mitigations"}]
                    @endcode
         */
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
    /**
     * @brief Global container of all error-codes.
     */
    error_message_declaration_container g_error_message_container;
#endif

    /**
     * @brief Constructor.
     */
    inline error_message_declaration_internal::error_message_declaration_internal(
            const std::string_view & _error_code,
            const std::string_view & _error_message,
            const std::string_view & _error_mitigations,
            size_t _nr_parameters,
            const char* const * const _parameters) :
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

    inline std::string error_message_declaration_internal::str() {
        std::string msg = std::string(error_code) + ": " + std::string(error_message);
        if (!error_mitigations.empty()) {
            msg.append(" ");
            msg.append(error_mitigations);
        }
        return msg;
    }
}

/**
 * @internal
 * Helper macro to select _f0 or _f1
 *  @endinternal
 */
#define FUNC_CHOOSER(_f0, _f1, ...) _f1
/**
 * @internal
 * Helper macro.
 *  @endinternal
 */
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
/**
 * @internal
 * Helper macro to choose F_f0 or F_f1
 *  @endinternal
 */
#define CHOOSE_FROM_ARG_COUNT(F, ...) FUNC_RECOMPOSER((__VA_ARGS__, F##_1, ))
/**
 * @internal
 * Helper macro to choose F_f0 or F_f1
 *  @endinternal
 */
#define NO_ARG_EXPANDER(FUNC) ,FUNC ## _0

/**
 * @internal
 * Following macros are used to support optional arguments for MITIGATIONS().
 * Need to allow empty arguments for MITIGATIONS macro:
 * Clients can use MITIGATIONS("ABC") or MITIGATIONS()
 *  @endinternal
 */
#define MACRO_CHOOSER(FUNC, ...) CHOOSE_FROM_ARG_COUNT(FUNC, NO_ARG_EXPANDER __VA_ARGS__ (FUNC))

/**
 * @brief Macro to declare one or zero mitigations.
 *
 * The mitigation argument must be a string. It might contain parameter placeholders, for example:
 * @code{.cpp}
 *  MITIGATION("Delete something from {{Device}}.")
 * @endcode
 *
 * If there is more than one mitigation, you can separate them by linebreak.
 *
 * You can skip the parameter if there are no mitigations:
 * @code{.cpp}
 *  MITIGATION()
 * @endcode
 *
 */
#define MITIGATIONS(...)  MACRO_CHOOSER(MITIGATIONS, __VA_ARGS__)(__VA_ARGS__)

/**
 * @internal
 * Used if MITIGATIONS is called with an argument. Adds the constexpr member mitigations_str to the generated class.
 *  @endinternal
 */
#define MITIGATIONS_1(x) \
static constexpr std::string_view  mitigations_str = (x);

/**
 * @internal
 * Used if MITIGATIONS is called without an argument. Adds the (empty) constexpr member mitigations_str to the generated class.
 *  @endinternal
 */
#define MITIGATIONS_0() \
static constexpr std::string_view  mitigations_str = "";


#ifdef CREATE_ERROR_MESSAGE_CLASS_INSTANCES
#define ERROR_MESSAGE_CLASS_INSTANCE_MODIFIER
#else
#define ERROR_MESSAGE_CLASS_INSTANCE_MODIFIER extern
#endif



#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H

