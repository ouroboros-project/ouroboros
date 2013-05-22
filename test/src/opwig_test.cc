#include <opwig/md/variable.h>
#include <opwig/md/class.h>
#include <opwig/md/function.h>
#include <opwig/md/namespace.h>
#include <opwig/md/accessspecifier.h>
#include <opwig/md/semanticerror.h>
#include <opwig/md/nestednamespecifier.h>
#include <opwig/parser/mdparser.h>
#include <opwig/parser/declarator.h>

#include <gtest/gtest.h>

#include <vector>

#include <string>
#include <istream>
#include <sstream>
#include <iostream>
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
using opwig::md::Scope;
using opwig::md::Namespace;
using opwig::md::Class;
using opwig::md::AccessSpecifier;
using opwig::md::NestedNameSpecifier;
using opwig::parser::BaseSpecifier;
using opwig::parser::Declarator;
using opwig::MDParser;

class MDBaseTest : public ::testing::Test {
protected:
    Ptr<const Namespace> global_;
    
    int RunParse(const string& str) {
        istringstream input (str);
        MDParser parser(input);
        int ret = parser.parse();
        global_ = parser.global_namespace();
        EXPECT_TRUE(static_cast<bool>(global_));
        return ret;
    }
    void RunParseThrow(const string& str) {
        istringstream input (str);
        MDParser parser(input);
        EXPECT_THROW(parser.parse(), std::exception);
    }
    
    //////////// variable
    Ptr<const Variable> TestVariable(const string& name, const string& type, AccessSpecifier access) {
        return TestVariable(global_, name, type, access);
    }
    Ptr<const Variable> TestVariable(Ptr<const Scope> scope, const string& name, const string& type, AccessSpecifier access) {
        Ptr<const Variable> var = scope->GlobalVariable(name);
        EXPECT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(type, var->type());
        EXPECT_EQ(access, var->access());
        return var;
    }
    
    /////////// functions
    Ptr<const Function> TestFunction(const string& name, const string& return_type, AccessSpecifier access, bool isPure) {
        return TestFunction(global_, name, return_type, access, isPure);
    }
    Ptr<const Function> TestFunction(Ptr<const Scope> scope, const string& name, const string& return_type, AccessSpecifier access, bool isPure) {
        Ptr<const Function> func = scope->NestedFunction(name);
        internalCheckFunction(func, name, return_type, access, isPure);
        return func;
    }
    void internalCheckFunction(const Ptr<const Function>& func, const string& name, const string& return_type, AccessSpecifier access, bool isPure) {
        ASSERT_TRUE(static_cast<bool>(func));
        EXPECT_EQ(name, func->name());
        EXPECT_EQ(return_type, func->return_type());
        EXPECT_EQ(access, func->access());
        EXPECT_EQ(isPure, func->is_pure());
    }
    
    void TestFunctionNoParameters(Ptr<const Function> func) {
        EXPECT_THROW(func->parameter_type(0), std::out_of_range);
        EXPECT_THROW(func->parameter_name(0), std::out_of_range);
    }
    void TestFunctionParameter(Ptr<const Function> func, int paramIndex, const string& param_name, const string& param_type) {
        EXPECT_EQ(param_name, func->parameter_name(paramIndex));
        EXPECT_EQ(param_type, func->parameter_type(paramIndex));
    }
    
    /////////// namespace
    Ptr<const Namespace> TestNamespace(const string& name, AccessSpecifier access, size_t numVariables, 
                                       size_t numFunctions, size_t numClasses, size_t numNamespaces) {
        return TestNamespace(global_, name, access, numVariables, numFunctions, numClasses, numNamespaces);
    }
    Ptr<const Namespace> TestNamespace(Ptr<const Scope> scope, const string& name, AccessSpecifier access, 
                                       size_t numVariables, size_t numFunctions,
                                       size_t numClasses, size_t numNamespaces) {
        Ptr<const Namespace> var = scope->NestedNamespace(name);
        EXPECT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(access, var->access());
        TestScopeChildNums(var, numVariables, numFunctions, numClasses, numNamespaces);
        return var;
    }
    
    void TestScopeChildNums(Ptr<const Scope> scope, size_t numVariables, size_t numFunctions,
                            size_t numClasses, size_t numNamespaces) {
        EXPECT_EQ(numVariables, scope->GlobalVariablesNum());
        EXPECT_EQ(numFunctions, scope->NestedFunctionsNum());
        EXPECT_EQ(numClasses, scope->NestedClassesNum());
        EXPECT_EQ(numNamespaces, scope->NestedNamespacesNum());
    }
    
    ////////////// class
    Ptr<const Class> TestClass(const string& name, AccessSpecifier access, size_t numVariables, 
                                       size_t numFunctions, size_t numClasses) {
        return TestClass(global_, name, access, numVariables, numFunctions, numClasses);
    }
    Ptr<const Class> TestClass(Ptr<const Scope> scope, const string& name, AccessSpecifier access, size_t numVariables, 
                                       size_t numFunctions, size_t numClasses) {
        Ptr<const Class> var = scope->NestedClass(name);
        EXPECT_TRUE(static_cast<bool>(var));
        EXPECT_EQ(name, var->name());
        EXPECT_EQ(access, var->access());
        TestScopeChildNums(var, numVariables, numFunctions, numClasses, 0u);
        return var;
    }
    
    void TestClassBaseNum(Ptr<const Class> c, size_t numBases) {
        EXPECT_EQ(numBases, c->base_specifiers().size());
    }
    void TestClassBaseByIndex(Ptr<const Class> c, int baseIndex, const string& name, bool isVirtual, AccessSpecifier access) {
        auto bspec = c->base_specifiers().begin();
        for (int i = 0; i < baseIndex; i++, bspec++);
        
        EXPECT_EQ(name, bspec->name());
        EXPECT_EQ(isVirtual, bspec->is_virtual());
        EXPECT_EQ(access, bspec->access_specifier());   
    }
    
    void TestClassDestructorNotDefined(Ptr<const Class> c) {
        ASSERT_FALSE(static_cast<bool>(c->destructor()));
    }
    void TestClassDestructor(Ptr<const Class> c, bool isVirtual, AccessSpecifier access) {
        Ptr<const Function> func = c->destructor();
        internalCheckFunction(func, "~"+c->name(), "", access, false);
        //TODO: CHECK IF IS VIRTUAL
        TestFunctionNoParameters(func);
    }
    
    void TestClassConstructorNum(Ptr<const Class> c, size_t numConstructors) {
        EXPECT_EQ(numConstructors, c->constructors().size());
    }
    Ptr<const Function> TestClassConstructorByIndex(Ptr<const Class> c, int ctorIndex, AccessSpecifier access) {
        Ptr<const Function> func = c->constructors()[ctorIndex];
        internalCheckFunction(func, c->name(), "", access, false);
        return func;
    }
};

#include <opwig/nestednamespecifier.cc>
#include <opwig/declarator.cc>
#include <opwig/class.cc>
#include <opwig/classmembers.cc>
#include <opwig/function.cc>
#include <opwig/namespace.cc>
#include <opwig/variable.cc>
