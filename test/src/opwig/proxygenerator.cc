
class ProxyGeneratorTest : public ::testing::Test {

  protected:

    Ptr<Scope>      given_scope_;
    ProxyGenerator  generator_;

    ProxyGeneratorTest() 
        : given_scope_(Namespace::Create("")) {}

    virtual void SetUp() {}

    void AddNonVirtualClass() {
        string class_name = "NonVirtualClass";
        given_scope_->AddNestedClass(class_name, Class::Create(class_name, {}));
    }

    void AddVirtualClass() {

    }

};

TEST_F(ProxyGeneratorTest, NoClassesAtAll) {
    generator_.Generate(given_scope_);
}

