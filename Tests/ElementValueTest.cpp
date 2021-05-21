//
// Created by Dmitriy Steshenko on 24.05.2020.
//

#include "../Model.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE elementValue
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(elementValueSuite)

struct ElementFixture {
    ElementValue ev;
};

    BOOST_FIXTURE_TEST_CASE(elementValueInt, ElementFixture) {
        ev = ElementValue(5);
        BOOST_CHECK_EQUAL(ev.value.tnumber, 5);
        BOOST_CHECK_EQUAL(ev.type, tnumber);
    }
    BOOST_FIXTURE_TEST_CASE(elementValueDouble, ElementFixture) {
        ev = ElementValue(5.5);
        BOOST_CHECK_EQUAL(ev.value.tnumber, 5.5);
        BOOST_CHECK_EQUAL(ev.type, tnumber);
    }
    BOOST_FIXTURE_TEST_CASE(elementValueString, ElementFixture) {
        ev = ElementValue("test");
        BOOST_CHECK_EQUAL(*ev.value.tstring, "test");
        BOOST_CHECK_EQUAL(ev.type, tstring);
    }
    BOOST_FIXTURE_TEST_CASE(elementValueVector, ElementFixture) {
        vector<ElementValue> a = {1, 2, 3, 4};
        ev = ElementValue(a);
        BOOST_CHECK_EQUAL(ev.type, tarray);
    }

BOOST_AUTO_TEST_SUITE_END();