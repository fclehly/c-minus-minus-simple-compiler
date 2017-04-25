#include "semantic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void semantic_analysis(ST_NODE* root) {
	table_init();
	traverse(root);
}

void node_parse(ST_NODE* head) {
	if (head->name != NULL) {
			if (head->type == Def_T) {
				def_parser(head);
			}
			else if (head->type == ExtDef_T) {
				extdef_parser(head);
			}
			else if (head->type == Exp_T) {
				exp_parser(head);
			}
		}
}

void traverse(ST_NODE* root) {
	if (root) {
		node_parse(root);
		traverse(root->first_child);
		traverse(root->neighbor);
	}
}

void semantic_error(int error_type, int lineno, char* msg, char* token) {
	printf("Error type %d at Line %d: %s: \"%s\"\n", error_type, lineno, msg, token);
}



void extdef_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	specifier_parser(child);
	child = child->neighbor;
	if (child->type == ExtDecList_T) {
		extdeclist_parser(child);
	}
	else if (child->type == FunDec_T) {
		fundec_parser(child);
		child = child->neighbor;
		compst_parser(child);
	}
}

void specifier_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	if (child->type == TYPE_T) {

	}
	else if (child->type == StructSpecifier_T) {
		structspecifier_parser(child);
	}
}

void structspecifier_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	child = child->neighbor;
	if (child->type == OptTag_T) {
		opttag_parser(child);
		child = child->neighbor->neighbor;
		deflist_parser(child);

	}
	else if (child->type == Tag_T) {
		ST_NODE* child = child->first_child;
		FieldList* field = table_get(child->name);
		if (field == NULL) {

		}
		else {
			semantic_error(DuplicatedStructureName, child->lineno, "Duplicated name", child->string_value);
		}
	}
}

void opttag_parser(ST_NODE* head) {
	if (head != NULL) {
		ST_NODE* child = head->first_child;
		if (child != NULL && child->type == ID_T) {
			FieldList* field = table_get(child->name);
			if (field == NULL) {

			}
			else {
				semantic_error(DuplicatedStructureName, child->lineno, "Duplicated name", child->string_value);
			}
			
		}
	}
}

void deflist_parser(ST_NODE* head) {
	if (head != NULL) {
		ST_NODE* child = head->first_child;
		if (child != NULL) {
			def_parser(child);
			child = child->neighbor;
			deflist_parser(child);
		}
	}
}

void def_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	specifier_parser(child);
	child = child->neighbor;
	declist_parser(child);

}

void declist_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	dec_parser(child);
	child = child->neighbor;
	if (child != NULL) {
		child = child->neighbor;
		declist_parser(child);

	}
}

void dec_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	vardec_parser(child);
	child = child->neighbor;
	if (child != NULL) {
		child = child->neighbor;
		exp_parser(child);
	}
}

void vardec_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	if (child->type == ID_T) {

	}
	else if (child->type == VarDec_T) {
		vardec_parser(child);
	}
}

void extdeclist_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	vardec_parser(child);
	child = child->neighbor;
	if (child != NULL) {
		child = child->neighbor;
		extdeclist_parser(child);
	}
}

void fundec_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	if (child->type == ID_T) {

	}
	child = child->neighbor->neighbor;
	if (child->type == RP_T) {

	}
	else if (child->type == VarList_T) {
		varlist_parser(child);
	}
}

void varlist_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	paramdec_parser(child);
	child = child->neighbor;
	if (child != NULL) {
		child = child->neighbor;
		varlist_parser(child);
	}
}

void paramdec_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	specifier_parser(child);
	child = child->neighbor;
	vardec_parser(child);
}

void compst_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	child = child->neighbor;
	deflist_parser(child);
	child = child->neighbor;
	stmtlist_parser(child);
}

void stmtlist_parser(ST_NODE* head) {
	if (head != NULL) {
		ST_NODE* child = head->first_child;
		if (child != NULL) {
			stmt_parser(child);
			child = child->neighbor;
			stmtlist_parser(child);
		}
	}
}

void stmt_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	if (child->type == Exp_T) {
		exp_parser(child);

	}
	else if (child->type == CompSt_T) {
		compst_parser(child);
	}
	else if (child->type == RETURN_T) {
		child = child->neighbor;
		exp_parser(child);
	}
	else if (child->type == IF_T) {
		child = child->neighbor->neighbor;
		exp_parser(child);
		child = child->neighbor->neighbor;
		stmt_parser(child);
		child = child->neighbor;
		if (child != NULL) {
			child = child->neighbor;
			stmt_parser(child);
		}
	}
	else if (child->type == WHILE_T) {
		child = child->neighbor->neighbor;
		exp_parser(child);
		child = child->neighbor->neighbor;
		stmt_parser(child);
	}
}

void exp_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	if (child->type == Exp_T) {
		exp_parser(child);
		child = child->neighbor->neighbor;
		if (child) {
			if (child->type == Exp_T) {
				exp_parser(child);
			}
			else if (child->type == ID_T) {
				if (table_get(child->name) == NULL) {
					semantic_error(UndefinedVariable, child->lineno, "Undefined Variable", child->string_value);
				}
			}
		}
		
	}
	else if (child->type == ID_T) {
		FieldList* field = table_get(child->name);
		if (field == NULL) {
			semantic_error(UndefinedVariable, child->lineno, "Undefined Variable", child->string_value);
		}
		child = child->neighbor->neighbor;
		if (child->type == Args_T) {
			args_parser(child);
		}

	}
	else if ((child->type == LP_T || child->type == MINUS_T || child->type == LP_T) && child->neighbor->type == Exp_T ) {
		child = child->neighbor;
		exp_parser(child);
	}
}

void args_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	exp_parser(child);
	child = child->neighbor;
	if (child != NULL) {
		child = child->neighbor;
		args_parser(child);
	}
}