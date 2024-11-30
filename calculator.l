%{
    #include "calculator.tab.h"
    #include "calculator.h"
%}
%option noyywrap yylineno nodefault

%%

"+" |
"-" |
"*" |
"/" |
"|" |
"^" |
"%" |
"(" |
")"     { return yytext[0]; }
"ans"   { yylval.num = calc_data.ans; return ANS; }
([0-9]*\.[0-9]+|[0-9]+\.[0-9]*|[0-9]+)  { yylval.num = atof(yytext); return FLOATNUM; }
\n      { return NEWLINE; }
[ \t]   { /* whitespace */ }
"//".*  { /* comment */ }
.       { printf("Unknown character (line %d): %c\n", calc_data.current_line, *yytext); }
%%