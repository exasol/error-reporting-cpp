#ifndef ERROR_REPORTING_CPP_DEMO_LIB_ERRORS_H
#define ERROR_REPORTING_CPP_DEMO_LIB_ERRORS_H

#include "error-reporting-cpp/src/error_message.h"

namespace demo_errors {

    DECLARE_ERR_MSG(DEMO_1, "Division by zero.",MITIGATIONS());

    DECLARE_ERR_MSG_PARAMS(DEMO_2, "Parameter {{FUNC_PARAM}} out of range. Value {{VALUE}} is invalid.",
                           MITIGATIONS("Change {{FUNC_PARAM}}"), PARAM("Parameter for division", FUNC_PARAM), PARAM("Value of parameter for division", VALUE));
}

#endif //ERROR_REPORTING_CPP_DEMO_LIB_ERRORS_H
