

TEST (NestedNameTest, ConstructorGetSetName) {
    Declarator declarator("name");
    ASSERT_EQ("name", declarator.name());
    declarator.set_name("another_name");
    ASSERT_EQ("another_name", declarator.name());
}
