%option yylineno
%{ 
#include "syntax.tab.h"
extern YYSTYPE yylval;
int yycolumn = 1;
/*#define YY_USER_ACTION {\
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng; \
	}
	*/
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
";"   {return SEMI;}
","   {return COMMA;}
"="   {return ASSIGNOP;}
"+"   {return PLUS;}
"-"   {return MINUS;}
"*"   {return STAR;}
"/"   {return DIV;}
"&&"  {return AND;}
"||"  {return OR;}
"."   {return DOT;}
"!"   {return NOT;}
"("   {return LP;}
")"   {return RP;}
"["   {return LB;}
"]"   {return RB;}
"{"   {return LC;}
"}"   {return RC;}
{relop}	{return RELOP;}
struct   {return STRUCT;}
return   {return RETURN;}
if   {return IF;}
else   {return ELSE;}
while   {return WHILE;}
{type}	{return TYPE;}
{float}	{return FLOAT;}
{dec}|{oct}|{hex}|0    {yylval = atoi(yytext); return INT;}
[a-zA-Z_][a-zA-Z0-9_]*   {return ID;}
. {printf("Error type A at Line %d: Mysterious characters %s\n",yylineno, yytext);}
%%
