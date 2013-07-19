namespace opwig {
namespace gen {
namespace testing {

using std::string;
using std::ios_base;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;
using opwig::gen::ProxyGenerator;

namespace {

struct TestCase {
    string source_file;
    string expected_code;  
};

}

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
        the_class->AddNestedFunction(Function::Create("NonVirtualMethod", "void", {}));
    }

    void AddVirtualClassWithSimpleMethod (const string& class_name) {
        Ptr<Class> the_class = Class::Create(class_name, {});
        given_scope_->AddNestedClass(the_class);
        the_class->AddNestedFunction(Function::Create("VirtualMethod", "void", {}, false, true));
    }

    void AddVirtualClassWithManySimpleMethods (const string& class_name) {
        Ptr<Class> the_class = Class::Create(class_name, {});
        given_scope_->AddNestedClass(the_class);
        the_class->AddNestedFunction(Function::Create("VirtualMethod", "void", {}, false, true));
        the_class->AddNestedFunction(Function::Create("AnotherVirtualMethod", "void", {}, false, true));
        the_class->AddNestedFunction(Function::Create("GuessWhatYetAnotherVirtualMethod", "void", {}, false, true));
    }

    bool Open (const string& filepath) {
        if (generated_code_.is_open())
            generated_code_.close();
        generated_code_.open(filepath.c_str(), ios_base::in);
        return generated_code_.good();
    }

    bool GenerateCodeMatches (const TestCase& the_case) {
        if (!Open(OUROBOROS_TEST_DUMP_DIR "/" + the_case.source_file))
            return false;
        stringstream ss(the_case.expected_code);
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

namespace {

TestCase  CLASS_WITH_SIMPLE_METHOD_CASE = {
              "VirtualClassProxy.h",
              "#ifndef OPWIG_GENERATED_VirtualClass_H_\n"
              "#define OPWIG_GENERATED_VirtualClass_H_\n"
              "namespace generated {\n"
              "class VirtualClass_Proxy : public VirtualClass {\n"
              "  public:\n"
              "    VirtualClass_Proxy (const VirtualObj& the_proxy)\n"
              "        : VirtualClass(), proxy_(the_proxy) {}\n"
              "    void VirtualMethod () {\n"
              "        (proxy_ | \"VirtualMethod\") ();\n"
              "    }\n"
              "  private:\n"
              "    VirtualObj proxy_;\n"
              "};\n"
              "} // namespace\n"
              "#endif\n"
          },
          CLASS_WITH_MANY_SIMPLE_METHODS_CASE = {
              "AnotherVirtualClassProxy.h",
              "#ifndef OPWIG_GENERATED_AnotherVirtualClass_H_\n"
              "#define OPWIG_GENERATED_AnotherVirtualClass_H_\n"
              "namespace generated {\n"
              "class AnotherVirtualClass_Proxy : public AnotherVirtualClass {\n"
              "  public:\n"
              "    AnotherVirtualClass_Proxy (const VirtualObj& the_proxy)\n"
              "        : AnotherVirtualClass(), proxy_(the_proxy) {}\n"
              "    void AnotherVirtualMethod () {\n"
              "        (proxy_ | \"AnotherVirtualMethod\") ();\n"
              "    }\n"
              "    void GuessWhatYetAnotherVirtualMethod () {\n"
              "        (proxy_ | \"GuessWhatYetAnotherVirtualMethod\") ();\n"
              "    }\n"
              "    void VirtualMethod () {\n"
              "        (proxy_ | \"VirtualMethod\") ();\n"
              "    }\n"
              "  private:\n"
              "    VirtualObj proxy_;\n"
              "};\n"
              "} // namespace\n"
              "#endif\n"
          };

}

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

TEST_F (ProxyGeneratorTest, SingleVirtualClassWithSimpleMethod) {
    AddVirtualClassWithSimpleMethod("VirtualClass");
    EXPECT_EQ(1u, Generate()) << COMMENT << "Should have found exactly one inheritable class.";
    EXPECT_TRUE(GenerateCodeMatches(CLASS_WITH_SIMPLE_METHOD_CASE))
        << COMMENT << "Generated proxy source should have matched the expected code.";
}

TEST_F (ProxyGeneratorTest, ManyVirtualClassesWithSimpleMethods) {
    AddVirtualClassWithSimpleMethod("VirtualClass");
    AddVirtualClassWithManySimpleMethods("AnotherVirtualClass");
    EXPECT_EQ(2u, Generate()) << COMMENT << "Should have found exactly two inheritable classes.";
    EXPECT_TRUE(GenerateCodeMatches(CLASS_WITH_SIMPLE_METHOD_CASE))
        << COMMENT << "Generated proxy source should have matched for " << "VirtualClass"
                   << "'s expected code.";
    EXPECT_TRUE(GenerateCodeMatches(CLASS_WITH_MANY_SIMPLE_METHODS_CASE))
        << COMMENT << "Generated proxy source should have matched for " << "AnotherVirtualClass"
                   << "'s expected code.";
}

} // namespace testing
} // namespace gen
} // namespace opwig

