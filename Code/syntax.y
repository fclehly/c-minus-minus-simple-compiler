%{
#include <stdio.h>
#include "lex.yy.c"
//#define YYDEBUG 1
extern int yyerror(char* msg);
%}
/* declared types */
/* declared tokents */
%token LC RC COMMA SEMI STRUCT RETURN IF WHILE TYPE
%token FLOAT INT ID

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left MINUS PLUS
%left DIV STAR
%right NOT
%left DOT LB RB LP RP

%%
Program:	ExtDefList
	;
ExtDefList:	ExtDef ExtDefList
	|		/* Empty */
	;
ExtDef:		Specifier ExtDecList SEMI
	|		Specifier SEMI
	|		Specifier FunDec CompSt
	|		error SEMI
	;
ExtDecList:	VarDec
	|		VarDec COMMA ExtDecList
	;
Specifier:	TYPE
	|		StructSpecifier
	;
StructSpecifier:	STRUCT OptTag LC DefList RC
	|		STRUCT Tag
	;
OptTag:		ID
	|		/* Empty */
	;
Tag:		ID
	;
VarDec:		ID
	|		VarDec LB INT RB
	;
FunDec:		ID LP VarList RP
	|		ID LP RP
	;
VarList:	ParamDec COMMA VarList
	|		ParamDec
	;
ParamDec:	Specifier VarDec
	;
CompSt:		LC DefList StmtList RC
	|		error RC
	;
StmtList:	Stmt StmtList
	|		/* Empty */
	;
Stmt:		Exp SEMI
	|		CompSt
	|		RETURN Exp SEMI
	|		IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
	|		IF LP Exp RP Stmt ELSE Stmt
	|		WHILE LP Exp RP Stmt
	|		error SEMI
	;
DefList:	Def DefList
	|		/* Empty */
	;
Def:		Specifier DecList SEMI
	|		error SEMI
	;
DecList:	Dec
	|		Dec COMMA DecList
	;
Dec:		VarDec
	|		VarDec ASSIGNOP Exp
	;
Exp:		Exp ASSIGNOP Exp
	|		Exp AND Exp
	|		Exp OR Exp
	|		Exp RELOP Exp
	|		Exp PLUS Exp
	|		Exp MINUS Exp
	|		Exp STAR Exp
	|		Exp DIV Exp
	|		LP Exp RP
	|		MINUS Exp
	|		NOT Exp
	|		ID LP Args RP
	|		ID LP RP
	|		Exp LB Exp RB
	|		Exp DOT ID
	|		ID
	|		INT
	|		FLOAT
	|		error RP
	;
Args:		Exp COMMA Args
	|		Exp
	;

%%
