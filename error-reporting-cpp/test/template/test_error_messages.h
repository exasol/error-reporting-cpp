#ifndef ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H
#define ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H

#include "error-reporting-cpp/src/template/error_message.h"

struct EC_DB_1_class : error_reporting::SimpleError {
    EC_DB_1_class() : error_reporting::SimpleError("EC_DB_1", "Out of memory.", {"Buy more RAM"})
    {}
};

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

struct EC_DB_2_cls : error_reporting::BaseError<ParamDummy1, ParamDummy2>{
    EC_DB_2_cls() :
    error_reporting::BaseError<ParamDummy1, ParamDummy2>("EC_DB_2",  "hd error {{Dummy1}} , {{Dummy2}}.", {"Replace {{Dummy1}}"}) {}
};

struct EC_DB_3_class : error_reporting::SimpleError {
    EC_DB_3_class() : error_reporting::SimpleError("EC_DB_3",  "Kernel panic", {})
    {}
};

ERROR_MSG_PREFIX EC_DB_1_class EC_DB_1;
ERROR_MSG_PREFIX EC_DB_2_cls EC_DB_2;
ERROR_MSG_PREFIX EC_DB_3_class EC_DB_3;


#endif //ERROR_REPORTING_CPP_TEST_ERROR_MESSAGES_H


