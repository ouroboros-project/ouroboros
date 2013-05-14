
#include <fstream>

using std::ios_base;
using std::ifstream;

class ProxyGeneratorTest : public ::testing::Test {

  protected:

    ProxyGenerator  generator_;
    Ptr<Scope>      given_scope_;
    ifstream        generated_code_;

    ProxyGeneratorTest () : 
        generator_(OUROBOROS_TEST_DUMP_DIR),
        given_scope_(Namespace::Create("")) {}

    virtual void SetUp () {}

    size_t Generate () {
        return generator_.Generate(given_scope_);
    }

    void AddNonVirtualClass () {
        string class_name = "NonVirtualClass";
        given_scope_->AddNestedClass(class_name, Class::Create(class_name, {}));
    }

    void AddVirtualClass (const string& class_name) {
        Ptr<Class> the_class = Class::Create(class_name, {});
        given_scope_->AddNestedClass(class_name, the_class);
        the_class->AddNestedFunction(Function::Create("~"+class_name, "", {}, false));
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
        return false;
    }

};

TEST_F (ProxyGeneratorTest, NoClassesAtAll) {
    EXPECT_EQ(0u, Generate()) << "There is no class.";
}

TEST_F (ProxyGeneratorTest, SingleNonVirtualClass) {
    AddNonVirtualClass();
    EXPECT_EQ(0u, Generate()) << "That class does not have a virtual destructor.";
}

TEST_F (ProxyGeneratorTest, SingleVirtualClass) {
    AddVirtualClass("VirtualClass");
    EXPECT_EQ(1u, Generate()) << "Did not found class with virtual destructor, or found too many";
    string expected_code =
        "#ifndef OUROBOROS_GENERATED_VirtualClass_H_\n"
        "#define OUROBOROS_GENERATED_VirtualClass_H_\n"
        "namespace generated {\n"
        "class VirtualClass_Proxy : public VirtualClass {};\n"
        "} // namespace generated\n"
        "#endif\n";
    EXPECT_TRUE(GenerateCodeMatches("virtualclass_proxy.h", expected_code))
        << "Generated proxy source did not match the expected code.";
}

