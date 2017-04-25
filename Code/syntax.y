%{
#include <stdio.h>
#include "st.h"
#include "lex.yy.c"

#define YYERROR_VERBOSE 1		//define YYERROR_VERBOSE to get syntax-error's details
//#define YYDEBUG 1
//int is_err = 0;
extern void yyerror(const char* msg);
%}
%locations
/* declared types */
%union {
	struct st_node* type_st_node;
}
/* declared terminal tokens */
%token <type_st_node> COMMA SEMI STRUCT RETURN IF WHILE TYPE ELSE
%token <type_st_node> FLOAT INT ID
%token <type_st_node> AND OR RELOP NOT
%token <type_st_node> MINUS PLUS DIV STAR
%token <type_st_node> ASSIGNOP DOT LB RB LP RP LC RC

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
%type <type_st_node> Program ExtDefList ExtDef ExtDecList
%type <type_st_node> Specifier StructSpecifier OptTag Tag
%type <type_st_node> VarDec FunDec VarList ParamDec
%type <type_st_node> CompSt StmtList Stmt
%type <type_st_node> DefList Def DecList Dec
%type <type_st_node> Exp Args

%%
Program:	ExtDefList	{
				st_root = $$ = st_create_node(@$.first_line, "Program", Program_T, NULL);
				st_append_children($$, 1, $1);
			}
	;
ExtDefList:	ExtDef ExtDefList {
				$$ = st_create_node(@$.first_line, "ExtDefList", ExtDefList_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	|		/* Empty */ {$$ = NULL;}
	;
ExtDef:		Specifier ExtDecList SEMI {
				$$ = st_create_node(@$.first_line, "ExtDef", ExtDef_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Specifier SEMI {
				$$ = st_create_node(@$.first_line, "ExtDef", ExtDef_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	|		Specifier FunDec CompSt {
				$$ = st_create_node(@$.first_line, "ExtDef", ExtDef_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		error SEMI	{}
	;
ExtDecList:	VarDec {
				$$ = st_create_node(@$.first_line, "ExtDecList", ExtDecList_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		VarDec COMMA ExtDecList {
				$$ = st_create_node(@$.first_line, "ExtDecList", ExtDecList_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	;
Specifier:	TYPE {
				$$ = st_create_node(@$.first_line, "Specifier", Specifier_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		StructSpecifier {
				$$ = st_create_node(@$.first_line, "Specifier", Specifier_T, NULL);
				st_append_children($$, 1, $1);
			}
	;
StructSpecifier:	STRUCT OptTag LC DefList RC {
				$$ = st_create_node(@$.first_line, "StructSpecifier", StructSpecifier_T, NULL);
				st_append_children($$, 5, $1, $2, $3, $4, $5);
			}
	|		STRUCT Tag {
				$$ = st_create_node(@$.first_line, "StructSpecifier", StructSpecifier_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	;
OptTag:		ID {
				$$ = st_create_node(@$.first_line, "OptTag", OptTag_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		/* Empty */ { $$ = NULL;}
	;
Tag:		ID {
				$$ = st_create_node(@$.first_line, "Tag", Tag_T, NULL);
				st_append_children($$, 1, $1);
			}
	;
VarDec:		ID {
				$$ = st_create_node(@$.first_line, "VarDec", VarDec_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		VarDec LB INT RB {
				$$ = st_create_node(@$.first_line, "VarDec", VarDec_T, NULL);
				st_append_children($$, 4, $1, $2, $3, $4);
			}
	;
FunDec:		ID LP VarList RP {
				$$ = st_create_node(@$.first_line, "FunDec", FunDec_T, NULL);
				st_append_children($$, 4, $1, $2, $3, $4);
			}
	|		ID LP RP {
				$$ = st_create_node(@$.first_line, "FunDec", FunDec_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		error RP	{}
	;
VarList:	ParamDec COMMA VarList {
				$$ = st_create_node(@$.first_line, "VarList", VarList_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		ParamDec {
				$$ = st_create_node(@$.first_line, "VarList", VarList_T, NULL);
				st_append_children($$, 1, $1);
			}
	;
ParamDec:	Specifier VarDec {
				$$ = st_create_node(@$.first_line, "ParamDec", ParamDec_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	;
CompSt:		LC DefList StmtList RC {
				$$ = st_create_node(@$.first_line, "CompSt", CompSt_T, NULL);
				st_append_children($$, 4, $1, $2, $3, $4);
			}
	|		error RC	{}
	;
StmtList:	Stmt StmtList {
				$$ = st_create_node(@$.first_line, "StmtList", StmtList_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	|		/* Empty */ { $$ = NULL; }
	;
Stmt:		Exp SEMI {
				$$ = st_create_node(@$.first_line, "Stmt", Stmt_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	|		CompSt {
				$$ = st_create_node(@$.first_line, "Stmt", Stmt_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		RETURN Exp SEMI {
				$$ = st_create_node(@$.first_line, "Stmt", Stmt_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
				$$ = st_create_node(@$.first_line, "Stmt", Stmt_T, NULL);
				st_append_children($$, 5, $1, $2, $3, $4, $5);
			}
	|		IF LP Exp RP Stmt ELSE Stmt {
				$$ = st_create_node(@$.first_line, "Stmt", Stmt_T, NULL);
				st_append_children($$, 7, $1, $2, $3, $4, $5, $6, $7);
			}
	|		WHILE LP Exp RP Stmt {
				$$ = st_create_node(@$.first_line, "Stmt", Stmt_T, NULL);
				st_append_children($$, 5, $1, $2, $3, $4, $5);
			}
	|		error SEMI	{}
	;
DefList:	Def DefList {
				$$ = st_create_node(@$.first_line, "DefList", Stmt_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	|		/* Empty */ { $$ = NULL; }
	;
Def:		Specifier DecList SEMI {
				$$ = st_create_node(@$.first_line, "Def", Def_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Specifier error SEMI {}
	;
DecList:	Dec {
				$$ = st_create_node(@$.first_line, "DecList", DecList_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		Dec COMMA DecList {
				$$ = st_create_node(@$.first_line, "DecList", DecList_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	;
Dec:		VarDec {
				$$ = st_create_node(@$.first_line, "Dec", Dec_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		VarDec ASSIGNOP Exp {
				$$ = st_create_node(@$.first_line, "Dec", Dec_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	;
Exp:		Exp ASSIGNOP Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp AND Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp OR Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp RELOP Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp PLUS Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp MINUS Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp STAR Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp DIV Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		LP Exp RP {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		MINUS Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	|		NOT Exp {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 2, $1, $2);
			}
	|		ID LP Args RP {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 4, $1, $2, $3, $4);
			}
	|		ID LP RP {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp LB Exp RB {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 4, $1, $2, $3, $4);
			}
	|		Exp DOT ID {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		ID {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		INT {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		FLOAT {
				$$ = st_create_node(@$.first_line, "Exp", Exp_T, NULL);
				st_append_children($$, 1, $1);
			}
	|		LP error RP	{}
	|		Exp LB error RB	{}
	;
Args:		Exp COMMA Args {
				$$ = st_create_node(@$.first_line, "Args", Args_T, NULL);
				st_append_children($$, 3, $1, $2, $3);
			}
	|		Exp {
				$$ = st_create_node(@$.first_line, "Args", Args_T, NULL);
				st_append_children($$, 1, $1);
			}
	;

%%

void yyerror(const char* msg) {
	printf("Error type B at Line %d: %s.\n", yylineno, msg);
	is_err = 1;
}

