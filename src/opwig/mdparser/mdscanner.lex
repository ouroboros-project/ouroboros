
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

integer_literal             ({nonzero}{digit}*[ULul]?)
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

"asm"               { /*LEX_STATIC_TOKEN(ASM);*/ }
"auto"              { /*LEX_STATIC_TOKEN(AUTO);*/ }
"bool"              { /*LEX_C_STATIC_TOKEN(BOOL);*/ }
"break"             { /*LEX_STATIC_TOKEN(BREAK);*/ }
"case"              { /*LEX_STATIC_TOKEN(CASE);*/ }
"catch"             { /*LEX_C_STATIC_TOKEN(CATCH);*/ }
"char"              { /*LEX_STATIC_TOKEN(CHAR);*/ }
"class"             { /*LEX_C_STATIC_TOKEN(CLASS);*/ }
"const"             { /*LEX_STATIC_TOKEN(CONST);*/ }
"const_cast"        { /*LEX_C_STATIC_TOKEN(CONST_CAST);*/ }
"continue"          { /*LEX_STATIC_TOKEN(CONTINUE);*/ }
"default"           { /*LEX_STATIC_TOKEN(DEFAULT);*/ }
"delete"            { /*LEX_C_STATIC_TOKEN(DELETE);*/ }
"do"                { /*LEX_STATIC_TOKEN(DO);*/ }
"double"            { /*LEX_STATIC_TOKEN(DOUBLE);*/ }
"dynamic_cast"      { /*LEX_C_STATIC_TOKEN(DYNAMIC_CAST);*/ }
"else"              { /*LEX_STATIC_TOKEN(ELSE);*/ }
"enum"              { /*LEX_STATIC_TOKEN(ENUM);*/ }
"explicit"          { /*LEX_C_STATIC_TOKEN(EXPLICIT);*/ }
"export"            { /*LEX_C_STATIC_TOKEN(EXPORT);*/ }
"extern"            { /*LEX_STATIC_TOKEN(EXTERN);*/ }
"false"             { /*LEX_C_STATIC_TOKEN(FALSE);*/ }
"float"             { /*LEX_STATIC_TOKEN(FLOAT);*/ }
"for"               { /*LEX_STATIC_TOKEN(FOR);*/ }
"friend"            { /*LEX_STATIC_TOKEN(FRIEND);*/ }
"goto"              { /*LEX_STATIC_TOKEN(GOTO);*/ }
"if"                { /*LEX_STATIC_TOKEN(IF);*/ }
"inline"            { /*LEX_C_STATIC_TOKEN(INLINE);*/ }
"int"               { /*LEX_STATIC_TOKEN(INT);*/ }
"long"              { /*LEX_STATIC_TOKEN(LONG);*/ }
"mutable"           { /*LEX_C_STATIC_TOKEN(MUTABLE);*/ }
"namespace"         { /*LEX_C_STATIC_TOKEN(NAMESPACE);*/ }
"new"               { /*LEX_C_STATIC_TOKEN(NEW);*/ }
"operator"          { /*LEX_C_STATIC_TOKEN(OPERATOR);*/ }
"private"           { /*LEX_C_STATIC_TOKEN(PRIVATE);*/ }
"protected"         { /*LEX_C_STATIC_TOKEN(PROTECTED);*/ }
"public"            { /*LEX_C_STATIC_TOKEN(PUBLIC);*/ }
"register"          { /*LEX_STATIC_TOKEN(REGISTER);*/ }
"reinterpret_cast"  { /*LEX_C_STATIC_TOKEN(REINTERPRET_CAST);*/ }
"return"            { /*LEX_STATIC_TOKEN(RETURN);*/ }
"short"             { /*LEX_STATIC_TOKEN(SHORT);*/ }
"signed"            { /*LEX_C_STATIC_TOKEN(SIGNED);*/ }
"sizeof"            { /*LEX_STATIC_TOKEN(SIZEOF);*/ }
"static"            { /*LEX_STATIC_TOKEN(STATIC);*/ }
"static_cast"       { /*LEX_C_STATIC_TOKEN(STATIC_CAST);*/ }
"struct"            { /*LEX_STATIC_TOKEN(STRUCT);*/ }
"switch"            { /*LEX_STATIC_TOKEN(SWITCH);*/ }
"template"          { /*LEX_C_STATIC_TOKEN(TEMPLATE);*/ }
"this"              { /*LEX_C_STATIC_TOKEN(THIS);*/ }
"throw"             { /*LEX_C_STATIC_TOKEN(THROW);*/ }
"true"              { /*LEX_C_STATIC_TOKEN(TRUE);*/ }
"try"               { /*LEX_C_STATIC_TOKEN(TRY);*/ }
"typedef"           { /*LEX_STATIC_TOKEN(TYPEDEF);*/ }
"typeid"            { /*LEX_C_STATIC_TOKEN(TYPEID);*/ }
"typename"          { /*LEX_C_STATIC_TOKEN(TYPENAME);*/ }
"union"             { /*LEX_STATIC_TOKEN(UNION);*/ }
"unsigned"          { /*LEX_STATIC_TOKEN(UNSIGNED);*/ }
"using"             { /*LEX_C_STATIC_TOKEN(USING);*/ }
"virtual"           { /*LEX_STATIC_TOKEN(VIRTUAL);*/ }
"void"              { /*LEX_STATIC_TOKEN(VOID);*/ }
"volatile"          { /*LEX_STATIC_TOKEN(VOLATILE);*/ }
"wchar_t"           { /*LEX_C_STATIC_TOKEN(WCHAR_T);*/ }
"while"             { /*LEX_STATIC_TOKEN(WHILE);*/ }

{identifier}          return MDParserBase::IDENTIFIER;

.                     return matched().front();

