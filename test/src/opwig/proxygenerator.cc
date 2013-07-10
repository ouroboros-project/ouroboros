
#include <fstream>
#include <sstream>

namespace opwig {
namespace gen {
namespace testing {

using std::ios_base;
using std::ifstream;
using std::stringstream;
using std::string;
using std::cout;
using std::endl;
using opwig::gen::ProxyGenerator;

class ProxyGeneratorTest : public ::testing::Test {

  protected:

    ProxyGenerator      generator_;
    Ptr<Scope>          given_scope_;
    ifstream            generated_code_;

    const static string COMMENT;

    ProxyGeneratorTest () : 
        generator_(OUROBOROS_TEST_DUMP_DIR),
        given_scope_(Namespace::Create("")) {}

    virtual ~ProxyGeneratorTest () {
        generated_code_.close();
    }

    virtual void SetUp () {}

    size_t Generate () {
        return generator_.Generate(given_scope_);
    }

    void AddEmptyNonVirtualClass () {
        string class_name = "NonVirtualClass";
        given_scope_->AddNestedClass(Class::Create(class_name, {}));
    }

    void AddNonEmptyNonVirtualClass (const string& class_name) {
        Ptr<Class> the_class = Class::Create(class_name, {});
        given_scope_->AddNestedClass(the_class);
        the_class->AddNestedFunction(Function::Create("~"+class_name, "", {}));
    }

    void AddEmptyVirtualClass (const string& class_name) {
        Ptr<Class> the_class = Class::Create(class_name, {});
        given_scope_->AddNestedClass(the_class);
        the_class->AddNestedFunction(Function::Create("~"+class_name, "", {}, false, true));
    }

    bool Open (const string& filepath) {
        if (generated_code_.is_open())
            generated_code_.close();
        generated_code_.open(filepath.c_str(), ios_base::in);
        return generated_code_.good();
    }

    bool GenerateCodeMatches (const string& source_file, const string& expected_code) {
        if (!Open(OUROBOROS_TEST_DUMP_DIR "/" + source_file))
            return false;
        stringstream ss(expected_code);
        while (!ss.eof()) {
            string check, line;
            getline(ss, check);
            getline(generated_code_, line);
            EXPECT_EQ(check, line);
        }
        return true;
    }

};

const string ProxyGeneratorTest::COMMENT = " Comment: ";

TEST_F (ProxyGeneratorTest, NoClassesAtAll) {
    EXPECT_EQ(0u, Generate()) << COMMENT << "Should not have detected any inheritable classes.";
}

TEST_F (ProxyGeneratorTest, SingleEmptyNonVirtualClass) {
    AddEmptyNonVirtualClass();
    EXPECT_EQ(0u, Generate()) << COMMENT << "Should not have detected any inheritable classes.";
}

TEST_F (ProxyGeneratorTest, SingleNonEmptyNonVirtualClass) {
    AddNonEmptyNonVirtualClass("NonInheritableClass");
    EXPECT_EQ(0u, Generate()) << COMMENT << "Should not have detected any inheritable classes.";
}

TEST_F (ProxyGeneratorTest, SingleEmptyVirtualClass) {
    AddEmptyVirtualClass("VirtualClass");
    EXPECT_EQ(1u, Generate()) << COMMENT << "Should have found exactly one inheritable class.";
    string expected_code =
        "#ifndef OPWIG_GENERATED_VirtualClass_H_\n"
        "#define OPWIG_GENERATED_VirtualClass_H_\n"
        "namespace generated {\n"
        "class VirtualClass_Proxy : public VirtualClass {};\n"
        "} // namespace\n"
        "#endif\n";
    EXPECT_TRUE(GenerateCodeMatches("VirtualClass_proxy.h", expected_code))
        << COMMENT << "Generated proxy source should have matched the expected code.";
}

} // namespace testing
} // namespace gen
} // namespace opwig

