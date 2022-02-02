
#ifndef ERROR_REPORTING_CPP_ERROR_MESSAGE_H
#define ERROR_REPORTING_CPP_ERROR_MESSAGE_H

/** @file
 * @brief Main header file for Exasol error-reporting-cpp
 *
 * Include this file in your project to use the error-reporting-cpp macros.
 *
 * @note Declare @link MAIN_ERROR_REPORTING_CPP  @endlink @b before including this file in your project in exactly one cpp file. This is where the global variables will be stored.
 * @note Declare @link ERROR_MESSAGE_COLLECTION @endlink @b before including this file in your project in any cpp file, if you want to support collection of all error codes, see error_reporting::error_message_declaration_container::collect_error_messages_as_json()
 */


#include "error-reporting-cpp/src/macro/error_message_no_param.h"
#include "error-reporting-cpp/src/macro/error_message_with_param.h"

#endif //ERROR_REPORTING_CPP_ERROR_MESSAGE_H

