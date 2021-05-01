#ifndef ENUMS_H
#define ENUMS_H

#define CRLF "\r\n"
#define BLANK "\n"

enum RequestType{
    GET,POST,
    PUT,DELETE,
    BAD
};

enum ProcessType{
    NO_ACTION,
    CREATE_ACTION,
    BRIDGE_ACTION
};

#endif // ENUMS_H
