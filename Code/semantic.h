#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "symbol_type.h"
#include "symbol_table.h"
#include "st.h"

void semantic_analysis(ST_NODE* root);
void traverse(ST_NODE* root);

void sematic_error(int error_type, int lineno, char* msg);

void extdef_parser(ST_NODE* head);
void specifier_parser(ST_NODE* head);
void structspecifier_parser(ST_NODE* head);
void opttag_parser(ST_NODE* head);
void deflist_parser(ST_NODE* head);
void def_parser(ST_NODE* head);
void declist_parser(ST_NODE* head);
void dec_parser(ST_NODE* head);
void vardec_parser(ST_NODE* head);
void extdeclist_parser(ST_NODE* head);
void fundec_parser(ST_NODE* head);
void varlist_parser(ST_NODE* head);
void paramdec_parser(ST_NODE* head);
void compst_parser(ST_NODE* head);
void stmtlist_parser(ST_NODE* head);
void stmt_parser(ST_NODE* head);
void exp_parser(ST_NODE* head);

#endif