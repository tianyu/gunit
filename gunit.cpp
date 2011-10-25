#include "upp.h"
#include "gunit.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdarg>
#include <cstdio>
#include <exception>
#include <auto_ptr.h>

using namespace std;

struct AssertException {
    char const*msg;

    AssertException(char const*msg) :
        msg(msg? strdup(msg):NULL) {}

    ~AssertException() { delete msg; }
};

vector<Test*>tests = vector<Test*>();

Test::Test(char const*_name): name(_name) {
    tests.push_back(this);
}

void Test::setup() {}

void Test::cleanup() throw() {}

void uMain::main() {
    int fails = 0, errors = 0;
    for (vector<Test*>::iterator i=tests.begin(); i<tests.end(); ++i) {
        Test &test = **i;
        try {
            cout << "Testing " << setw(65) << setfill('.') << left << test.name;
            cout.flush();
            test.setup();
            test.execute();
            cout << ".Passed" << endl;
        } catch (AssertException &e) {
            ++fails;
            cout << ".FAILED" << endl;
            if (e.msg) cout << e.msg << endl;
            test.cleanup();
        } catch (exception &e) {
            ++errors;
            cout << ".FAILED\nEncountered EXCEPTION" << endl;
            if (e.what()) cout << e.what() << endl;
            test.cleanup();
        } catch (...) {
            ++errors;
            cout << "..ERROR" << endl;
            test.cleanup();
            throw;
        }
    }

    if (fails) cerr << fails << " of " << tests.size() << " tests FAILED." << endl;
    if (errors) cerr << errors << " of " << tests.size() << " tests ended with ERROR." << endl;
    uRetCode = fails + errors;
}

namespace Assert {

void Fail(char const*msg) {
    throw AssertException(msg);
}

void Fail(char const*msg, char const*format, ...) {
    static char buf[4096];
    char *b = buf;

    // If there is a message, copy it to the buffer.
    if (msg) {
        while (*msg) *(b++) = *(msg++);
        *(b++) = ':';
        *(b++) = '\n';
    }

    va_list msgs;
    va_start(msgs, format);
    vsprintf(b, format, msgs);
    va_end(msgs);
    Fail(buf);
}

void True(bool expr, const char *msg) {
    if (expr) return;
    Fail(msg, "Expected true but was false");
}

void False(bool expr, const char *msg) {
    if (!expr) return;
    Fail(msg, "Expected false but was true");
}

void Null(void *ptr, const char *msg) {
    if (ptr == NULL) return;
    Fail(msg, "Expected NULL, got 0x%x", ptr);
}

void NotNull(void *ptr, const char *msg) {
    if (ptr) return;
    Fail(msg, "Expected NOT NULL, got 0x%x", ptr);
}

template<>
void Equal<int, int>
(int expected, int tested, char const*msg) {
    if (expected == tested) return;
    Fail(msg, "Expected: %d, Got: %d", expected, tested);
}

template<>
void Equal<char const*, char const*>
(char const*expected, char const*tested, char const*msg) {
    if (expected == tested) return;
    if (expected == NULL || tested == NULL) goto fail;
    for (const char *e = expected, *t = tested; *e || *t;)
        if (*e++ != *t++) goto fail;
    return;

fail:
    Fail(msg, "Expected: \"%s\"\nGot: \"%s\"", expected, tested);
}

} // namespace Assert

