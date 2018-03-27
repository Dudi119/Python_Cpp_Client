#include "gtest/gtest.h"
#include <Python.h>
#include "PythonEmbedder.h"
#include "CPythonClassTestModule.h"

static int _argc;
static char **_argv;

namespace pycppconnTest {

    bool CPythonClassTestSubject::m_valid = false;

    class CPythonClassTest : public ::testing::Environment {
    public:
        CPythonClassTest() {}

        void SetUp() override {
            PythonEmbedder::InitiateInterperter("CPythonClassTest", _argc, _argv);
            const char *testingScript = "from CPythonClassTestModule import TestClass\n";
            PyRun_SimpleString(testingScript);
        }

        void TearDown() override {
            PythonEmbedder::TerminateInterperter();
        }
    };


    TEST(CPythonClassTest, StaticMethod) {
        const char *testingScript = "TestClass.Setter()\n"
                                    "value = TestClass.Getter()";
        PyRun_SimpleString(testingScript);
        ASSERT_EQ(CPythonClassTestSubject::Getter(), true);
        ASSERT_EQ(CPythonClassTestSubject::Getter(), PythonEmbedder::GetAttribute<bool>("value"));
    }


    TEST(CPythonClassTest, PODByValueArgumentConstructor) {
        const char *testingScript = "a = TestClass(7)";
        PyRun_SimpleString(testingScript);

    }

    TEST(CPythonClassTest, PODByValueMember) {
        const char *testingScript = "a = TestClass(7)\n"
                                    "a.byValueInt = 5\n";
        PyRun_SimpleString(testingScript);
        ASSERT_EQ(PythonEmbedder::GetAttribute<int>("a.byValueInt"), 5);
    }
}

int main(int argc, char **argv) {
    _argc = argc;
    _argv = argv;
    ::testing::AddGlobalTestEnvironment(new pycppconnTest::CPythonClassTest);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}