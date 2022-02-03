
#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H

#include <type_traits>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <utility>
#include <string_view>


namespace error_reporting {

    /**
     * @brief Internal struct, representing a node of the internal linked list of error code messages.
     *
     * Contains all information for a particular error code, which are required to build the error collection JSON.
     * Also contains a pointer to the next element of the linked list.
     */
    class error_message_declaration_internal {
    public:
        void setAndRegister(
                const std::string & _error_code,
                const std::string & _error_message,
                const std::vector<std::string> & error_mitigations,
                const std::vector<std::string> & parameters);

        std::string str();
        std::string toJSON();
        error_message_declaration_internal * next;
    private:
        std::string error_code;
        std::string error_message;
        std::vector<std::string> error_mitigations;
        std::vector<std::string> parameters;
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
                msg_stream << n->toJSON() << ", ";
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
    inline void error_message_declaration_internal::setAndRegister(const std::string &_error_code,
                                                                   const std::string &_error_message,
                                                                   const std::vector<std::string> & _error_mitigations,
                                                                   const std::vector<std::string> & _parameters) {
        error_code = _error_code;
        error_message = _error_message;
        error_mitigations = _error_mitigations;
        parameters = _parameters;
#ifdef ERROR_MESSAGE_COLLECTION
        g_error_message_container.register_error_message(this);
#endif
    }

    inline std::string error_message_declaration_internal::toJSON() {
        std::ostringstream msg_stream;
        msg_stream << R"({ "code": ")" << error_code << R"(" ,  "message": ")" << error_message
        << R"(", "parameters" : [)";
        for (const auto & parameter : parameters) {
            msg_stream << "\"" << parameter << "\",";
        }
        msg_stream << R"(], "mitigations" : [)";
        for (const auto & mitigation : error_mitigations) {
            msg_stream << "\"" << mitigation << "\",";
        }
        msg_stream << R"(]})";
        return msg_stream.str();
    }

} //namespace error_reporting




#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_INTERNAL_H
