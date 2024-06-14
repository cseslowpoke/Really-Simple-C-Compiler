/* declarations */
%option noyywrap
%{
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"    
%}

/* patterns & actions */

%%

"main"   { return KW_MAIN; }
"return" { return KW_RET; }
"int"    { return KW_INT; }
"print"  { return KW_PRINT; }
"scan"   { return KW_SCAN; }
"("      { return LPAREN; }
")"      { return RPAREN; }
"{"      { return LBRCKT; }
"}"      { return RBRCKT; }

";"      { return SEMI; }
","      { return COMMA; }
"="      { return ASSN; }

"+"      { return ADD; }
"-"      { return SUB; }
"*"      { return MUL; }
"/"      { return DIV; }
[a-z] {
    yylval.identifier = strdup(yytext);
    return ID;
}

[0-9]+   { yylval.value = atoi(yytext); return NUMBER; }
[ \t]    {}
\n       {}
.        {}
%%

/* 
gcc -o calc lex.yy.c parser.tab.c parser.tab.h ast.c -lfl
 */