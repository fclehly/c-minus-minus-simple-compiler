#include <stdio.h>
#include "ast.h"
#include "syntax.tab.h"
extern int yyparse();
extern void yyrestart(FILE* input_file);
extern FILE* yyin;
extern int yylineno;
//int is_error = 0;
/*
int yyerror(const char* msg) {
	fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
	is_err = 1;
	return 1;
}
*/
int main(int argc, char** argv) {
	is_err = 0;
	if (argc <= 1) {
		return 1;
	}
	
	FILE* f = fopen(argv[1], "r");
	if (!f) {
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
//	yydebug = 1;
	yyparse();
	if (!is_err) {
		ast_print_tree(ast_root, 0);
	}
	
	return 0;
}
