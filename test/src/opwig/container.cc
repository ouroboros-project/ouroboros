
namespace opwig {
namespace md {
namespace testing {

class MDMock {
  public:
    MDMock (int x) :
        x_(x) {}
    void set_access (AccessSpecifier the_access) {}
    bool operator == (const MDMock& rhs) const { return x_ == rhs.x_; }
  private:
    int x_;
};

class ContainerTest : public ::testing::Test {

  protected:

    Container<MDMock> container0_, container1_, container2_;

    ContainerTest () {}

    void SetUp () {
        container1_.Add("a",    Ptr<MDMock>(new MDMock(1)));
        container2_.Add("b",    Ptr<MDMock>(new MDMock(2)));
        container2_.Add("c",  Ptr<MDMock>(new MDMock(3)));
    }

};

TEST_F (ContainerTest, IterateEmpty) {
    for (auto entry : container0_.Iterate())
        EXPECT_TRUE(false);
}

TEST_F (ContainerTest, IterateOne) {
    for (auto entry : container1_.Iterate()) {
        EXPECT_EQ("a", entry.first);
        EXPECT_EQ(MDMock(1), *entry.second);
    }
}

TEST_F (ContainerTest, IterateTwo) {
    int i = 0;
    for (auto entry : container2_.Iterate()) {
        if (i==0) {
            EXPECT_EQ("b", entry.first);
            EXPECT_EQ(MDMock(2), *entry.second);
        } else {
            EXPECT_EQ("c", entry.first);
            EXPECT_EQ(MDMock(3), *entry.second);
        }
        ++i;
    }
}

} // namespace testing
} // namespace md
} // namespace opwig

