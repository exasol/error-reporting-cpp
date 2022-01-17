#ifndef ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H
#define ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H

#include "error-reporting-cpp/src/error_message.h"


DECLARE_ERR_MSG(EC_DB_1, "Out of memory.", MITIGATIONS("Buy more RAM"));


DECLARE_ERR_MSG_PARAMS(EC_DB_2, "hd error {{Dummy1}} , {{Dummy2}}.",
                       MITIGATIONS("Replace {{Dummy1}}"), PARAM("Description for Dummy1", Dummy1), PARAM("Description for Dummy2", Dummy2));


DECLARE_ERR_MSG(EC_DB_3, "Kernel panic", MITIGATIONS());


#endif //ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H


