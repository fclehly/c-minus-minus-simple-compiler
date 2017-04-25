#include "semantic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void semantic_analysis(ST_NODE* root) {
	table_init();
	traverse(root);
}

void traverse(ST_NODE* root) {
	if (root) {
		traverse(root->first_child);
		traverse(root->neighbor);
		if (root->name != NULL) {
			if () {
				def_parser(root);
			}
			else if (!strcmp(root->name, "ExtDef")) {
				extdef_parser(root);
			}
			else if (!strcmp(root->name, "Exp")) {
				exp_parser(root);
			}
		}
	}
}

void semantic_error(int error_type, int lineno, char* msg) {
	printf("Error type %d at Line %d: %s\n", error_type, lineno, msg);
}



void extdef_parser(ST_NODE* head) {}

void specifier_parser(ST_NODE* head) {}

void structspecifier_parser(ST_NODE* head) {}

void opttag_parser(ST_NODE* head) {}

void deflist_parser(ST_NODE* head) {}

void def_parser(ST_NODE* head) {}

void declist_parser(ST_NODE* head) {}

void dec_parser(ST_NODE* head) {}

void vardec_parser(ST_NODE* head) {}

void extdeclist_parser(ST_NODE* head) {}

void fundec_parser(ST_NODE* head) {}

void varlist_parser(ST_NODE* head) {}

void paramdec_parser(ST_NODE* head) {}

void compst_parser(ST_NODE* head) {}

void stmtlist_parser(ST_NODE* head) {}

void stmt_parser(ST_NODE* head) {}

void exp_parser(ST_NODE* head) {
	// ST_NODE* child = head->first_child;
	// if (!strcmp(child->name, "Exp")) {
	// 	exp_parser(child);
	// 	child = child->neighbor->neighbor;
	// 	if (child) {
	// 		if (!strcmp(child->name, "Exp")) {
	// 			exp_handler(child);
	// 		}
	// 		else if (!strcmp(child->name, "ID")) {
	// 			if (table_get())
	// 		}
	// 	}
		
	// }
}