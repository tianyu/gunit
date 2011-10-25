/*
G-Unit is a small unit testing library.

Copyright (C) 2011 Tianyu Zhu

Define tests by defining child-classes of the Test structure. Each Test object
that is created will be run as a test. Functions defined in the Assert
namespace can be used while executing the test to safetly assert for
conditions. For example:

// Definition of MyTest
struct MyTest : public Test {
    MyTest(char const*file) : file(file), Test("My Test") {}
    void execute() const {
        // Open the file ...
        Assert::True(filestream.open(file), "Open the file stream");
        // Run the test with contents of file ...
        Assert:: ...
    }
private:
    char const*file;
};

// Run MyTest using file1.txt and file2.txt as input.
MyTest test_with_file1("file1.txt"), test_with_file2("file2.txt");

Note that G-Unit already defines the main function in its implementation.

*/

#ifndef GUNIT_H
#define GUNIT_H

#include <cstring>

struct Test {
    Test(char const*_name);
    char const*const name;
    virtual void setup();
    virtual void execute() = 0;
    virtual void cleanup() throw();
};

namespace Assert {

void Fail(char const*msg, char const*format, ...);

void Fail(char const*msg = NULL);

void True(bool expr, char const*msg = NULL);

void False(bool expr, char const*msg = NULL);

void Null(void *prt, char const*msg = NULL);

void NotNull(void *prt, char const*msg = NULL);

template<typename T1, typename T2>
void Equal(T1 expected, T2 tested, char const*msg = NULL);

} // namespace Assert

#endif // GUNIT_H
