//
// Created by Dmitriy Steshenko on 25.05.2020.
//

#include "../Table.h"

#include <boost/test/unit_test.hpp>

class TestModel: public Model {
public:
    TestModel() {}
    TestModel(const TestModel& src):Model(src) {}

    virtual const map<std::string, TypeName> Fields() const {
        map<std::string, TypeName> f;
        TypeName tn = {tstring, "ID"};
        f.insert(make_pair("ID", tn));
        tn = {tnumber, "Тест1"};
        f.insert(make_pair("TestNum", tn));
        tn = {tstring, "Тест2"};
        f.insert(make_pair("TestString", tn));
        tn = {tarray, "Тест3"};
        f.insert(make_pair("TestArray", tn));
        tn = {tboolean, "Тест4"};
        f.insert(make_pair("TestBool", tn));
        return f;
    }
    virtual Model* clone() { return new TestModel(*this); }
};

class TestTable: public Table<TestModel> {
public:
    static auto& instance() {
        static TestTable table;
        return table;
    }

    TestTable():Table<TestModel>("Test") { searchPath = "../../Tests/"; }
};

BOOST_AUTO_TEST_SUITE(ModelTestSuite)

struct ModelFixture {
    TestTable test;
    ModelFixture() { test.load(); }
};

    BOOST_FIXTURE_TEST_CASE(ModelTestLoadSize, ModelFixture) {
        BOOST_CHECK_EQUAL(test.elements().size(), 2);
    }
    BOOST_FIXTURE_TEST_CASE(ModelTestLoadElementSize, ModelFixture) {
        for (auto &i : test.elements()) {
            BOOST_CHECK_EQUAL(i->Values().size(), 6);
        }
    }
    BOOST_FIXTURE_TEST_CASE(ModelTestLoadElementType, ModelFixture) {
        for (auto &i : test.elements()) {
            for (auto &j : i->Values()) {
                if (j.first == "ID") {
                    BOOST_CHECK_EQUAL(j.second.type, tstring);
                } else if (j.first == "TestNum") {
                    BOOST_CHECK_EQUAL(j.second.type, tnumber);
                } else if (j.first == "TestString") {
                    BOOST_CHECK_EQUAL(j.second.type, tstring);
                } else if (j.first == "TestArray") {
                    BOOST_CHECK_EQUAL(j.second.type, tarray);
                } else if (j.first == "TestBool") {
                    BOOST_CHECK_EQUAL(j.second.type, tboolean);
                }
            }
        }
    }
    BOOST_FIXTURE_TEST_CASE(ModelTestFind, ModelFixture) {
        vector<TestModel> a;
        a = test.find([](TestModel el) { return el["TestNum"].value.tnumber == 1234; });
        BOOST_CHECK_EQUAL(a.size(), 1);
        a = test.find([](TestModel el) { return el["TestNum"].value.tnumber == 2222; });
        BOOST_CHECK_EQUAL(a.size(), 0);
    }

BOOST_AUTO_TEST_SUITE_END()