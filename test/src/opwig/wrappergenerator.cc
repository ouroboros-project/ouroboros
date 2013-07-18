namespace opwig {
namespace gen {

/*******************************************************
 Dummy Wrapper definition for testing
*******************************************************/
class DummyProvider : public ConverterProvider {
  public:
    DummyProvider() {}
    virtual ~DummyProvider() {}

    virtual std::string GetConstructorCode() const {
        return 
            "Converter() {}";
    }
    virtual std::string GetDestructorCode() const {
        return
            "~Converter() {}";
    }
    virtual std::string GetToFunctionsCode() const {
        return
            "template <typename T>\n"
            "ScriptType toScript(T value) {\n"
            "    // Do steps required to convert C++ type T to script.\n"
            "    // You might want to write more than one toScript function,\n"
            "    // specializing the template for specific types (such as primitives).\n"
            "    return SOMETHING;\n"
            "}";
    }
    virtual std::string GetFromFunctionsCode() const {
        return
            "template <typename T>\n"
            "T fromScript(ScriptType value) {\n"
            "    // Do steps required to convert a script value to a C++ type T.\n"
            "    // You might want to write more than one fromScript function,\n"
            "    // specializing the template for specific types (such as primitives).\n"
            "    return SOMETHING;\n"
            "}";
    }
};
//////////////
class DummySpecification : public WrapperSpecification {
  public:
    DummySpecification() {}
    virtual ~DummySpecification() {}
    virtual std::string WrapperName() const { return "Dummy"; }
    
    virtual md::Ptr<ConverterProvider> GetConverterProvider() const {
        return md::Ptr<ConverterProvider>(new DummyProvider() );
    }
    virtual std::string FileHeader() const {
        return
            "// Source generated with OPWIG for scripting language DUMMY\n"
            "//\n"
            "// This text block is written FIRST in the generated files, and as such\n"
            "// you should put anything you will otherwise need here...\n"
            "// -------------------------------------------------------------\n"
            "\n"
            "#define RETURN_TYPE void //depends on script language\n"
            "#define ARG_1_TYPE int //depends on script language";
    }
    virtual std::string FinishFile() const {
        return
            "// -------------------------------------------------------------\n"
            "// This text block is written LAST in the generated files, and as such\n"
            "// you should put anything to finish the wrap code here...\n"
            "// \n"
            "// Most scripting languages (Python, Lua, etc) wrapping module code \n"
            "// has a 'init' function, defined last in the code, which inicializes \n"
            "// the module. You should write that here as well.";
    }
    virtual std::string WrapFunction(const md::Ptr<const md::Function>& obj) const {
        std::stringstream func;
        func << "RETURN_TYPE OPWIG_wrap_"+obj->name()+"(ARG_1_TYPE arg1) {" << std::endl;
        func << "    // The converter'll always be of this class, but you should initialize it" << std::endl;
        func << "    // calling the appropriate constructor according to what your ConverterProvider defines." << std::endl;
        func << "    Converter converter ();" << std::endl;
        func << "    " << std::endl;
        func << "    // How to call the converting methods depends on your implementation and parameters" << std::endl;
        func << "    // received (on this function), however it most likely will be something like this" << std::endl;
        std::stringstream args ("");
        for (unsigned i=0; i<obj->num_parameters(); i++) {
            func << "    "<< obj->parameter_type(i) <<" fArg"<< i <<" = converter.fromScript<"<< obj->parameter_type(i) <<">(arg1);" << std::endl;
            if (i > 0)
                args << ", ";
            args << "fArg" << i;
        }
        func << "    " << std::endl;
        func << "    " << obj->return_type() << " fValue = " << obj->name() << "("<< args.str() << ");" << std::endl;
        func << "    return converter.toScript<"<< obj->return_type() <<">(fValue);" << std::endl;
        func << "}" << std::endl;
        return func.str();
    }
};
 
/*********************************************************/
namespace testing {

using std::string;
using std::ios_base;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;
using opwig::gen::WrapperGenerator;
using opwig::gen::DummySpecification;

namespace {

struct WrapperTestCase {
    string source_file;
    string expected_code;  
};

}

class WrapperGeneratorTest : public ::testing::Test {
  protected:

    WrapperGenerator      generator_;
    Ptr<Scope>          given_scope_;
    ifstream            generated_code_;

    const static string COMMENT;

    WrapperGeneratorTest () : 
        generator_(OUROBOROS_TEST_DUMP_DIR),
        given_scope_(Namespace::Create("")) {}

    virtual ~WrapperGeneratorTest () {
        generated_code_.close();
    }

    virtual void SetUp () {}

    void Generate () {
        Ptr<DummySpecification> spec(new DummySpecification() );
        generator_.Generate(given_scope_, spec);
    }

    bool Open (const string& filepath) {
        if (generated_code_.is_open())
            generated_code_.close();
        generated_code_.open(filepath.c_str(), ios_base::in);
        return generated_code_.good();
    }

    bool GenerateCodeMatches (const WrapperTestCase& the_case) {
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

const string WrapperGeneratorTest::COMMENT = " Comment: ";

namespace {

WrapperTestCase  SIMPLE_GLOBAL_FUNCTIONS = {
              "Dummy_wrap.cxx",
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
          };

}

TEST_F (WrapperGeneratorTest, SimpleGlobalFunctions) {
    given_scope_->AddNestedFunction(Function::Create("Wat", "CppRetType", {}, false, false));
    given_scope_->AddNestedFunction(Function::Create("DoStuff", "CppRetType", {{"CppType0", "n0"}, {"CppType1", "n1"}}, false, false));
    Generate();
    EXPECT_TRUE(GenerateCodeMatches(SIMPLE_GLOBAL_FUNCTIONS))
        << COMMENT << "Generated wrapper source should have matched the expected code.";
}

} // namespace testing
} // namespace gen
} // namespace opwig

