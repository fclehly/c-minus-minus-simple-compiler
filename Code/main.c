#include <stdio.h>
#include "syntax.tab.h"
extern int yyparse();
extern void yyrestart(FILE* input_file);
extern FILE* yyin;
int yyerror(char* msg) {
	fprintf(stderr, "error: %s\n", msg);
	return 0;
}
int main(int argc, char** argv) {
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
	return 0;
}
