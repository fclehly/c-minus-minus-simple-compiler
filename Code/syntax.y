%{
#include <stdio.h>
#include "ast.h"
#include "lex.yy.c"

#define YYERROR_VERBOSE 1		//define YYERROR_VERBOSE to get syntax-error's details
//#define YYDEBUG 1
//int is_err = 0;
extern void yyerror(const char* msg);
%}
%locations
/* declared types */
%union {
	struct ast_node* type_ast_node;
}
/* declared terminal tokens */
%token <type_ast_node> COMMA SEMI STRUCT RETURN IF WHILE TYPE ELSE
%token <type_ast_node> FLOAT INT ID
%token <type_ast_node> AND OR RELOP NOT
%token <type_ast_node> MINUS PLUS DIV STAR
%token <type_ast_node> ASSIGNOP DOT LB RB LP RP LC RC

/* declared precedence */

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

/* declared non-terminal tokens */ 
%type <type_ast_node> Program ExtDefList ExtDef ExtDecList
%type <type_ast_node> Specifier StructSpecifier OptTag Tag
%type <type_ast_node> VarDec FunDec VarList ParamDec
%type <type_ast_node> CompSt StmtList Stmt
%type <type_ast_node> DefList Def DecList Dec
%type <type_ast_node> Exp Args

%%
Program:	ExtDefList	{
				ast_root = $$ = ast_create_node(@$.first_line, "Program", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	;
ExtDefList:	ExtDef ExtDefList {
				$$ = ast_create_node(@$.first_line, "ExtDefList", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	|		/* Empty */ {$$ = NULL;}
	;
ExtDef:		Specifier ExtDecList SEMI {
				$$ = ast_create_node(@$.first_line, "ExtDef", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Specifier SEMI {
				$$ = ast_create_node(@$.first_line, "ExtDef", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	|		Specifier FunDec CompSt {
				$$ = ast_create_node(@$.first_line, "ExtDef", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		error SEMI	{}
	;
ExtDecList:	VarDec {
				$$ = ast_create_node(@$.first_line, "ExtDecList", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		VarDec COMMA ExtDecList {
				$$ = ast_create_node(@$.first_line, "ExtDecList", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	;
Specifier:	TYPE {
				$$ = ast_create_node(@$.first_line, "Specifier", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		StructSpecifier {
				$$ = ast_create_node(@$.first_line, "Specifier", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	;
StructSpecifier:	STRUCT OptTag LC DefList RC {
				$$ = ast_create_node(@$.first_line, "StructSpecifier", SYN_T, NULL);
				ast_append_children($$, 5, $1, $2, $3, $4, $5);
			}
	|		STRUCT Tag {
				$$ = ast_create_node(@$.first_line, "StructSpecifier", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	;
OptTag:		ID {
				$$ = ast_create_node(@$.first_line, "OptTag", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		/* Empty */ { $$ = NULL;}
	;
Tag:		ID {
				$$ = ast_create_node(@$.first_line, "Tag", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	;
VarDec:		ID {
				$$ = ast_create_node(@$.first_line, "VarDec", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		VarDec LB INT RB {
				$$ = ast_create_node(@$.first_line, "VarDec", SYN_T, NULL);
				ast_append_children($$, 4, $1, $2, $3, $4);
			}
	;
FunDec:		ID LP VarList RP {
				$$ = ast_create_node(@$.first_line, "FunDec", SYN_T, NULL);
				ast_append_children($$, 4, $1, $2, $3, $4);
			}
	|		ID LP RP {
				$$ = ast_create_node(@$.first_line, "FunDec", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		error RP	{}
	;
VarList:	ParamDec COMMA VarList {
				$$ = ast_create_node(@$.first_line, "VarList", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		ParamDec {
				$$ = ast_create_node(@$.first_line, "VarList", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	;
ParamDec:	Specifier VarDec {
				$$ = ast_create_node(@$.first_line, "ParamDec", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	;
CompSt:		LC DefList StmtList RC {
				$$ = ast_create_node(@$.first_line, "CompSt", SYN_T, NULL);
				ast_append_children($$, 4, $1, $2, $3, $4);
			}
	|		error RC	{}
	;
StmtList:	Stmt StmtList {
				$$ = ast_create_node(@$.first_line, "StmtList", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	|		/* Empty */ { $$ = NULL; }
	;
Stmt:		Exp SEMI {
				$$ = ast_create_node(@$.first_line, "Stmt", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	|		CompSt {
				$$ = ast_create_node(@$.first_line, "Stmt", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		RETURN Exp SEMI {
				$$ = ast_create_node(@$.first_line, "Stmt", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
				$$ = ast_create_node(@$.first_line, "Stmt", SYN_T, NULL);
				ast_append_children($$, 5, $1, $2, $3, $4, $5);
			}
	|		IF LP Exp RP Stmt ELSE Stmt {
				$$ = ast_create_node(@$.first_line, "Stmt", SYN_T, NULL);
				ast_append_children($$, 7, $1, $2, $3, $4, $5, $6, $7);
			}
	|		WHILE LP Exp RP Stmt {
				$$ = ast_create_node(@$.first_line, "Stmt", SYN_T, NULL);
				ast_append_children($$, 5, $1, $2, $3, $4, $5);
			}
	|		error SEMI	{}
	;
DefList:	Def DefList {
				$$ = ast_create_node(@$.first_line, "DefList", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	|		/* Empty */ { $$ = NULL; }
	;
Def:		Specifier DecList SEMI {
				$$ = ast_create_node(@$.first_line, "Def", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Specifier error SEMI {}
	;
DecList:	Dec {
				$$ = ast_create_node(@$.first_line, "DecList", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		Dec COMMA DecList {
				$$ = ast_create_node(@$.first_line, "DecList", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	;
Dec:		VarDec {
				$$ = ast_create_node(@$.first_line, "Dec", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		VarDec ASSIGNOP Exp {
				$$ = ast_create_node(@$.first_line, "Dec", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	;
Exp:		Exp ASSIGNOP Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp AND Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp OR Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp RELOP Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp PLUS Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp MINUS Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp STAR Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp DIV Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		LP Exp RP {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		MINUS Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	|		NOT Exp {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 2, $1, $2);
			}
	|		ID LP Args RP {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 4, $1, $2, $3, $4);
			}
	|		ID LP RP {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp LB Exp RB {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 4, $1, $2, $3, $4);
			}
	|		Exp DOT ID {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		ID {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		INT {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		FLOAT {
				$$ = ast_create_node(@$.first_line, "Exp", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	|		LP error RP	{}
	|		Exp LB error RB	{}
	;
Args:		Exp COMMA Args {
				$$ = ast_create_node(@$.first_line, "Args", SYN_T, NULL);
				ast_append_children($$, 3, $1, $2, $3);
			}
	|		Exp {
				$$ = ast_create_node(@$.first_line, "Args", SYN_T, NULL);
				ast_append_children($$, 1, $1);
			}
	;

%%

void yyerror(const char* msg) {
	fprintf(stderr, "Error type B at Line %d: %s.\n", yylineno, msg);
	is_err = 1;
}

