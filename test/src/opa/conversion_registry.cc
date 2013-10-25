
#include <opa/type.h>
#include <gtest/gtest.h>

namespace opa {
namespace testing {

namespace {

class A {};
class B : public A {};
class C : public A {};
class D : public B {};
class E {};
class F : public C, public E {};

} // unnamed namespace

class ConversionRegistryTest : public ::testing::Test {

  protected:

    HeritageTable table_;

    virtual void SetUp () {
        ClearConversions();
    }

    void BuildSmallTable () {
        table_ = {
          { typeid(B), { typeid(A) } }
        };
    }

    void BuildBigTable () {
        table_ = {
          { typeid(B), { typeid(A) } },
          { typeid(C), { typeid(A) } },
          { typeid(D), { typeid(B), typeid(A) } },
          { typeid(F), { typeid(C), typeid(E), typeid(A) } }
        };
    }
    
    template <typename T, typename S>
    void CheckTwoWayConversion () {
        EXPECT_EQ(Convertibility::INHERITS, opa::CheckConversion(typeid(S), typeid(T)));
        EXPECT_EQ(Convertibility::IS_BASE, opa::CheckConversion(typeid(T), typeid(S)));
    }
    
    template <typename T, typename S>
    void CheckTwoWayNonConversion () {
        EXPECT_EQ(Convertibility::NONE, opa::CheckConversion(typeid(S), typeid(T)));
        EXPECT_EQ(Convertibility::NONE, opa::CheckConversion(typeid(T), typeid(S)));
    }

};

TEST_F (ConversionRegistryTest, DefineAndCheck) {
    DefineConversion(typeid(B), typeid(A), Convertibility::INHERITS);
    EXPECT_EQ(Convertibility::NONE, CheckConversion(typeid(B), typeid(int)));
    EXPECT_EQ(Convertibility::INHERITS, opa::CheckConversion(typeid(B), typeid(A)));
}

TEST_F (ConversionRegistryTest, CheckNonExistant) {
    EXPECT_EQ(Convertibility::NONE, CheckConversion(typeid(B), typeid(int)));
    EXPECT_EQ(Convertibility::NONE, CheckConversion(typeid(B), typeid(A)));
}

TEST_F (ConversionRegistryTest, CheckEmptyTable) {
    DefineConversion(table_);
    EXPECT_EQ(Convertibility::NONE, CheckConversion(typeid(B), typeid(int)));
    EXPECT_EQ(Convertibility::NONE, CheckConversion(typeid(B), typeid(A)));
}

TEST_F (ConversionRegistryTest, CheckSmallTable) {
    BuildSmallTable();
    DefineConversion(table_);
    EXPECT_EQ(Convertibility::NONE, CheckConversion(typeid(B), typeid(int)));
    CheckTwoWayConversion<A,B>();
    CheckTwoWayNonConversion<A,E>();
    CheckTwoWayNonConversion<F,B>();
    CheckTwoWayNonConversion<C,D>();
}

TEST_F (ConversionRegistryTest, CheckBigTable) {
    BuildBigTable();
    DefineConversion(table_);
    EXPECT_EQ(Convertibility::NONE, CheckConversion(typeid(B), typeid(int)));
    CheckTwoWayConversion<A,B>();
    CheckTwoWayConversion<A,C>();
    CheckTwoWayConversion<A,D>();
    CheckTwoWayConversion<A,F>();
    CheckTwoWayConversion<B,D>();
    CheckTwoWayConversion<C,F>();
    CheckTwoWayConversion<E,F>();
    CheckTwoWayNonConversion<A,E>();
    CheckTwoWayNonConversion<F,B>();
    CheckTwoWayNonConversion<C,D>();
}

} // namespace testing
} // namespace opa

