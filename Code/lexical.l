%option yylineno
%{
#include <stdio.h>
%}
type    int|float
digit  [0-9]
letter  [a-zA-Z]
ws  [\f\v\n\t ]
%%
{ws}+    {/*Do nothing*/}
";"   {printf("SEMI\n");}
","   {printf("COMMA\n");}
"="   {printf("ASSIGNOP\n");}
"+"   {printf("PLUS\n");}
"-"   {printf("MINUS\n");}
"*"   {printf("STAR\n");}
"/"   {printf("DIV\n");}
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
{digit}+    {printf("INT\n");}
{type}  {printf("TYPE\n");}
[a-zA-Z_][a-zA-Z0-9_]*   {printf("ID\n");}
. {printf("Error at Line %d: Mysterious characters %s\n",yylineno, yytext);}
%%
int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror("argv[1]");
            return 1;
        }
    }
    yylex();
    return 0;
}