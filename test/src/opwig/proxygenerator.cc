
#include <fstream>

using std::ios_base;
using std::ifstream;

class ProxyGeneratorTest : public ::testing::Test {

  protected:

    ProxyGenerator  generator_;
    Ptr<Scope>      given_scope_;
    ifstream        generated_code_;

    ProxyGeneratorTest() : 
        generator_(OUROBOROS_TEST_DUMP_DIR),
        given_scope_(Namespace::Create("")) {}

    virtual void SetUp() {}

    void AddNonVirtualClass() {
        string class_name = "NonVirtualClass";
        given_scope_->AddNestedClass(class_name, Class::Create(class_name, {}));
    }

    void AddVirtualClass() {

    }

    bool Open(const string& filepath) {
        if (generated_code_.is_open())
            generated_code_.close();
        generated_code_.open(filepath.c_str(), ios_base::in);
        return generated_code_.good();
    }

};

TEST_F(ProxyGeneratorTest, NoClassesAtAll) {
    EXPECT_EQ(0u, generator_.Generate(given_scope_));
}

