#ifndef __AST_H__
#define __AST_H__

/*
 * Define abstract syntax tree struct and operation
 * */

/* relation op type */
enum relop_type {
	GT, LT, GE, LE, EQ, NE
};

typedef enum relop_type RELOP_TYPE;

/* type of ast's node */
enum ast_node_type {
	ID_T, TYPE_T, INT_T, FLOAT_T, RELOP_T, LEX_T, SYN_T
};

typedef enum ast_node_type AST_NODE_TYPE;

/* structure of ast's node */
struct ast_node {
	char* name;								//node's name
	AST_NODE_TYPE type;						//ndoe's type
	int lineno;								//node's line number
	union {									//node's value
		int int_value;
		double float_value;
		char* string_value;
		RELOP_TYPE relop_value;
	};
	struct ast_node* parent;
	struct ast_node* first_child, *last_child;
	struct ast_node* neighbor;
	int children_num;
};

typedef struct ast_node AST_NODE;

AST_NODE* ast_root;

int is_err;

AST_NODE* ast_new_node(int lineno, char* name);
AST_NODE* ast_create_node(int lineno, char* name, AST_NODE_TYPE type, char* value);
//AST_NODE* ast_create_node(int lineno, char* name, AST_NODE_TYPE type);
void ast_append_child(AST_NODE* parent, AST_NODE* child);
void ast_append_children(AST_NODE* parent, int n, ...);

void ast_print_node(AST_NODE* current);
void ast_print_tree(AST_NODE* root, int space);
RELOP_TYPE str_to_relop_type(const char* str);
#endif 
