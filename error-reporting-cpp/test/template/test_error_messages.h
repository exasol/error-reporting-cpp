#ifndef ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H
#define ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H

#include "error-reporting-cpp/src/template/error_message.h"


struct ParamDummy1 {
    struct Desc : error_reporting::ParamDesc {
        Desc():ParamDesc("Dummy1", "Description for Dummy2"){};
    };

    std::string value;
    ParamDummy1(const std::string & value) : value(value){};
};

struct ParamDummy2{
    struct Desc : error_reporting::ParamDesc {
        Desc():ParamDesc("Dummy2", "Description for Dummy1"){};
    };

    int value;
    ParamDummy2(int value):value(value){};
};

class EC_DB_2_cls : public error_reporting::BaseError<ParamDummy1, ParamDummy2>{
public:
    EC_DB_2_cls(std::string code, std::string message, const std::vector<std::string> & mitigations)
        :BaseError(code, message, mitigations) {};
};


CREATE_ERROR_MSG(error_reporting::SimpleError, EC_DB_1, ( "EC_DB_1", "Out of memory.", {"Buy more RAM"}))
CREATE_ERROR_MSG(EC_DB_2_cls,  EC_DB_2, ("EC_DB_2",  "hd error {{Dummy1}} , {{Dummy2}}.", {"Replace {{Dummy1}}"}))
CREATE_ERROR_MSG(error_reporting::SimpleError,  EC_DB_3, ("EC_DB_3",  "Kernel panic", {}))


#endif //ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H


