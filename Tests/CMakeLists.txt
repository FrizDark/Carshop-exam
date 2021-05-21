set (Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

add_executable (Boost_Tests_run ElementValueTest.cpp ../Model.h ../Model.cpp ModelTest.cpp ../BaseTable.h ../BaseTable.cpp ../Table.h ViewTest.cpp ../View.h ../View.cpp)
target_link_libraries (Boost_Tests_run ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})