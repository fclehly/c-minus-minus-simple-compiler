%option yylineno
%{
//#include <stdlib.h>  
//#include <string.h>
#include <stdio.h>
#include "syntax.tab.h"
#include "st.h"

int yycolumn = 1;

#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng; 
extern YYSTYPE yylval;
//extern ST_NODE* st_create_node(int lineno, char* name, ST_NODE_TYPE type, char* value);
	
%}
type    int|float
digit  [0-9]
digits {digit}+
optionalFraction	\.{digits}
optionalExponent	[eE][+-]?{digits}
float	{digits}{optionalFraction}
dec	[1-9][0-9]*
oct	0[1-7][0-7]*
hex	0[xX][1-9a-fA-F][0-9a-fA-F]* 
letter  [a-zA-Z]
relop	>|<|>=|<=|==|!=
ws  [\f\v\t ]
%%
{ws}+    {/*Do nothing*/}
"//"	{
			char c = input(); 
			while (c != '\n') 
				c = input();
		}
"/*"	{
			char c1, c2 = 0; 
			while (c2 = input()) {
				if (c2 == -1) {
					printf("Error type A at Line %d: met EOF\n", yylineno);	
					is_err = 1;
					break;
				}
				if (c1 == '*' && c2 == '/')
					 break; 
				c1 = c2;
			}
		}
\n		{yycolumn = 1;}
";"   {yylval.type_st_node = st_create_node(yylineno, "SEMI", SEMI_T, yytext); return SEMI;}
","   {yylval.type_st_node = st_create_node(yylineno, "COMMA", COMMA_T, yytext); return COMMA;}
"="   {yylval.type_st_node = st_create_node(yylineno, "ASSIGNOP", ASSIGNOP_T, yytext); return ASSIGNOP;}
"+"   {yylval.type_st_node = st_create_node(yylineno, "PLUS", PLUS_T, yytext); return PLUS;}
"-"   {yylval.type_st_node = st_create_node(yylineno, "MINUS", MINUS_T, yytext); return MINUS;}
"*"   {yylval.type_st_node = st_create_node(yylineno, "STAR", STAR_T, yytext); return STAR;}
"/"   {yylval.type_st_node = st_create_node(yylineno, "DIV", DIV_T, yytext); return DIV;}
"&&"  {yylval.type_st_node = st_create_node(yylineno, "AND", AND_T, yytext); return AND;}
"||"  {yylval.type_st_node = st_create_node(yylineno, "OR", OR_T, yytext); return OR;}
"."   {yylval.type_st_node = st_create_node(yylineno, "DOT", DOT_T, yytext); return DOT;}
"!"   {yylval.type_st_node = st_create_node(yylineno, "NOT", NOT_T, yytext); return NOT;}
"("   {yylval.type_st_node = st_create_node(yylineno, "LP", LP_T, yytext); return LP;}
")"   {yylval.type_st_node = st_create_node(yylineno, "RP", RP_T, yytext); return RP;}
"["   {yylval.type_st_node = st_create_node(yylineno, "LB", LB_T, yytext); return LB;}
"]"   {yylval.type_st_node = st_create_node(yylineno, "RB", RB_T, yytext); return RB;}
"{"   {yylval.type_st_node = st_create_node(yylineno, "LC", LC_T, yytext); return LC;}
"}"   {yylval.type_st_node = st_create_node(yylineno, "RC", RC_T, yytext); return RC;}
{relop}	{yylval.type_st_node = st_create_node(yylineno, "RELOP", RELOP_T, yytext); return RELOP;}
struct   {yylval.type_st_node = st_create_node(yylineno, "STRUCT", STRUCT_T, yytext); return STRUCT;}
return   {yylval.type_st_node = st_create_node(yylineno, "RETURN", RETURN_T, yytext); return RETURN;}
if   {yylval.type_st_node = st_create_node(yylineno, "IF", IF_T, yytext); return IF;}
else   {yylval.type_st_node = st_create_node(yylineno, "ELSE", ELSE_T, yytext); return ELSE;}
while   {yylval.type_st_node = st_create_node(yylineno, "WHILE", WHILE_T, yytext); return WHILE;}
{type}	{yylval.type_st_node = st_create_node(yylineno, "TYPE", TYPE_T, yytext); return TYPE;}
{float}	{yylval.type_st_node = st_create_node(yylineno, "FLOAT", FLOAT_T, yytext); return FLOAT;}
{dec}|0    {yylval.type_st_node = st_create_node(yylineno, "INT", INT_T, yytext); return INT;}
[a-zA-Z_][a-zA-Z0-9_]*   {yylval.type_st_node = st_create_node(yylineno, "ID", ID_T, yytext); return ID;}
. {printf("Error type A at Line %d: Mysterious characters %s\n",yylineno, yytext); is_err = 1;}
%%
