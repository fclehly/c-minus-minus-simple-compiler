%option yylineno
%{ 
#include "syntax.tab.h"
extern YYSTYPE yylval;
int yycolumn = 1;
/*#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;*/
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
relop	>|<|>=|<=|==|!=
ws  [\f\v\t ]
%%
{ws}+    {/*Do nothing*/}
\n		{yycolumn = 1;}
";"   {printf("SEMI\n"); return SEMI;}
","   {printf("COMMA\n"); return COMMA;}
"="   {printf("ASSIGNOP\n"); return ASSIGNOP;}
"+"   {printf("PLUS\n"); return PLUS;}
"-"   {printf("MINUS\n"); return MINUS;}
"*"   {printf("STAR\n"); return STAR;}
"/"   {printf("DIV\n"); return DIV;}
"&&"   {printf("AND\n"); return AND;}
"||"   {printf("OR\n"); return OR;}
"."   {printf("DOT\n"); return DOT;}
"!"   {printf("NOT\n"); return NOT;}
"("   {printf("LP\n"); return LP;}
")"   {printf("RP\n"); return RP;}
"["   {printf("LB\n"); return LB;}
"]"   {printf("RB\n"); return RB;}
"{"   {printf("LC\n"); return LC;}
"}"   {printf("RC\n"); return RC;}
{relop}	{printf("RELOP\n"); return RELOP;}
struct   {printf("STRUCT\n"); return STRUCT;}
return   {printf("RETURN\n"); return RETURN;}
if   {printf("IF\n"); return IF;}
else   {printf("ELSE\n"); return ELSE;}
while   {printf("WHILE\n"); return WHILE;}
{type}	{printf("TYPE\n"); return TYPE;}
{float}	{printf("FLOAT\n"); return FLOAT;}
{dec}|{oct}|{hex}|0    {printf("INT\n"); yylval = atoi(yytext); return INT;}
[a-zA-Z_][a-zA-Z0-9_]*   {printf("ID\n");}
. {printf("Error at Line %d: Mysterious characters %s\n",yylineno, yytext); return ID;}
%%
