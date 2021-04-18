#include <gtest/gtest.h>
#include "../json_parser.h"

TEST(TestJsonParser, CorrectParsingBasisScenarious)
{
    std::vector<std::string> test_source1 = {
        //without whitespaces
        "{\"a\":5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,3,2,1,[1,2,3]], \"object\":{\"a1\":66,\"b1\":12,\"text\":\"Hello!\"}}",

        //whitespaces
        " {\"a\"   :5, \"b\":   9, \"text\"   :   \"Hello world\", \"array\"   : [5  ,4,    3,  2,  1,[1,  2,3]],  \"object\":{\"a1\":66,\"b1\":12,\"text\":\"Hello!\"}  }    ",

        //whitespace+formatted
        " { "
        "   \"a\"   :5,"
            " \"b\":   9, "
            "\"text\"   :   \"Hello world\", "
            "   \"array\"   : [5  ,4,    3,  2,  1,[1,2,3] ] ,   "
            " \"object\":{\"a1\":66,\"b1\":12,\"text\":\"Hello!\"}"
        "}    ",

        //payload between trash
        "asdasdasd{\"a\":5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,3,2,1,[1,2,3]],\"object\":{\"a1\":66,\"b1\":12,\"text\":\"Hello!\"}}dasdasdasd",
    };

    for(auto& src: test_source1 ) {
        JsonParser jp;

        for(auto ch : src) {
            jp.parse(ch);
        }

        ASSERT_EQ(jp.is_finished(), true);
        ASSERT_EQ(jp.get()["a"], "5");
        ASSERT_EQ(jp.get()["b"], "9");
        ASSERT_EQ(jp.get()["text"], "Hello world");
        ASSERT_EQ(jp.get()["array"], "[5,4,3,2,1,[1,2,3]]");
        ASSERT_EQ(jp.get()["object"], "{\"a1\":66,\"b1\":12,\"text\":\"Hello!\"}");
    }
}

TEST(TestJsonParser, BrokenArrayIsOk)
{
    JsonParser jp;

    //broken array,doesn't matter
    std::string test_source = "{\"a\":5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,,,{?1,[1,2,3]], \"object\":{\"a1\":66,\"b1\":12,\"text\":\"Hello!\"}}";
    for(auto ch : test_source) {
        jp.parse(ch);
    }

    ASSERT_EQ(jp.is_finished(), true);
    ASSERT_EQ(jp.get()["a"], "5");
    ASSERT_EQ(jp.get()["b"], "9");
    ASSERT_EQ(jp.get()["text"], "Hello world");
    ASSERT_EQ(jp.get()["array"], "[5,4,,,{?1,[1,2,3]]");
    ASSERT_EQ(jp.get()["object"], "{\"a1\":66,\"b1\":12,\"text\":\"Hello!\"}");
}

TEST(TestJsonParser, BrokenInernalObjectIsOk)
{
    JsonParser jp;

    //broken object,doesn't matter
    std::string test_source = "{\"a\":5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,3,2,1,[1,2,3]], \"object\":{\"a1\":66,b1:12,\"text\":\"Hello!\"}}";
    for(auto ch : test_source) {
        jp.parse(ch);
    }

    ASSERT_EQ(jp.is_finished(), true);
    ASSERT_EQ(jp.get()["a"], "5");
    ASSERT_EQ(jp.get()["b"], "9");
    ASSERT_EQ(jp.get()["text"], "Hello world");
    ASSERT_EQ(jp.get()["array"], "[5,4,3,2,1,[1,2,3]]");
    ASSERT_EQ(jp.get()["object"], "{\"a1\":66,b1:12,\"text\":\"Hello!\"}");
}

TEST(TestJsonParser, IncorrectParsing)
{
    std::vector<std::string> test_source = {
        //miss endig }
        "{\"a\":5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,3,2,1]",

        //miss starting {
        "\"a\":5, \"b\":9, \"text\":Hello world, \"array\":[5,4,3,2,1]}",

        //paaram without quotes
        "{a:5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,3,2,1]}",

        //string without quotes
        "{\"a\":5, \"b\":9, \"text\":Hello world, \"array\":[5,4,3,2,1]}",

        //miss ]
        "{\"a\":5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,3,2,1}",

        //miss ]
        "{\"a\":5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,3,2,1,[1,2,3]}",

        //miss ,
        "{\"a\":5, \"b\":9 \"text\":\"Hello world\", \"array\":[5,4,3,2,1]}",

        //miss } for internal object
        "{\"a\":5, \"b\":9, \"text\":\"Hello world\", \"array\":[5,4,3,2,1], \"object\":{\"a1\":2}",
    };

    for(auto& src: test_source) {
        JsonParser jp;

        for(auto ch : src) {
            jp.parse(ch);
        }

        ASSERT_EQ(jp.is_finished(), false);
    }
}
