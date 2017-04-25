#ifndef __ST_H__
#define __ST_H__

/*
 * Define syntax tree struct and operation
 * */

/* relation op type */
enum relop_type {
	GT, LT, GE, LE, EQ, NE
};

typedef enum relop_type RELOP_TYPE;

/* type of st's node */
enum st_node_type {
	INT_T, TYPE_T, FLOAT_T, ID_T, SEMI_T,
    COMMA_T, ASSIGNOP_T, RELOP_T, PLUS_T,
    MINUS_T, STAR_T, DIV_T, AND_T, OR_T,
    NOT_T, LP_T, RP_T, LB_T, RB_T,
    LC_T, RC_T, STRUCT_T, RETURN_T, IF_T,
    ELSE_T, WHILE_T, DOT_T,

    Program_T, ExtDefList_T, ExtDef_T,
    Specifier_T, ExtDecList_T, FunDec_T,
    CompSt_T, VarDec_T, StructSpecifier_T,
    OptTag_T, Tag_T, VarList_T, ParamDec_T,
    StmtList_T, Stmt_T, Exp_T, DefList_T,
    DecList_T, Dec_T, Def_T, Args_T 
};

typedef enum st_node_type ST_NODE_TYPE;

/* structure of st's node */
struct st_node {
	char* name;								//node's name
	ST_NODE_TYPE type;						//ndoe's type
	int lineno;								//node's line number
	union {									//node's value
		int int_value;
		double float_value;
		char* string_value;
		RELOP_TYPE relop_value;
	};
	struct st_node* parent;
	struct st_node* first_child, *lst_child;
	struct st_node* neighbor;
	int children_num;
};

typedef struct st_node ST_NODE;

ST_NODE* st_root;

int is_err;

ST_NODE* st_new_node(int lineno, char* name);
ST_NODE* st_create_node(int lineno, char* name, ST_NODE_TYPE type, char* value);
//ST_NODE* st_create_node(int lineno, char* name, ST_NODE_TYPE type);
void st_append_child(ST_NODE* parent, ST_NODE* child);
void st_append_children(ST_NODE* parent, int n, ...);

void st_print_node(ST_NODE* current);
void st_print_tree(ST_NODE* root, int space);
RELOP_TYPE str_to_relop_type(const char* str);

#endif 
