%option yylineno
%{ 
#include "syntax.tab.h"
extern YYSTYPE yylval;
%}
type    int|float
digit  [0-9]
digits {digit}+
optionalFraction	\.{digits}
optionalExponent	[eE][+-]?{digits}
float	{digits}{optionalFraction}{optionalExponent}?
dec	[1-9][0-9]*
oct	0[1-7][0-7]*
hex	0[xX][1-9a-fA-F][0-9a-fA-F]* 
letter  [a-zA-Z]
ws  [\f\v\n\t ]
%%
{ws}+    {/*Do nothing*/}
";"   {printf("SEMI\n");}
","   {printf("COMMA\n");}
"="   {printf("ASSIGNOP\n");}
"+"   {printf("PLUS\n"); return PLUS;}
"-"   {printf("MINUS\n"); return MINUS;}
"*"   {printf("STAR\n"); return STAR;}
"/"   {printf("DIV\n"); return DIV;}
"&&"   {printf("AND\n");}
"||"   {printf("OR\n");}
"."   {printf("DOT\n");}
"!"   {printf("NOT\n");}
"("   {printf("LP\n");}
")"   {printf("RP\n");}
"["   {printf("LB\n");}
"]"   {printf("RB\n");}
"{"   {printf("LC\n");}
"}"   {printf("RC\n");}
struct   {printf("STRUCT\n");}
return   {printf("RETURN\n");}
if   {printf("IF\n");}
else   {printf("ELSE\n");}
while   {printf("WHILE\n");}
{type}	{printf("TYPE\n");}
{float}	{printf("FLOAT\n");}
{dec}|{oct}|{hex}|0    {printf("INT\n"); yylval = atoi(yytext); return INT;}
[a-zA-Z_][a-zA-Z0-9_]*   {printf("ID\n");}
. {printf("Error at Line %d: Mysterious characters %s\n",yylineno, yytext);}
%%
