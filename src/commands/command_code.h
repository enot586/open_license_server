#pragma once

enum CommandCode : int {
    GET_VERSION = 0,
    GENERATE_LICENSE,
};

enum CommandStatus: int {
    OK = 0,
    ERROR,
};
