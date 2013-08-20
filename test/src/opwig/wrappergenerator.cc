namespace opwig {
namespace gen {

using std::string;
using std::list;

/*******************************************************
 Dummy Wrapper definition for testing
*******************************************************/
class DummySpecification : public WrapperSpecification {
  public:
    DummySpecification() {}
    virtual ~DummySpecification() {}
    virtual string wrapper_name() const { return "Dummy"; }
    
    virtual string FileHeader() const {
        return
            "// Source generated with OPWIG for scripting language DUMMY\n"
            "// -------------------------------------------------------------";
    }
    virtual string MiddleBlock() const { return ""; }
    virtual string FinishFile() const {
        return
            "// -------------------------------------------------------------\n"
            "// finished file";
    }
    virtual string WrapFunction(const md::Ptr<const md::Function>& obj) {
        std::stringstream func;
        func << "RETURN_TYPE OPWIG_wrap_"+obj->name()+"(ARG_1_TYPE arg1) {" << std::endl;
        func << "    DummyConverter converter ();" << std::endl;
        std::stringstream args ("");
        for (unsigned i=0; i<obj->num_parameters(); i++) {
            func << "    "<< obj->parameter_type(i)->full_type() <<" fArg"<< i;
            func << " = converter.fromScript<"<< obj->parameter_type(i)->full_type() <<">(arg1);" << std::endl;
            if (i > 0)
                args << ", ";
            args << "fArg" << i;
        }
        func << "    " << obj->return_type()->full_type() << " fValue = " << obj->name() << "("<< args.str() << ");" << std::endl;
        func << "    return converter.toScript<"<< obj->return_type()->full_type() <<">(fValue);" << std::endl;
        func << "}";
        return func.str();
    }
    virtual string WrapVariable(const md::Ptr<const md::Variable>& obj) { return "VARIABLE:"+obj->name(); }
    virtual string WrapEnum(const md::Ptr<const md::Enum>& obj) { return "ENUM:"+obj->name(); }

    virtual string OpenClass(const md::Ptr<const md::Class>& obj) { return "CLASS:"+obj->name(); }
    virtual string CloseClass(const md::Ptr<const md::Class>& obj) { return "CLASS:"+obj->name(); }
    virtual string OpenNamespace(const md::Ptr<const md::Namespace>& obj) { return "NAMESPACE:"+obj->name(); }
    virtual string CloseNamespace(const md::Ptr<const md::Namespace>& obj) { return "NAMESPACE:"+obj->name(); }
    virtual string LoadFuncSignature() const { return "void(*)(void)"; }
    virtual list<ScriptModule> GetGeneratedModules() const {return {{module_name_, "init_"+module_name_}}; }
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
        generator_({}, OUROBOROS_TEST_DUMP_DIR),
        given_scope_(Namespace::Create("")) {}

    virtual ~WrapperGeneratorTest () {
        generated_code_.close();
    }

    virtual void SetUp () {}

    void Generate () {
        Ptr<DummySpecification> spec(new DummySpecification() );
        generator_.Generate("TestModule", given_scope_, spec);
    }

    bool Open (const string& filepath) {
        if (generated_code_.is_open())
            generated_code_.close();
        generated_code_.open(filepath.c_str(), ios_base::in);
        return generated_code_.good();
    }

    void GenerateCodeMatches (const WrapperTestCase& the_case) {
        if (!Open(OUROBOROS_TEST_DUMP_DIR "/" + the_case.source_file))
            return;
        stringstream ss(the_case.expected_code);
        while (!ss.eof()) {
            string check, line;
            getline(generated_code_, line);
            if (line.size() > 0 && !(line[0]=='/' && line[1]=='/') && !(line[0]=='/' && line[1]=='*')) {
                getline(ss, check);
                ASSERT_EQ(check, line) << COMMENT << "Generated wrapper source should have matched the expected code.";
            }
        }
    }

};

const string WrapperGeneratorTest::COMMENT = " Comment: ";

namespace {

WrapperTestCase  SIMPLE_GLOBAL_FUNCTIONS = {
              "Dummy_TestModule_wrap.cxx",
              "RETURN_TYPE OPWIG_wrap_DoStuff(ARG_1_TYPE arg1) {\n"
              "    DummyConverter converter ();\n"
              "    CppType0 fArg0 = converter.fromScript<CppType0>(arg1);\n"
              "    CppType1 fArg1 = converter.fromScript<CppType1>(arg1);\n"
              "    CppRetType fValue = DoStuff(fArg0, fArg1);\n"
              "    return converter.toScript<CppRetType>(fValue);\n"
              "}\n"
              "RETURN_TYPE OPWIG_wrap_Wat(ARG_1_TYPE arg1) {\n"
              "    DummyConverter converter ();\n"
              "    CppRetType fValue = Wat();\n"
              "    return converter.toScript<CppRetType>(fValue);\n"
              "}"
          };

}

TEST_F (WrapperGeneratorTest, SimpleGlobalFunctions) {
    given_scope_->AddNestedFunction(Function::Create("Wat", Type::Create("CppRetType",false), {}, false, false));
    given_scope_->AddNestedFunction(Function::Create("DoStuff", Type::Create("CppRetType",false), 
                                                    {{Type::Create("CppType0",false), "n0"}, {Type::Create("CppType1",false), "n1"}},
                                                    false, false));
    Generate();
    GenerateCodeMatches(SIMPLE_GLOBAL_FUNCTIONS);
}

} // namespace testing
} // namespace gen
} // namespace opwig

