
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

"bool"              { return MDParserBase::KEYWORD; }
"catch"             { return MDParserBase::KEYWORD; }
"char"              { return MDParserBase::KEYWORD; }
"class"             { return MDParserBase::KEYWORD; }
"const"             { return MDParserBase::KEYWORD; }
"double"            { return MDParserBase::KEYWORD; }
"enum"              { return MDParserBase::KEYWORD; }
"explicit"          { return MDParserBase::KEYWORD; }
"export"            { return MDParserBase::KEYWORD; }
"extern"            { return MDParserBase::KEYWORD; }
"false"             { return MDParserBase::KEYWORD; }
"float"             { return MDParserBase::KEYWORD; }
"friend"            { return MDParserBase::KEYWORD; }
"inline"            { return MDParserBase::KEYWORD; }
"int"               { return MDParserBase::KEYWORD; }
"long"              { return MDParserBase::KEYWORD; }
"mutable"           { return MDParserBase::KEYWORD; }
"namespace"         { return MDParserBase::KEYWORD; }
"operator"          { return MDParserBase::KEYWORD; }
"private"           { return MDParserBase::KEYWORD; }
"protected"         { return MDParserBase::KEYWORD; }
"public"            { return MDParserBase::KEYWORD; }
"register"          { return MDParserBase::KEYWORD; }
"return"            { return MDParserBase::KEYWORD; }
"short"             { return MDParserBase::KEYWORD; }
"signed"            { return MDParserBase::KEYWORD; }
"sizeof"            { return MDParserBase::KEYWORD; }
"static"            { return MDParserBase::KEYWORD; }
"struct"            { return MDParserBase::KEYWORD; }
"template"          { return MDParserBase::KEYWORD; }
"throw"             { return MDParserBase::KEYWORD; }
"true"              { return MDParserBase::KEYWORD; }
"try"               { return MDParserBase::KEYWORD; }
"typedef"           { return MDParserBase::KEYWORD; }
"typeid"            { return MDParserBase::KEYWORD; }
"typename"          { return MDParserBase::KEYWORD; }
"union"             { return MDParserBase::KEYWORD; }
"unsigned"          { return MDParserBase::KEYWORD; }
"using"             { return MDParserBase::KEYWORD; }
"virtual"           { return MDParserBase::KEYWORD; }
"void"              { return MDParserBase::KEYWORD; }
"volatile"          { return MDParserBase::KEYWORD; }
"wchar_t"           { return MDParserBase::KEYWORD; }

{identifier}          return MDParserBase::IDENTIFIER;

.                     return matched().front();

