

TEST (NestedNameTest, NameAndGlobalEmptyPath) {
    NestedNameSpecifier nestedName("name", true);
    
    ASSERT_EQ("name", nestedName.name());
    ASSERT_EQ("::name", nestedName.ToString());
    nestedName.set_name("another_name");
    ASSERT_EQ("another_name", nestedName.name());
    ASSERT_EQ("::another_name", nestedName.ToString());
    
    ASSERT_EQ(true, nestedName.is_global());
    nestedName.set_global(false);
    ASSERT_EQ(false, nestedName.is_global());
    ASSERT_EQ("another_name", nestedName.ToString());
}

TEST (NestedNameTest, NonEmptyPath) {
    NestedNameSpecifier nestedName("name");
    nestedName.AddPath("foo");
    nestedName.AddPath("bar");
    
    ASSERT_EQ("foo::bar::name", nestedName.ToString());
    
    nestedName.set_global(true);
    ASSERT_EQ("::foo::bar::name", nestedName.ToString());
    
    nestedName.AddPath("wat");
    ASSERT_EQ("::foo::bar::wat::name", nestedName.ToString());
}
