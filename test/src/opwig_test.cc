
#include <opwig/md/variable.h>
#include <opwig/md/function.h>
#include <opwig/md/namespace.h>
#include <opwig/md/class.h>
#include <opwig/parser/mdparser.h>
#include <opwig/parser/declarator.h>

#include <gtest/gtest.h>

#include <vector>

#include <string>
#include <istream>
#include <sstream>
#include <list>
#include <algorithm>
#include <exception>

using std::vector;
using std::string;
using std::istringstream;
using std::list;

using opwig::md::Ptr;
using opwig::md::Variable;
using opwig::md::ParameterList;
using opwig::md::Function;
using opwig::md::Namespace;
using opwig::md::Class;
using opwig::parser::BaseSpecifier;
using opwig::parser::AccessSpecifier;
using opwig::MDParser;
using opwig::parser::Declarator;

//#define EXPECT_THROW(code, exception) \
//  try { code; EXPECT_TRUE(false); } catch (exception e) {}

TEST (DeclaratorTest, ConstructorGetSetName) {
    Declarator declarator("name");
    ASSERT_EQ("name", declarator.name());
    declarator.set_name("another_name");
    ASSERT_EQ("another_name", declarator.name());
}

TEST (DeclaratorTest, ParametersInformation) {
    using std::equal;
    Declarator declarator("name");
    EXPECT_FALSE(declarator.has_parameters());
    /* Empty parameter list */ {
        ParameterList parameters = {};
        declarator.set_parameters(parameters);
        EXPECT_TRUE(declarator.has_parameters());
        const ParameterList parameters_check = declarator.parameters();
        EXPECT_EQ(parameters.size(), parameters_check.size());
        EXPECT_TRUE(equal(parameters_check.begin(), parameters_check.end(), parameters.begin()));
    }
    /* Single parameter list */ {
        ParameterList parameters = {{"type1", "name1"}};
        declarator.set_parameters(parameters);
        EXPECT_TRUE(declarator.has_parameters());
        const ParameterList parameters_check = declarator.parameters();
        EXPECT_EQ(parameters.size(), parameters_check.size());
        EXPECT_TRUE(equal(parameters_check.begin(), parameters_check.end(), parameters.begin()));
    }
    /* Multiple parameter list */ {
        ParameterList parameters = {{"type1", "name1"}, {"type2", "name2"}, {"type3", "name3"}};
        declarator.set_parameters(parameters);
        EXPECT_TRUE(declarator.has_parameters());
        const ParameterList parameters_check = declarator.parameters();
        EXPECT_EQ(parameters.size(), parameters_check.size());
        EXPECT_TRUE(equal(parameters_check.begin(), parameters_check.end(), parameters.begin()));
    }
}

TEST (NamespaceTest, Create) {
    Ptr<Namespace> mdnamespace = Namespace::Create();
    ASSERT_TRUE(static_cast<bool>(mdnamespace));
    EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 0u);
}  

TEST (NamespaceTest, NestSingle) {
    Ptr<Namespace>  mdnamespace = Namespace::Create(),
                    nested = Namespace::Create();
    ASSERT_TRUE(static_cast<bool>(mdnamespace));
    EXPECT_TRUE(mdnamespace->AddNestedNamespace("nested", nested));
    EXPECT_EQ(mdnamespace->NestedNamespacesNum(), 1u);
    EXPECT_FALSE(mdnamespace->AddNestedNamespace("nested", nested));
    EXPECT_FALSE(mdnamespace->AddNestedNamespace("nested", Namespace::Create()));
    EXPECT_EQ(mdnamespace->NestedNamespace("nested"), nested);
    EXPECT_NE(mdnamespace->NestedNamespace("macaco"), nested);
}

TEST (NamespaceTest, AddSingleFunction) {
  Ptr<Namespace>  space = Namespace::Create();
  Ptr<Function>   foo   = Function::Create("foo", "void", {});
  EXPECT_TRUE(space->AddNestedFunction(foo));
  EXPECT_EQ(space->NestedFunctionsNum(), 1u);
  EXPECT_EQ(space->NestedFunction("foo"), foo);
  EXPECT_FALSE(static_cast<bool>(space->NestedFunction("bar")));
}

TEST (NamespaceTest, AddManyFunctions) {
  Ptr<Namespace>  space = Namespace::Create();
  Ptr<Function>   foo1  = Function::Create("foo1", "void", {});
  Ptr<Function>   foo2  = Function::Create("foo1", "void", {});
  Ptr<Function>   foo3  = Function::Create("foo2", "void", {});
  EXPECT_TRUE(space->AddNestedFunction(foo1));
  EXPECT_FALSE(space->AddNestedFunction(foo2));
  EXPECT_TRUE(space->AddNestedFunction(foo3));
  EXPECT_EQ(space->NestedFunctionsNum(), 2u);
  EXPECT_EQ(space->NestedFunction("foo1"), foo1);
  EXPECT_EQ(space->NestedFunction("foo2"), foo3);
  EXPECT_FALSE(static_cast<bool>(space->NestedFunction("foo3")));
}

TEST (VariableTest, Create) {
  Ptr<Variable> var = Variable::Create("varname", "vartype");
  ASSERT_TRUE(static_cast<bool>(var));
  EXPECT_EQ(var->name(), "varname");
  EXPECT_EQ(var->type(), "vartype");
}

TEST (FunctionTest, Create) {
  Ptr<Function> var = Function::Create("funcname", "returntype", {{"type0", "name0"}, {"type1", "name1"}});
  ASSERT_TRUE(static_cast<bool>(var));
  EXPECT_EQ(var->name(), "funcname");
  EXPECT_EQ(var->return_type(), "returntype");
  EXPECT_EQ(var->parameter_type(0), "type0");
  EXPECT_EQ(var->parameter_name(0), "name0");
  EXPECT_EQ(var->parameter_type(1), "type1");
  EXPECT_EQ(var->parameter_name(1), "name1");
  EXPECT_THROW(var->parameter_type(2), std::out_of_range);
}

TEST (MDParserTest, EmptyFile) {
  istringstream input("");
  MDParser parser(input);
  EXPECT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 0u);
}

namespace {
  string test00 =
    "namespace abc {}";
  string test01 =
    "namespace abc {}"
    "namespace def {}";
  string test02 =
    "namespace abc {"
    "  namespace def {}"
    "}";
}

TEST (MDParserTest, SingleEmptyNamespace) {
  istringstream input(test00);
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 1u);
  Ptr<const Namespace> nested = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(nested));
  EXPECT_EQ(nested->NestedNamespacesNum(), 0u);
}

TEST (MDParserTest, TwoEmptyNamespaces) {
  istringstream input(test01);
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 2u);
  Ptr<const Namespace> nested1 = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(nested1));
  EXPECT_EQ(nested1->NestedNamespacesNum(), 0u);
  Ptr<const Namespace> nested2 = global->NestedNamespace("def");
  EXPECT_TRUE(static_cast<bool>(nested2));
  EXPECT_EQ(nested2->NestedNamespacesNum(), 0u);
}

TEST (MDParserTest, SingleNestedNamespace) {
  istringstream input(test02);
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  EXPECT_TRUE(static_cast<bool>(global));
  EXPECT_EQ(global->NestedNamespacesNum(), 1u);
  Ptr<const Namespace> outer = global->NestedNamespace("abc");
  EXPECT_TRUE(static_cast<bool>(outer));
  EXPECT_EQ(outer->NestedNamespacesNum(), 1u);
  Ptr<const Namespace> inner = outer->NestedNamespace("def");
  EXPECT_TRUE(static_cast<bool>(inner));
  EXPECT_EQ(inner->NestedNamespacesNum(), 0u);
}

namespace {
    string test11 = 
        "type var = 0;";
    string test12 = 
        "type1 var1 = \"test\";"
        "type2 var2 = 2;";
    string test13 =
        "type var = 0;"
        "type2 var;";
    string test14 =
        "type var1 = 1, var2 = 2, var3 = 3;";
    string test15 =
        "namespace abc {"
        "type var;"
        "}";
}

TEST (MDParserTest, SingleVariable) {
    istringstream input(test11);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));

    Ptr<const Variable> var = global->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
}

TEST (MDParserTest, MultiVariable) {
    istringstream input(test12);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Variable> var1 = global->GlobalVariable("var1");
    EXPECT_TRUE(static_cast<bool>(var1));
    EXPECT_EQ(var1->name(), "var1");
    EXPECT_EQ(var1->type(), "type1");
    Ptr<const Variable> var2 = global->GlobalVariable("var2");
    EXPECT_TRUE(static_cast<bool>(var2));
    EXPECT_EQ(var2->name(), "var2");
    EXPECT_EQ(var2->type(), "type2");
}

TEST (MDParserTest, VariableConflict) {
    istringstream input(test13);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));

    Ptr<const Variable> var = global->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
}

TEST (MDParserTest, VariableSequence) {
    istringstream input(test14);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Variable> var1 = global->GlobalVariable("var1");
    EXPECT_TRUE(static_cast<bool>(var1));
    EXPECT_EQ(var1->name(), "var1");
    EXPECT_EQ(var1->type(), "type");
    Ptr<const Variable> var2 = global->GlobalVariable("var2");
    EXPECT_TRUE(static_cast<bool>(var2));
    EXPECT_EQ(var2->name(), "var2");
    EXPECT_EQ(var2->type(), "type");
    Ptr<const Variable> var3 = global->GlobalVariable("var3");
    EXPECT_TRUE(static_cast<bool>(var3));
    EXPECT_EQ(var3->name(), "var3");
    EXPECT_EQ(var3->type(), "type");
}

TEST (MDParserTest, VariableInNamespace) {
    istringstream input(test15);
    MDParser parser(input);
    ASSERT_EQ(parser.parse(), 0);
    Ptr<const Namespace> global = parser.global_namespace();
    EXPECT_TRUE(static_cast<bool>(global));
    Ptr<const Namespace> abc = global->NestedNamespace("abc");
    EXPECT_TRUE(static_cast<bool>(abc));
    Ptr<const Variable> var = abc->GlobalVariable("var");
    EXPECT_TRUE(static_cast<bool>(var));
    EXPECT_EQ(var->name(), "var");
    EXPECT_EQ(var->type(), "type");
}

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

namespace {
    string test21 = 
        "rtype name();";
    string test22 = 
        "rtype name(type);";
    string test23 =
        "rtype name(type0, type1);";
    string test24 =
        "rtype name(type0, type1, type2);";
    string test25 =
        "rtype0 name0(ptype0, ptype1);"
        "rtype1 name1(ptype0 pname0, ptype1);"
        "rtype2 name2(ptype0 pname0, ptype1 pname1);"
        "rtype3 name3(ptype0 **pname0[]);"
        "namespace abc {"
        "  rtype name0(ptype0, ptype1 pnameX);"
        "}";
}

TEST (MDParsetTest, GlobalNoArgFunction) {
  istringstream input("rtype name();");
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  ASSERT_TRUE(static_cast<bool>(global));
  ASSERT_EQ(1u, global->NestedFunctionsNum());
  Ptr<const Function> func = global->NestedFunction("name");
  ASSERT_TRUE(static_cast<bool>(func));
  EXPECT_EQ(func->name(), "name");
  EXPECT_EQ(func->return_type(), "rtype");
  EXPECT_THROW(func->parameter_type(0), std::out_of_range);
  EXPECT_THROW(func->parameter_name(0), std::out_of_range);
}

TEST (MDParsetTest, GlobalSingleArgFunction) {
  istringstream input("rtype name(type);");
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  ASSERT_TRUE(static_cast<bool>(global));
  ASSERT_EQ(1u, global->NestedFunctionsNum());
  Ptr<const Function> func = global->NestedFunction("name");
  ASSERT_TRUE(static_cast<bool>(func));
  EXPECT_EQ(func->name(), "name");
  EXPECT_EQ(func->return_type(), "rtype");
  EXPECT_EQ("type", func->parameter_type(0));
  EXPECT_EQ("", func->parameter_name(0));
}

TEST (MDParsetTest, GlobalDoubleArgFunction) {
  istringstream input("rtype name(type0, type1);");
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  ASSERT_TRUE(static_cast<bool>(global));
  ASSERT_EQ(1u, global->NestedFunctionsNum());
  Ptr<const Function> func = global->NestedFunction("name");
  ASSERT_TRUE(static_cast<bool>(func));
  EXPECT_EQ(func->name(), "name");
  EXPECT_EQ(func->return_type(), "rtype");
  EXPECT_EQ("type0", func->parameter_type(0));
  EXPECT_EQ("", func->parameter_name(0));
  EXPECT_EQ("type1", func->parameter_type(1));
  EXPECT_EQ("", func->parameter_name(1));
}

TEST (MDParsetTest, GlobalMultipleArgFunction) {
  istringstream input("rtype name(type0, type1, type2);");
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  ASSERT_TRUE(static_cast<bool>(global));
  ASSERT_EQ(1u, global->NestedFunctionsNum());
  Ptr<const Function> func = global->NestedFunction("name");
  ASSERT_TRUE(static_cast<bool>(func));
  EXPECT_EQ(func->name(), "name");
  EXPECT_EQ(func->return_type(), "rtype");
  EXPECT_EQ("type0", func->parameter_type(0));
  EXPECT_EQ("", func->parameter_name(0));
  EXPECT_EQ("type1", func->parameter_type(1));
  EXPECT_EQ("", func->parameter_name(1));
  EXPECT_EQ("type2", func->parameter_type(2));
  EXPECT_EQ("", func->parameter_name(2));
}

TEST (MDParsetTest, ManyDifferentFunctions) {
  istringstream input(
      "rtype0 name0(ptype0, ptype1);"
      "rtype1 name1(ptype0 pname0, ptype1);"
      "rtype2 name2(ptype0 pname0, ptype1 pname1);"
      "rtype3 name3(ptype0 **pname0[]);"
      "namespace abc {"
      "  rtype2 name0(ptype0, ptype1 pnameX);"
      "}"
  );
  MDParser parser(input);
  ASSERT_EQ(parser.parse(), 0);
  Ptr<const Namespace> global = parser.global_namespace();
  ASSERT_TRUE(static_cast<bool>(global));
  ASSERT_EQ(4u, global->NestedFunctionsNum());
  /* first */ {
      Ptr<const Function> func = global->NestedFunction("name0");
      ASSERT_TRUE(static_cast<bool>(func));
      EXPECT_EQ(func->name(), "name0");
      EXPECT_EQ(func->return_type(), "rtype0");
      EXPECT_EQ("ptype0", func->parameter_type(0));
      EXPECT_EQ("", func->parameter_name(0));
      EXPECT_EQ("ptype1", func->parameter_type(1));
      EXPECT_EQ("", func->parameter_name(1));
  }
  /* second */ {
      Ptr<const Function> func = global->NestedFunction("name1");
      ASSERT_TRUE(static_cast<bool>(func));
      EXPECT_EQ(func->name(), "name1");
      EXPECT_EQ(func->return_type(), "rtype1");
      EXPECT_EQ("ptype0", func->parameter_type(0));
      EXPECT_EQ("pname0", func->parameter_name(0));
      EXPECT_EQ("ptype1", func->parameter_type(1));
      EXPECT_EQ("", func->parameter_name(1));
  }
  /* third */ {
      Ptr<const Function> func = global->NestedFunction("name2");
      ASSERT_TRUE(static_cast<bool>(func));
      EXPECT_EQ(func->name(), "name2");
      EXPECT_EQ(func->return_type(), "rtype2");
      EXPECT_EQ("ptype0", func->parameter_type(0));
      EXPECT_EQ("pname0", func->parameter_name(0));
      EXPECT_EQ("ptype1", func->parameter_type(1));
      EXPECT_EQ("pname1", func->parameter_name(1));
  }
  /* forth */ {
      Ptr<const Function> func = global->NestedFunction("name3");
      ASSERT_TRUE(static_cast<bool>(func));
      EXPECT_EQ(func->name(), "name3");
      EXPECT_EQ(func->return_type(), "rtype3");
      EXPECT_EQ("ptype0", func->parameter_type(0));
      EXPECT_EQ("pname0", func->parameter_name(0));
  }
  /* fifth */ {
      Ptr<const Namespace> abc = global->NestedNamespace("abc");
      ASSERT_TRUE(static_cast<bool>(abc));
      Ptr<const Function> func = abc->NestedFunction("name0");
      ASSERT_TRUE(static_cast<bool>(func));
      EXPECT_EQ(func->name(), "name0");
      EXPECT_EQ(func->return_type(), "rtype2");
      EXPECT_EQ("ptype0", func->parameter_type(0));
      EXPECT_EQ("ptype1", func->parameter_type(1));
      EXPECT_EQ("pnameX", func->parameter_name(1));
  }
}

