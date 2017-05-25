#include <stdio.h>
#include "st.h"
#include "syntax.tab.h"
#include "semantic.h"
#include "irc.h"

// #define PRINT_TREE

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
	
	FILE* f1 = fopen(argv[1], "r");
	if (!f1) {
		perror(argv[1]);
		return 1;
	}
	FILE* f2 = stdout;
	if (argc >= 3) {
		f2 = fopen(argv[2], "w");
	}
	yyrestart(f1);
//	yydebug = 1;
	yyparse();
	if (!is_err) {
#ifdef PRINT_TREE
//lab1
		st_print_tree(st_root, 0);
		printf("\n\n");
#endif
//lab2
		semantic_analysis(st_root);

//lab3
		ir_generate(st_root);
		ir_output(f2);
		if (argc >= 3) {
			fclose(f2);
		}
		

	}
	
	return 0;
}
