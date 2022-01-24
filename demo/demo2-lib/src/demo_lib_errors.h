#ifndef ERROR_REPORTING_CPP_DEMO2_LIB_ERRORS_H
#define ERROR_REPORTING_CPP_DEMO2_LIB_ERRORS_H

#include "error-reporting-cpp/src/error_message.h"

namespace demo2_errors {

    DECLARE_ERR_MSG(DEMO2_1, "Log with value smaller equal zero not defined.",MITIGATIONS());

}

#endif //ERROR_REPORTING_CPP_DEMO2_LIB_ERRORS_H
