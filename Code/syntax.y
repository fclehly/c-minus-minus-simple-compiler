%{
#include <stdio.h>
#include "lex.yy.c"
%}
/* declared types */
%union {
	int type_int;
	float type_float;
	double type_double;
}
/* declared tokents */
%token <type_int> INT
%token <type_float> FLOAT
%token PLUS MINUS STAR DIV

/* declared non-terminals */
%type <type_double> Exp Factor Term

%%
Calc:	/* empty */
	|	Exp	{printf("= %lf\n", $1);}
	;
Exp:	Factor
	|	Exp PLUS Factor	{$$ = $1 + $3;}
	|	Exp MINUS Factor	{$$ = $1 - $3;}
	;
Factor:	Term
	|	Factor STAR Term	{$$ = $1 * $3;}
	|	Factor DIV Term	{$$ = $1 / $3;}
	;
Term:	INT		{$$ = $1;}
	|	FLOAT	{$$ = $1;}
	;
%%
int main() {
	return yyparse();
}
int yyerror(char* msg) {
	return fprintf(stderr, "error: %s\n", msg);
}
