#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "st.h"
//extern char* strdup(char* s);

ST_NODE* st_new_node(int lineno, char* name) {
	ST_NODE* node = (ST_NODE*) malloc(sizeof(ST_NODE));
	node->lineno = lineno;
	node->name = (char*) malloc(sizeof(name));
	strcpy(node->name, name);
	node->parent = NULL;
	node->lst_child = node->first_child = NULL;
	node->neighbor = NULL;
	node->children_num = 0;
	return node;
}
/*
ST_NODE* st_create_node(int lineno, char* name, ST_NODE_TYPE type) {
	ST_NODE* node = st_new_node(lineno, name);
	node->type = type;
	return node;
}*/

ST_NODE* st_create_node(int lineno, char* name, ST_NODE_TYPE type, char* value) {
	ST_NODE* node = st_new_node(lineno, name);
	node->type = type;
	//printf("%d %s %d %s\n", lineno, name, type, value);
	if (value != NULL) {
		switch (type) {
			case ID_T:
				//node->string_value = strdup(value);
				node->string_value = (char*) malloc(sizeof(value));
				strcpy(node->string_value, value);
				break;
			case INT_T:
				node->int_value = atoi(value);
				break;	
			case FLOAT_T:
				node->float_value = atof(value);
				break;
			case TYPE_T:
				//node->string_value = strdup(value);
				node->string_value = (char*) malloc(sizeof(value));
				strcpy(node->string_value, value);
				break;
			case RELOP_T:
				node->relop_value = str_to_relop_type(value);
			// case LEX_T:
			// 	//node->string_value = value;
			// 	break;
			default:
				// fprintf(stderr, "Error while creating lexical node: %s %s", name, value);
				break;
		}
	}
	return node;
}

void st_append_child(ST_NODE* parent, ST_NODE* child) {
	if (child != NULL) {
		child->parent = parent;
		if (parent->first_child == NULL) {
			parent->lst_child = parent->first_child = child;
		}
		else {
			parent->lst_child = parent->lst_child->neighbor = child;
		}
		parent->children_num++;
	}
}

void st_append_children(ST_NODE* parent, int n, ...) {
	va_list children;
	va_start(children, n);
	int i;
	for (i = 0; i < n; i++) {
		ST_NODE* node = va_arg(children, ST_NODE*);
		st_append_child(parent, node);
	}
}

void st_print_node(ST_NODE* current) {
	if (current != NULL) {
		printf("%s", current->name);
		switch (current->type) { 
			case ID_T:
				printf(": %s", current->string_value);
				break;
			case FLOAT_T:
				printf(": %lf", current->float_value);
				break;
			case INT_T:
				printf(": %d", current->int_value);
				break;
			case TYPE_T:
				printf(": %s", current->string_value);
			   break;
			default:
				if (current->type >= Program_T) {
					printf(" (%d)", current->lineno);
				}
			   break;
		}
		printf("\n");
	}
}

//static int space = 0;

void st_print_tree(ST_NODE* root, int space) {
	if (root != NULL) {
		int i;
		for (i = 0; i < space; i++) {
			printf("  ");
		}
		st_print_node(root);
		ST_NODE* p = root->first_child;
		space++;
		while (p) {
			st_print_tree(p, space);
			p = p->neighbor;
		}
		space--;
	}
}


RELOP_TYPE str_to_relop_type(const char* str) {
	if (!strcmp("<", str)) {
		return LT;	
	}
	else if (!strcmp(">", str)) {
		return GT;	
	}
	else if (!strcmp(">=", str)) {
		return GE;	
	}
	else if (!strcmp("<=", str)) {
		return LE;	
	}
	else if (!strcmp("==", str)) {
		return EQ;	
	}
	else if (!strcmp("!=", str)) {
		return NE;	
	} else {
		fprintf(stderr, "Error while transform string to relop type: %s", str);
		return 0;
	}
}
