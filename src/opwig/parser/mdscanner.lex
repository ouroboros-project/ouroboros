
%class-name             "MDScanner"
%class-header           "mdscanner.h"
%baseclass-header       "mdscannerbase.h"
%implementation-header  "mdscanner.ih"
%lex-source             "mdscanner-lex.cxx"
%namespace              "opwig"

ws                          [ \f\v\t\n]
digit                       [0-9]
nonzero                     [1-9]
hex                         [0-9A-Fa-f]
letter                      [A-Z_a-z]
universal_character_name    (\\u{hex}{hex}{hex}{hex}|\\U{hex}{hex}{hex}{hex}{hex}{hex}{hex}{hex})
non_digit                   ({letter}|{universal_character_name})

simple_escape_sequence      (\\\'|\\\"|\\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v)
octal_escape_sequence       (\\[0-7]|\\[0-7][0-7]|\\[0-7][0-7][0-7])
hexadecimal_escape_sequence (\\x{hex}+)
escape_sequence             ({simple_escape_sequence}|{octal_escape_sequence}|{hexadecimal_escape_sequence})

identifier                  ({non_digit}({non_digit}|{digit})*)

integer_literal             (((0)|({nonzero}{digit}*))[ULul]?)
hexadecimal_literal         (0[Xx]{hex}+)
octal_literal               (0[0-7]+)
float_literal               ({digit}*\.{digit}+([Ee][+-]{digit}+)?)
character_literal           (L?\'(([^\'\\\n])|(\\.))\')
string_literal              (L?\"(([^\"\\\n])|(\\.))*\")

pp_number                   (\.?{digit}({digit}|{non_digit}|[eE][-+]|\.)*)

%x string

%%

{ws}                  /* do nothing */;

{string_literal}      return MDParserBase::STRING_LITERAL;

{character_literal}   return MDParserBase::CHARACTER_LITERAL;

{integer_literal}     return MDParserBase::INTEGER_LITERAL;

{hexadecimal_literal} return MDParserBase::HEXADECIMAL_LITERAL;

{octal_literal}       return MDParserBase::OCTAL_LITERAL;

{float_literal}       return MDParserBase::FLOAT_LITERAL;

"asm"               { return MDParserBase::ASM; }
"auto"              { return MDParserBase::AUTO; }
"bool"              { return MDParserBase::PRIMITIVE; }
"catch"             { return MDParserBase::CATCH; }
"char"              { return MDParserBase::PRIMITIVE; }
"class"             { return MDParserBase::CLASS; }
"const"             { return MDParserBase::CONST; }
"default"           { return MDParserBase::DEFAULT; }
"delete"            { return MDParserBase::DELETE; }
"double"            { return MDParserBase::PRIMITIVE; }
"enum"              { return MDParserBase::ENUM; }
"explicit"          { return MDParserBase::EXPLICIT; }
"export"            { return MDParserBase::EXPORT; }
"extern"            { return MDParserBase::EXTERN; }
"false"             { return MDParserBase::FALSE; }
"float"             { return MDParserBase::PRIMITIVE; }
"friend"            { return MDParserBase::FRIEND; }
"inline"            { return MDParserBase::INLINE; }
"int"               { return MDParserBase::PRIMITIVE; }
"long"              { return MDParserBase::PRIMITIVE; }
"mutable"           { return MDParserBase::MUTABLE; }
"namespace"         { return MDParserBase::NAMESPACE; }
"operator"          { return MDParserBase::OPERATOR; }
"private"           { return MDParserBase::PRIVATE; }
"protected"         { return MDParserBase::PROTECTED; }
"public"            { return MDParserBase::PUBLIC; }
"register"          { return MDParserBase::REGISTER; }
"return"            { return MDParserBase::RETURN; }
"short"             { return MDParserBase::PRIMITIVE; }
"signed"            { return MDParserBase::SIGNED; }
"sizeof"            { return MDParserBase::SIZEOF; }
"static"            { return MDParserBase::STATIC; }
"struct"            { return MDParserBase::STRUCT; }
"template"          { return MDParserBase::TEMPLATE; }
"throw"             { return MDParserBase::THROW; }
"true"              { return MDParserBase::TRUE; }
"try"               { return MDParserBase::TRY; }
"typedef"           { return MDParserBase::TYPEDEF; }
"typeid"            { return MDParserBase::TYPEID; }
"typename"          { return MDParserBase::TYPENAME; }
"union"             { return MDParserBase::UNION; }
"unsigned"          { return MDParserBase::UNSIGNED; }
"using"             { return MDParserBase::USING; }
"virtual"           { return MDParserBase::VIRTUAL; }
"void"              { return MDParserBase::VOID; }
"volatile"          { return MDParserBase::VOLATILE; }
"wchar_t"           { return MDParserBase::PRIMITIVE; }
"::"                { return MDParserBase::SCOPE_OPERATOR; }
"*="                { return MDParserBase::ASSIGNMENT_OPERATOR; }
"/="                { return MDParserBase::ASSIGNMENT_OPERATOR; }
"%="                { return MDParserBase::ASSIGNMENT_OPERATOR; }
"+="                { return MDParserBase::ASSIGNMENT_OPERATOR; }
"-="                { return MDParserBase::ASSIGNMENT_OPERATOR; }
">>="               { return MDParserBase::ASSIGNMENT_OPERATOR; }
"<<="               { return MDParserBase::ASSIGNMENT_OPERATOR; }
"&="                { return MDParserBase::ASSIGNMENT_OPERATOR; }
"^="                { return MDParserBase::ASSIGNMENT_OPERATOR; }
"|="                { return MDParserBase::ASSIGNMENT_OPERATOR; }
"<<"                { return MDParserBase::EXPRESSION_OPERATOR; }
">>"                { return MDParserBase::EXPRESSION_OPERATOR; }
"=="                { return MDParserBase::EXPRESSION_OPERATOR; }
"!="                { return MDParserBase::EXPRESSION_OPERATOR; }
"<="                { return MDParserBase::EXPRESSION_OPERATOR; }
">="                { return MDParserBase::EXPRESSION_OPERATOR; }
"&&"                { return MDParserBase::EXPRESSION_OPERATOR; }
"||"                { return MDParserBase::EXPRESSION_OPERATOR; }
"++"                { return MDParserBase::EXPRESSION_OPERATOR; }
"--"                { return MDParserBase::EXPRESSION_OPERATOR; }
"->*"               { return MDParserBase::EXPRESSION_OPERATOR; }
"->"                { return MDParserBase::EXPRESSION_OPERATOR; }
".*"                { return MDParserBase::EXPRESSION_OPERATOR; }
"..."               { return MDParserBase::EXPRESSION_OPERATOR; }
"<:"                { return MDParserBase::EXPRESSION_OPERATOR; }
":>"                { return MDParserBase::EXPRESSION_OPERATOR; }
"while"             { return MDParserBase::WHILE; }
"for"               { return MDParserBase::FOR; }
"if"                { return MDParserBase::IF; }

{identifier} {
  if (currentIsTypeName()) return MDParserBase::TYPE_NAME;
  else if (!scope_stack_.empty() && scope_stack_.top()->name() == matched()) {
      return MDParserBase::CONSTRUCTOR_NAME;
  } 
  return MDParserBase::IDENTIFIER;
}

.                     return matched().front();

