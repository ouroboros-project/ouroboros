
%class-name             "MDScanner"
%class-header           "mdscanner.h"
%baseclass-header       "mdscannerbase.h"
%implementation-header  "mdscanner.ih"
%lex-source             "mdscanner-lex.cxx"
%namespace              "opwig"

ws                          [ \f\v\t]
digit                       [0-9]
hex                         [0-9A-Fa-f]
letter                      [A-Z_a-z]
simple_escape_sequence      (\\\'|\\\"|\\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v)
octal_escape_sequence       (\\[0-7]|\\[0-7][0-7]|\\[0-7][0-7][0-7])
hexadecimal_escape_sequence (\\x{hex}+)
escape_sequence             ({simple_escape_sequence}|{octal_escape_sequence}|{hexadecimal_escape_sequence})
universal_character_name    (\\u{hex}{hex}{hex}{hex}|\\U{hex}{hex}{hex}{hex}{hex}{hex}{hex}{hex})
non_digit                   ({letter}|{universal_character_name})
identifier                  ({non_digit}({non_digit}|{digit})*)

character_lit               (L?\'([^\'\\\n]|\\.)*)
character_literal           ({character_lit}\')

string_lit                  (L?\"([^\"\\\n]|\\.)*)
string_literal              ({string_lit}\")

pp_number                   (\.?{digit}({digit}|{non_digit}|[eE][-+]|\.)*)

%%

[_a-zA-Z][_a-zA-Z0-9]*  return MDParserBase::IDENTIFIER;

.|\n                    { /*skip*/ }

