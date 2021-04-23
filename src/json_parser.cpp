#include "json_parser.h"
#include <algorithm>
#include <cctype>

JsonParser::JsonParser() :
    m_parser_state(PARSE_BEGIN),
    m_opened_quotes(0)
{
    m_value.reserve(1024);
}

void JsonParser::parse(char ch)
{
    switch(m_parser_state)
    {
        case PARSE_BEGIN:
        {
            if(ch == '{') {
                m_opened_quotes = 0;
                m_parser_state = PARSE_PARAM_NAME;
            } else {
                reset();
            }
            break;
        }

        case PARSE_PARAM_NAME:
        {
            parse_param_name( static_cast<int>(ch) );
            break;
        }

        case PARSE_COLON:
        {
            parse_colon( static_cast<int>(ch) );
            break;
        }

        case PARSE_DETECT_PARAM_VALUE:
        {
            parse_detect_param_value( static_cast<int>(ch) );
            break;
        }

        case PARSE_INT_VALUE:
        {
            parse_int_value( static_cast<int>(ch) );
            break;
        }

        case PARSE_STRING_VALUE:
        {
            parse_string_value( static_cast<int>(ch) );
            break;
        }

        case PARSE_OBJECT_VALUE:
        {
            parse_object_value( static_cast<int>(ch) );
            break;
        }

        case PARSE_ARRAY_VALUE:
        {
            parse_array_value( static_cast<int>(ch) );
            break;
        }

        case PARSE_VALUE_ENDING:
        {
            parse_value_ending( static_cast<int>(ch) );
            break;
        }

        case PARSE_END:
        {
            break;
        }
    }
}

void JsonParser::parse_param_name(int ch)
{
    if( std::isspace(ch) ) {
        return;
    }

    if ( std::ispunct(ch) ) {
        if (ch == '\"') {
            ++m_opened_quotes;

            if (! (m_opened_quotes % 2) ) {
                if( m_param.empty() ) {
                    reset();
                    return;
                }
                m_opened_quotes = 0;
                m_parser_state = PARSE_COLON;
            }
            return;
        }

        reset();
    } else {
        if ( m_opened_quotes == 1 ) {
            m_param+= ch;
        } else {
            reset();
        }
    }
}

void JsonParser::parse_colon(int ch)
{
    if( std::isspace(ch) ) {
        return;
    }

    if (ch == ':') {
        m_parser_state = PARSE_DETECT_PARAM_VALUE;
        return;
    }

    reset();
}

void JsonParser::parse_detect_param_value(int ch)
{
    if( std::isspace(ch) ) {
        return;
    }

    if ( std::ispunct(ch) ) {
        if (ch == '\"') {
            m_parser_state = PARSE_STRING_VALUE;
            m_opened_quotes = 1;
            return;
        }

        if (ch == '{') {
            m_parser_state = PARSE_OBJECT_VALUE;
            m_opened_quotes = 1;
            m_value+= ch;
            return;
        }

        if (ch == '[') {
            m_parser_state = PARSE_ARRAY_VALUE;
            m_opened_quotes = 1;
            m_value+= ch;
            return;
        }

        reset();
    } else {
        if ( std::isdigit(ch) ) {
            m_parser_state = PARSE_INT_VALUE;
        } else {
            reset();
            return;
        }

        if (m_value.size() <= MAX_PARAM_SIZE) {
            m_value+= ch;
        } else {
            reset();
        }
    }
}

void JsonParser::parse_int_value(int ch)
{
    if( std::isspace(ch) ) {
        return;
    }

    if ( std::isdigit(ch) ) {
        if (m_value.size() <= MAX_PARAM_SIZE) {
            m_value+= ch;
        } else {
            reset();
        }
    } else {
        parse_value_ending(ch);
    }
}

void JsonParser::parse_string_value(int ch)
{
    if (ch == '\"') {
        ++m_opened_quotes;

        if ( !(m_opened_quotes % 2) ) {
            m_opened_quotes = 0;
            m_parser_state = PARSE_VALUE_ENDING;
            return;
        }
    }

    if (m_value.size() <= MAX_PARAM_SIZE) {
        m_value+= ch;
    } else {
        reset();
    }
}

void JsonParser::parse_value_ending(int ch)
{
    if( std::isspace(ch) ) {
        return;
    }

    if (ch == ',') {
         if ( m_value.empty() ) {
            reset();
            return;
         }

         m_parser_state = PARSE_PARAM_NAME;
         m_params[m_param]= m_value;

         m_param.clear();
         m_value.clear();
         return;
    }

    if (ch == '}') {
        if ( m_value.empty() ) {
            reset();
            return;
        }

        m_parser_state = PARSE_END;
        m_params[m_param]= m_value;
        m_param.clear();
        m_value.clear();
        return;
    }

    reset();
}

void JsonParser::parse_array_value(int ch)
{
    if( std::isspace(ch) ) {
        return;
    }

    if (ch == '[') {
        ++m_opened_quotes;
    }

    if (ch == ']') {
        m_value+= ch;
        if(m_opened_quotes) {
            --m_opened_quotes;
        }

        if (!m_opened_quotes) {
            m_parser_state = PARSE_VALUE_ENDING;
        }
        return;
    }

    if (m_value.size() <= MAX_PARAM_SIZE) {
        m_value+= ch;
    } else {
        reset();
    }
}

void JsonParser::parse_object_value(int ch)
{
    if( std::isspace(ch) ) {
        return;
    }

    if (ch == '{') {
        ++m_opened_quotes;
    }

    if (ch == '}') {
        m_value+= ch;
        if(m_opened_quotes) {
            --m_opened_quotes;
        }

        if (!m_opened_quotes) {
            m_parser_state = PARSE_VALUE_ENDING;
        }
        return;
    }

    if (m_value.size() <= MAX_PARAM_SIZE) {
        m_value+= ch;
    } else {
        reset();
    }
}

void JsonParser::reset()
{
    m_parser_state = PARSE_BEGIN;

    m_opened_quotes = 0;

    if ( !m_params.empty() )
        m_params.clear();

    if ( !m_value.empty() )
        m_value.clear();
}
