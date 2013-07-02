
class DeclSpecifierTest : public ::testing::Test {

  protected:

    static DeclSpecifier CreateNonVirtual () {
        return DeclSpecifier(NestedNameSpecifier("type"), false);
    }

    static DeclSpecifier CreateVirtual () {
        return DeclSpecifier(NestedNameSpecifier("type"), true);
    }

    static void TestConstructor (function<DeclSpecifier ()> creator) {
      DeclSpecifier declspecifier = creator();
      ASSERT_EQ("type", declspecifier.type().name()); // FIXME
      //ASSERT_FALSE(declspecifier.is_virtual());
    }
};

TEST_F (DeclSpecifierTest, Constructor) {
    TestConstructor(&DeclSpecifierTest::CreateNonVirtual);
}

