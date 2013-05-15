class MDClassTest : public MDBaseTest {
protected:
};

TEST_F (MDClassTest, CreateBaseSpecifier) {
    BaseSpecifier bspec ("name", true, AccessSpecifier::PUBLIC);
    EXPECT_EQ(bspec.name(), "name");
    EXPECT_TRUE(bspec.is_virtual());
    EXPECT_EQ(bspec.access_specifier(), AccessSpecifier::PUBLIC);
}

TEST_F (MDClassTest, CreateClass) {
    list<BaseSpecifier> bspecs;
    bspecs.push_back( BaseSpecifier ("name", true, AccessSpecifier::PUBLIC) );
    Ptr<Class> c = Class::Create("cname", bspecs);
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "cname");
    EXPECT_EQ(c->base_specifiers().front().name(), "name");
    EXPECT_TRUE(c->base_specifiers().front().is_virtual());
    EXPECT_EQ(c->base_specifiers().front().access_specifier(), AccessSpecifier::PUBLIC);   
}

TEST_F (MDClassTest, NamedClass) {
    ASSERT_EQ(RunParse("class name {};"), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c, 0u);
}

TEST_F (MDClassTest, DerivedNamedClass) {
    ASSERT_EQ(RunParse("class name : protected base {};"), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c, 1u);
    TestClassBaseByIndex(c, 0, "base", false, AccessSpecifier::PROTECTED);
}

TEST_F (MDClassTest, DerivedVirtualNamedClass) {
    ASSERT_EQ(RunParse("class name : protected virtual base {};"), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c, 1u);
    TestClassBaseByIndex(c, 0, "base", true, AccessSpecifier::PROTECTED);
}

TEST_F (MDClassTest, DerivedVirtualNamedClass2) {
    ASSERT_EQ(RunParse("class name : virtual base {};"), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c, 1u);
    TestClassBaseByIndex(c, 0, "base", true, AccessSpecifier::PRIVATE);
}

TEST_F (MDClassTest, MultipleDerivedNamedClass) {
    ASSERT_EQ(RunParse("class name : virtual base1, public base2, protected virtual base3, virtual public base4 {};"), 0);
    
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c, 4u);

    TestClassBaseByIndex(c, 0, "base1", true, AccessSpecifier::PRIVATE);
    TestClassBaseByIndex(c, 1, "base2", false, AccessSpecifier::PUBLIC);
    TestClassBaseByIndex(c, 2, "base3", true, AccessSpecifier::PROTECTED);
    TestClassBaseByIndex(c, 3, "base4", true, AccessSpecifier::PUBLIC);
}
 
TEST_F (MDClassTest, ClassInNamespace) {
    ASSERT_EQ(RunParse("namespace abc { class name {}; }"), 0);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 1u, 0u);
    
    auto c = TestClass(abc, "name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c, 0u);
}
 
TEST_F (MDClassTest, ClassInAndOutOfNamespace) {
    ASSERT_EQ(RunParse("class name {}; namespace abc { class name {}; }"), 0);
 
    auto c = TestClass("name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c, 0u);
    
    auto abc = TestNamespace("abc", AccessSpecifier::PUBLIC, 0u, 0u, 1u, 0u);
    auto c2 = TestClass(abc, "name", AccessSpecifier::PUBLIC, 0u, 0u, 0u);
    TestClassBaseNum(c2, 0u);
}

/*
parseando:
-class { }
-class : Base {}
-class : public Base {}
-class : virtual base1, public base2 {}
-class {} class {}
-namespace { class {} }
*/
