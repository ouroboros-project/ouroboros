// Generated by Bisonc++ V4.02.01 on Fri, 10 May 2013 22:11:27 -0300

#ifndef opwigMDParserBase_h_included
#define opwigMDParserBase_h_included

#include <exception>
#include <vector>
#include <iostream>

// $insert preincludes
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <opwig/parser/mdparser-preincludes.h>

namespace // anonymous
{
    struct PI__;
}

// $insert namespace-open
namespace opwig
{

// $insert polymorphic
enum class Tag__
{
    ParameterSeq,
    DeclaratorSeq,
    Parameter,
    Declarator,
    String,
    AccessSpecifier,
    BaseSpecifier,
    Class,
    Type,
    BaseSpecifierSeq,
    DeclarationSeq,
    StringSeq,
    Declaration,
};

namespace Meta__
{
    template <Tag__ tag>
    struct TypeOfBase;

    template <typename Tp_>
    struct TagOf;

// $insert polymorphicSpecializations
    template <>
    struct TagOf<std::shared_ptr<md::ParameterList>>
    {
        static Tag__ const tag = Tag__::ParameterSeq;
    };

    template <>
    struct TagOf<std::shared_ptr<parser::DeclaratorList>>
    {
        static Tag__ const tag = Tag__::DeclaratorSeq;
    };

    template <>
    struct TagOf<md::Parameter>
    {
        static Tag__ const tag = Tag__::Parameter;
    };

    template <>
    struct TagOf<parser::Declarator>
    {
        static Tag__ const tag = Tag__::Declarator;
    };

    template <>
    struct TagOf<std::string>
    {
        static Tag__ const tag = Tag__::String;
    };

    template <>
    struct TagOf<md::AccessSpecifier>
    {
        static Tag__ const tag = Tag__::AccessSpecifier;
    };

    template <>
    struct TagOf<parser::BaseSpecifier>
    {
        static Tag__ const tag = Tag__::BaseSpecifier;
    };

    template <>
    struct TagOf<md::Ptr<md::Class>>
    {
        static Tag__ const tag = Tag__::Class;
    };

    template <>
    struct TagOf<TypeAction>
    {
        static Tag__ const tag = Tag__::Type;
    };

    template <>
    struct TagOf<std::shared_ptr<BaseSpecifierList>>
    {
        static Tag__ const tag = Tag__::BaseSpecifierSeq;
    };

    template <>
    struct TagOf<std::shared_ptr<ScopeActionList>>
    {
        static Tag__ const tag = Tag__::DeclarationSeq;
    };

    template <>
    struct TagOf<std::shared_ptr<StringList>>
    {
        static Tag__ const tag = Tag__::StringSeq;
    };

    template <>
    struct TagOf<ScopeAction>
    {
        static Tag__ const tag = Tag__::Declaration;
    };

    template <>
    struct TypeOfBase<Tag__::ParameterSeq>
    {
        typedef std::shared_ptr<md::ParameterList> DataType;
    };

    template <>
    struct TypeOfBase<Tag__::DeclaratorSeq>
    {
        typedef std::shared_ptr<parser::DeclaratorList> DataType;
    };

    template <>
    struct TypeOfBase<Tag__::Parameter>
    {
        typedef md::Parameter DataType;
    };

    template <>
    struct TypeOfBase<Tag__::Declarator>
    {
        typedef parser::Declarator DataType;
    };

    template <>
    struct TypeOfBase<Tag__::String>
    {
        typedef std::string DataType;
    };

    template <>
    struct TypeOfBase<Tag__::AccessSpecifier>
    {
        typedef md::AccessSpecifier DataType;
    };

    template <>
    struct TypeOfBase<Tag__::BaseSpecifier>
    {
        typedef parser::BaseSpecifier DataType;
    };

    template <>
    struct TypeOfBase<Tag__::Class>
    {
        typedef md::Ptr<md::Class> DataType;
    };

    template <>
    struct TypeOfBase<Tag__::Type>
    {
        typedef TypeAction DataType;
    };

    template <>
    struct TypeOfBase<Tag__::BaseSpecifierSeq>
    {
        typedef std::shared_ptr<BaseSpecifierList> DataType;
    };

    template <>
    struct TypeOfBase<Tag__::DeclarationSeq>
    {
        typedef std::shared_ptr<ScopeActionList> DataType;
    };

    template <>
    struct TypeOfBase<Tag__::StringSeq>
    {
        typedef std::shared_ptr<StringList> DataType;
    };

    template <>
    struct TypeOfBase<Tag__::Declaration>
    {
        typedef ScopeAction DataType;
    };


        // determining the nature of a polymorphic semantic value:
        // if it's a class-type, use 'Type const &' as returntype of const
        // functions; if it's a built-in type (like 'int') use Type:

    struct ClassType
    {
        char _[2];
    };
    
    struct BuiltinType
    {
        char _;
    };

    template <typename T>
    BuiltinType test(...);

    template <typename T>
    ClassType test(void (T::*)());

    template <Tag__ tg_>
    struct TypeOf: public TypeOfBase<tg_>
    {
        typedef typename TypeOfBase<tg_>::DataType DataType;
        enum: bool 
        { 
            isBuiltinType = sizeof(test<DataType>(0)) == sizeof(BuiltinType)
        };

        typedef typename std::conditional<
                    isBuiltinType, DataType, DataType const &
                >::type ReturnType;
    };

        // The Base class: 
        // Individual semantic value classes are derived from this class.
        // This class offers a member returning the value's Tag__
        // and two member templates get() offering const/non-const access to
        // the actual semantic value type.
    class Base
    {
        Tag__ d_tag;
    
        protected:
            Base(Tag__ tag);

        public:
            Base(Base const &other) = delete;
            virtual ~Base();

            Tag__ tag() const;
    
            template <Tag__ tg_>
            typename TypeOf<tg_>::ReturnType get() const;
    
            template <Tag__ tg_>
            typename TypeOf<tg_>::DataType &get();
    };
    
        // The class Semantic is derived from Base. It stores a particular
        // semantic value type. The stored data are declared 'mutable' to
        // allow the definitions of a const and non-const conversion operator.
        // This way, const objects continue to offer non-modifiable data
    template <Tag__ tg_>
    class Semantic: public Base
    {
        typedef typename TypeOf<tg_>::DataType DataType;
    
        mutable DataType d_data;
    
        public:
            typedef typename TypeOf<tg_>::ReturnType ReturnType;
    
                // The default constructor and constructors for 
                // defined data types are available
            Semantic();
            Semantic(DataType const &data);
            Semantic(DataType &&tmp);

                // Conversion operators allow const/non-const access to d_data
            operator ReturnType() const;
            operator DataType &();
    };

        // The class Stype wraps the shared_ptr holding a pointer to Base.
        // It becomes the polymorphic STYPE__
        // Constructors expect (l/r-value) references to defined semantic
        // value types.
        // It also wraps Base's get members, allowing constructions like
        // $$.get<INT> to be used, rather than $$->get<INT>.
        // Furthermore, its operator= can be used to assign a Semantic *
        // directly to the SType object. The free functions (in the parser's
        // namespace (if defined)) semantic__ can be used to obtain a 
        // Semantic *. 
    class SType: public std::shared_ptr<Base>
    {
        public:
            SType() = default;
            SType(SType const &other) = default;
            SType(SType &&tmp) = default;
        
            SType &operator=(SType const &rhs) = default;
            SType &operator=(SType &&tmp) = default;
            template <typename Tp_>
            SType &operator=(Tp_ &&value);

            Tag__ tag() const;

                // this get()-member checks for 0-pointer and correct tag 
                // in shared_ptr<Base>, and may throw a logic_error
            template <Tag__ tg_>                    
            typename TypeOf<tg_>::ReturnType get() const;
    
                // this get()-member checks for 0-pointer and correct tag 
                // in shared_ptr<Base>, and resets the shared_ptr's Base * 
                // to point to Meta::__Semantic<tg_>() if not
            template <Tag__ tg_>
            typename TypeOf<tg_>::DataType &get();

                // the data()-members do not check, and may result in a 
                // bad_cast exception or segfault if used incorrectly

            template <Tag__ tg_>
            typename TypeOf<tg_>::ReturnType data() const;

            template <Tag__ tg_>
            typename TypeOf<tg_>::DataType &data();
    };

}  // namespace Meta__

class MDParserBase
{
    public:
// $insert tokens

    // Symbolic tokens:
    enum Tokens__
    {
        IDENTIFIER = 257,
        CONSTRUCTOR_NAME,
        PRIMITIVE,
        CATCH,
        CLASS,
        CONST,
        ENUM,
        EXPLICIT,
        EXPORT,
        EXTERN,
        FALSE,
        FRIEND,
        INLINE,
        MUTABLE,
        NAMESPACE,
        OPERATOR,
        PRIVATE,
        PROTECTED,
        PUBLIC,
        REGISTER,
        RETURN,
        SIGNED,
        SIZEOF,
        STATIC,
        STRUCT,
        TEMPLATE,
        THROW,
        TRUE,
        TRY,
        TYPEDEF,
        TYPEID,
        TYPENAME,
        UNION,
        UNSIGNED,
        USING,
        VIRTUAL,
        VOID,
        VOLATILE,
        STRING_LITERAL,
        CHARACTER_LITERAL,
        INTEGER_LITERAL,
        HEXADECIMAL_LITERAL,
        OCTAL_LITERAL,
        FLOAT_LITERAL,
    };

// $insert STYPE
    typedef Meta__::SType STYPE__;


    private:
        int d_stackIdx__;
        std::vector<size_t>   d_stateStack__;
        std::vector<STYPE__>  d_valueStack__;

    protected:
        enum Return__
        {
            PARSE_ACCEPT__ = 0,   // values used as parse()'s return values
            PARSE_ABORT__  = 1
        };
        enum ErrorRecovery__
        {
            DEFAULT_RECOVERY_MODE__,
            UNEXPECTED_TOKEN__,
        };
        bool        d_debug__;
        size_t      d_nErrors__;
        size_t      d_requiredTokens__;
        size_t      d_acceptedTokens__;
        int         d_token__;
        int         d_nextToken__;
        size_t      d_state__;
        STYPE__    *d_vsp__;
        STYPE__     d_val__;
        STYPE__     d_nextVal__;

        MDParserBase();

        void ABORT() const;
        void ACCEPT() const;
        void ERROR() const;
        void clearin();
        bool debug() const;
        void pop__(size_t count = 1);
        void push__(size_t nextState);
        void popToken__();
        void pushToken__(int token);
        void reduce__(PI__ const &productionInfo);
        void errorVerbose__();
        size_t top__() const;

    public:
        void setDebug(bool mode);
}; 

inline bool MDParserBase::debug() const
{
    return d_debug__;
}

inline void MDParserBase::setDebug(bool mode)
{
    d_debug__ = mode;
}

inline void MDParserBase::ABORT() const
{
    throw PARSE_ABORT__;
}

inline void MDParserBase::ACCEPT() const
{
    throw PARSE_ACCEPT__;
}

inline void MDParserBase::ERROR() const
{
    throw UNEXPECTED_TOKEN__;
}

// $insert polymorphicInline
namespace Meta__
{

inline Base::Base(Tag__ tag)
:
    d_tag(tag)
{}

inline Tag__ Base::tag() const
{
    return d_tag;
}

template <Tag__ tg_>
inline Semantic<tg_>::Semantic()
:
    Base(tg_),
    d_data(typename TypeOf<tg_>::DataType())
{}

template <Tag__ tg_>
inline Semantic<tg_>::Semantic(typename TypeOf<tg_>::DataType const &data)
:
    Base(tg_),
    d_data(data)
{}

template <Tag__ tg_>
inline Semantic<tg_>::Semantic(typename TypeOf<tg_>::DataType &&tmp)
:
    Base(tg_),
    d_data(std::move(tmp))
{}

template <Tag__ tg_>
inline Semantic<tg_>::operator ReturnType() const
{
    return d_data;
}

template <Tag__ tg_>
inline Semantic<tg_>::operator typename Semantic<tg_>::DataType &()
{
    return d_data;
}

template <Tag__ tg_>
inline typename TypeOf<tg_>::ReturnType Base::get() const
{
    return dynamic_cast<Semantic<tg_> const &>(*this);
}

template <Tag__ tg_>
inline typename TypeOf<tg_>::DataType &Base::get()
{
    return dynamic_cast<Semantic<tg_> &>(*this);
}

inline Tag__ SType::tag() const
{
    return std::shared_ptr<Base>::get()->tag();
}
    
template <Tag__ tg_>
inline typename TypeOf<tg_>::ReturnType SType::get() const
{
    if (std::shared_ptr<Base>::get() == 0  || tag() != tg_)
        throw std::logic_error("undefined semantic value requested");

    return std::shared_ptr<Base>::get()->get<tg_>();
}

template <Tag__ tg_>
inline typename TypeOf<tg_>::DataType &SType::get()
{
                    // if we're not yet holding a (tg_) value, initialize to 
                    // a Semantic<tg_> holding a default value
    if (std::shared_ptr<Base>::get() == 0 || tag() != tg_)
        reset(new Semantic<tg_>());

    return std::shared_ptr<Base>::get()->get<tg_>();
}

template <Tag__ tg_>
inline typename TypeOf<tg_>::ReturnType SType::data() const
{
    return std::shared_ptr<Base>::get()->get<tg_>();
}

template <Tag__ tg_>
inline typename TypeOf<tg_>::DataType &SType::data()
{
    return std::shared_ptr<Base>::get()->get<tg_>();
}

template <bool, typename Tp_>
struct Assign;

template <typename Tp_>
struct Assign<true, Tp_>
{
    static SType &assign(SType *lhs, Tp_ &&tp);
};

template <typename Tp_>
struct Assign<false, Tp_>
{
    static SType &assign(SType *lhs, Tp_ const &tp);
};

template <>
struct Assign<false, SType>
{
    static SType &assign(SType *lhs, SType const &tp);
};

template <typename Tp_>
inline SType &Assign<true, Tp_>::assign(SType *lhs, Tp_ &&tp)
{
    lhs->reset(new Semantic<TagOf<Tp_>::tag>(std::move(tp)));
    return *lhs;
}

template <typename Tp_>
inline SType &Assign<false, Tp_>::assign(SType *lhs, Tp_ const &tp)
{
    lhs->reset(new Semantic<TagOf<Tp_>::tag>(tp));
    return *lhs;
}

inline SType &Assign<false, SType>::assign(SType *lhs, SType const &tp)
{
    return lhs->operator=(tp);
}

template <typename Tp_>
inline SType &SType::operator=(Tp_ &&rhs) 
{
    return Assign<
                std::is_rvalue_reference<Tp_ &&>::value, 
                typename std::remove_reference<Tp_>::type
           >::assign(this, std::forward<Tp_>(rhs));
}

} // namespace Meta__

// As a convenience, when including ParserBase.h its symbols are available as
// symbols in the class Parser, too.
#define MDParser MDParserBase

// $insert namespace-close
}

#endif


