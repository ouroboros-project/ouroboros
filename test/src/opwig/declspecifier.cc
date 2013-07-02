
class DeclSpecifierTest : public ::testing::Test {

  protected:

    DeclSpecifier nonvirtualtyped_, virtualtyped_, virtualnontyped_;

    DeclSpecifierTest () :
        nonvirtualtyped_(false, NestedNameSpecifier("type")),
        virtualtyped_(true, NestedNameSpecifier("type")),
        virtualnontyped_(true) {}

};

TEST_F (DeclSpecifierTest, Constructor) {
    ASSERT_EQ("type", nonvirtualtyped_.type().ToString());
    ASSERT_FALSE(nonvirtualtyped_.is_virtual());
    ASSERT_EQ("type", virtualtyped_.type().ToString());
    ASSERT_TRUE(virtualtyped_.is_virtual());
    ASSERT_EQ("", virtualnontyped_.type().ToString());
    ASSERT_TRUE(virtualnontyped_.is_virtual());
}

TEST_F (DeclSpecifierTest, JoinTypeAndVirtualSpecs) {
    
}

