//
// Created by Dmitriy Steshenko on 25.05.2020.
//

#include "../View.h"

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






class TestOneModel: public Model {
public:
    TestOneModel() {}
    TestOneModel(const TestOneModel& src):Model(src) {}

    virtual const map<std::string, TypeName> Fields() const {
        map<std::string, TypeName> f;
        TypeName tn = {tstring, "ID"};
        f.insert(make_pair("ID", tn));
        tn = {tnumber, "Тест1_1"};
        f.insert(make_pair("NumTest", tn));
        tn = {tstring, "Тест2_2"};
        f.insert(make_pair("StringTest", tn));
        tn = {tarray, "Тест3_3"};
        f.insert(make_pair("ArrayTest", tn));
        tn = {tboolean, "Тест4_4"};
        f.insert(make_pair("BoolTest", tn));
        return f;
    }
    virtual Model* clone() { return new TestOneModel(*this); }
};

class TestOneTable: public Table<TestOneModel> {
public:
    static auto& instance() {
        static TestOneTable table;
        return table;
    }

    TestOneTable():Table<TestOneModel>("TestOne") { searchPath = "../../Tests/"; }
};






class TestView: public View {
public:
    TestView():View(TestTable::instance(), "TestOne_ID", TestOneTable::instance(), "ID") {
        m_printFields.push_back(TestTable::instance().name() + ".TestNum");
        m_printFields.push_back(TestTable::instance().name() + ".TestString");
        m_printFields.push_back(TestTable::instance().name() + ".TestBool");

        m_printFields.push_back(TestOneTable::instance().name() + ".NumTest");
        m_printFields.push_back(TestOneTable::instance().name() + ".StringTest");
        m_printFields.push_back(TestOneTable::instance().name() + ".BoolTest");
    }
};






BOOST_AUTO_TEST_SUITE(ViewTestSuite)

    struct ViewFixture {
        ViewFixture() {
            TestTable::instance().load();
            TestOneTable::instance().load();
        }
    };

    BOOST_FIXTURE_TEST_CASE(ViewTestFind, ViewFixture) {
        TestView testView;
        auto v = ElementValue(1234);
        auto f = "Test.TestNum";
        auto a = testView.find([f, v](ViewModel *el) { return (*el)[f].asString() == v.asString(); });
        BOOST_CHECK_EQUAL(a.size(), 1);
    }

BOOST_AUTO_TEST_SUITE_END()