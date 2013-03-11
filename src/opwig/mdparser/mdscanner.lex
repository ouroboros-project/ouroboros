
%class-name             "MDScanner"
%class-header           "mdscanner.h"
%baseclass-header       "mdscannerbase.h"
%implementation-header  "mdscanner.ih"
%lex-source             "mdscanner-lex.cxx"
%namespace              "opwig"

ws                          [ \f\v\t\n]
digit                       [0-9]
hex                         [0-9A-Fa-f]
letter                      [A-Z_a-z]
universal_character_name    (\\u{hex}{hex}{hex}{hex}|\\U{hex}{hex}{hex}{hex}{hex}{hex}{hex}{hex})
non_digit                   ({letter}|{universal_character_name})

simple_escape_sequence      (\\\'|\\\"|\\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v)
octal_escape_sequence       (\\[0-7]|\\[0-7][0-7]|\\[0-7][0-7][0-7])
hexadecimal_escape_sequence (\\x{hex}+)
escape_sequence             ({simple_escape_sequence}|{octal_escape_sequence}|{hexadecimal_escape_sequence})

identifier                  ({non_digit}({non_digit}|{digit})*)

integer_literal             ({digit}+[ULul]?)
float_literal               ({digit}*\.{digit}+([Ee][+-]{digit}+)?)
character_literal           (L?\'(([^\'\\\n])|(\\.))\')
string_literal              (L?\"(([^\"\\\n])|(\\.))*\")

pp_number                   (\.?{digit}({digit}|{non_digit}|[eE][-+]|\.)*)

%x string

%%

{ws}                /* do nothing */;

{string_literal}    return MDParserBase::STRING_LITERAL;

{character_literal} return MDParserBase::CHARACTER_LITERAL;

{integer_literal}   return MDParserBase::INTEGER_LITERAL;

{float_literal}     return MDParserBase::FLOAT_LITERAL;

{identifier}        return MDParserBase::IDENTIFIER;

