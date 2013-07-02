
class DeclSpecifierTest : public ::testing::Test {

  protected:

    DeclSpecifier nonvirtualspec_, virtualspec_;

    DeclSpecifierTest () :
        nonvirtualspec_(NestedNameSpecifier("type"), false),
        virtualspec_(NestedNameSpecifier("type"), true) {}

    static void TestConstructor (function<DeclSpecifier ()> creator) {
      DeclSpecifier declspecifier = creator();
      ASSERT_EQ("type", declspecifier.type().ToString()); // FIXME
      ASSERT_FALSE(declspecifier.is_virtual());
    }

};

TEST_F (DeclSpecifierTest, Constructor) {
    ASSERT_EQ("type", nonvirtualspec_.type().ToString());
    ASSERT_FALSE(nonvirtualspec_.is_virtual());
    ASSERT_EQ("type", virtualspec_.type().ToString());
    ASSERT_TRUE(virtualspec_.is_virtual());
}

