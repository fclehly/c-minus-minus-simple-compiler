#ifndef __AST_H__
#define __AST_H__

struct ast_node {
	char* string;
	int lineno;
	union {
		int int_value;
		double float_value;
		char* string_value;
	}
	ast_node* parent;
	ast_node* first_child;
	ast_node* neighbor;
	int children_num;
};

typedef struct ast_node AST_NODE;

AST_NODE* ast_new_node(int lineno, char* str);
void ast_append_node(AST_NODE* parent, AST_NODE* child);
void ast_print_node(AST_NODE* current);
void ast_print_tree(AST_NODE* root);
#endif 
