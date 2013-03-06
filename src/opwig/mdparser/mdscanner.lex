
%class-name             "MDScanner"
%class-header           "mdscanner.h"
%baseclass-header       "mdscannerbase.h"
%implementation-header  "mdscanner.ih"
%lex-source             "mdscanner-lex.cxx"
%namespace              "opwig"

%%

[_a-zA-Z][_a-zA-Z0-9]*  return MDParserBase::IDENTIFIER;

.|\n                    { /*skip*/ }

