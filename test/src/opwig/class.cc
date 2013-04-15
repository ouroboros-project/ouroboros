
TEST (BaseSpecifierTest, Create) {
    BaseSpecifier bspec ("name", true, AccessSpecifier::PUBLIC);
    EXPECT_EQ(bspec.name(), "name");
    EXPECT_TRUE(bspec.is_virtual());
    EXPECT_EQ(bspec.access_specifier(), AccessSpecifier::PUBLIC);
}

TEST (ClassTest, Create) {
    list<BaseSpecifier> bspecs;
    bspecs.push_back( BaseSpecifier ("name", true, AccessSpecifier::PUBLIC) );
    Ptr<Class> c = Class::Create("cname", bspecs);
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "cname");
    EXPECT_EQ(c->base_specifiers().front().name(), "name");
    EXPECT_TRUE(c->base_specifiers().front().is_virtual());
    EXPECT_EQ(c->base_specifiers().front().access_specifier(), AccessSpecifier::PUBLIC);   
}

TEST (MDParserTest, NamedClass) {
    istringstream input("class name {};");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
}

TEST (MDParserTest, DerivedNamedClass) {
    istringstream input("class name : protected base {};");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 1);
    
    EXPECT_EQ(c->base_specifiers().front().name(), "base");
    EXPECT_FALSE(c->base_specifiers().front().is_virtual());
    EXPECT_EQ(c->base_specifiers().front().access_specifier(), AccessSpecifier::PROTECTED);   
}

TEST (MDParserTest, DerivedVirtualNamedClass) {
    istringstream input("class name : protected virtual base {};");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 1);
    
    EXPECT_EQ(c->base_specifiers().front().name(), "base");
    EXPECT_TRUE(c->base_specifiers().front().is_virtual());
    EXPECT_EQ(c->base_specifiers().front().access_specifier(), AccessSpecifier::PROTECTED);   
}

TEST (MDParserTest, DerivedVirtualNamedClass2) {
    istringstream input("class name : virtual base {};");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 1);
    
    EXPECT_EQ(c->base_specifiers().front().name(), "base");
    EXPECT_TRUE(c->base_specifiers().front().is_virtual());
    EXPECT_EQ(c->base_specifiers().front().access_specifier(), AccessSpecifier::PRIVATE);   
}

TEST (MDParserTest, MultipleDerivedNamedClass) {
    istringstream input("class name : virtual base1, public base2, protected virtual base3, virtual public base4 {};");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 4);
    
    auto bspec = c->base_specifiers().begin();
    
    EXPECT_EQ(bspec->name(), "base1");
    EXPECT_TRUE(bspec->is_virtual());
    EXPECT_EQ(bspec->access_specifier(), AccessSpecifier::PRIVATE);   
    bspec++;
    EXPECT_EQ(bspec->name(), "base2");
    EXPECT_FALSE(bspec->is_virtual());
    EXPECT_EQ(bspec->access_specifier(), AccessSpecifier::PUBLIC);   
    bspec++;
    EXPECT_EQ(bspec->name(), "base3");
    EXPECT_TRUE(bspec->is_virtual());
    EXPECT_EQ(bspec->access_specifier(), AccessSpecifier::PROTECTED);   
    bspec++;
    EXPECT_EQ(bspec->name(), "base4");
    EXPECT_TRUE(bspec->is_virtual());
    EXPECT_EQ(bspec->access_specifier(), AccessSpecifier::PUBLIC);   
    bspec++;
}
 
TEST (MDParserTest, ClassInNamespace) {
    istringstream input("namespace abc { class name {}; }");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Namespace> abc = global->NestedNamespace("abc");
    EXPECT_TRUE(static_cast<bool>(abc));
    Ptr<const Class> c = abc->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
}
 
TEST (MDParserTest, ClassInAndOutOfNamespace) {
    istringstream input("class name {}; namespace abc { class name {}; }");
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
 
    Ptr<const Class> c = global->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c));
    EXPECT_EQ(c->name(), "name");
    EXPECT_EQ(c->base_specifiers().size(), 0);
    
    Ptr<const Namespace> abc = global->NestedNamespace("abc");
    EXPECT_TRUE(static_cast<bool>(abc));
    Ptr<const Class> c2 = abc->NestedClass("name");
    EXPECT_TRUE(static_cast<bool>(c2));
    EXPECT_EQ(c2->name(), "name");
    EXPECT_EQ(c2->base_specifiers().size(), 0);
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
