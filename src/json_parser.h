#pragma once

#include <vector>
#include <map>
#include <string>

#include "commands/command.h"

class JsonParser
{
public:

    enum PraserState : int {
        PARSE_BEGIN = 0,
        PARSE_PARAM_NAME,
        PARSE_COLON,
        PARSE_DETECT_PARAM_VALUE,
        PARSE_INT_VALUE,
        PARSE_STRING_VALUE,
        PARSE_ARRAY_VALUE,
        PARSE_OBJECT_VALUE,
        PARSE_VALUE_ENDING,
        PARSE_END
    };

    JsonParser();

    void parse(char ch);

    const CommandParams& get_ref() const { return m_params; }
    CommandParams get() const { return m_params; }

    bool is_finished() const { return m_parser_state == PARSE_END; }

    void reset();

protected:
    void parse_param_name(int ch);

    void parse_colon(int ch);

    void parse_detect_param_value(int ch);

    void parse_int_value(int ch);
    void parse_string_value(int ch);
    void parse_array_value(int ch);
    void parse_object_value(int ch);

    void parse_value_ending(int ch);

private:
    PraserState m_parser_state;
    CommandParams m_params;
    std::string m_param;
    std::string m_value;
    size_t m_opened_quotes;

    static constexpr size_t MAX_PARAM_SIZE = 1024;
};


