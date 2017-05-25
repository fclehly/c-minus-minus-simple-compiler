#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "symbol_type.h"
#include "symbol_table.h"
#include "st.h"


enum semantic_error_type {
    UndefinedVariable = 1, UndefinedFunction, RedefinedVariable, RedefinedFunction,
    TypeMismatchedAssignment, LeftHandNotVariable, TypeMismatchedOperands,
    TypeMismatchedReturn, FunctionNotApplicable, IdNotArray, IdNotFunction, 
    ArrayKeyNotInteger, IllegalUseDot, NonExistentField, RedefinedField, 
    DuplicatedStructureName, UndefinedStructure,
};

typedef enum semantic_error_type SemanticErrorType;

void semantic_analysis(ST_NODE* root);
int node_parse(ST_NODE* head);
void traverse(ST_NODE* root);

void sematic_error(int error_type, int lineno, char* msg);
int type_equals(Type* type1, Type* type2);
Type* array_type(Type* type);

void extdef_parser(ST_NODE* head);
Type* specifier_parser(ST_NODE* head);
Type* structspecifier_parser(ST_NODE* head);
char* opttag_parser(ST_NODE* head);
FieldList* deflist_parser(ST_NODE* head, int isInStructure);
FieldList* def_parser(ST_NODE* head, int isInStructure);
FieldList* declist_parser(ST_NODE* head, Type* type, int isInStructure);
FieldList* dec_parser(ST_NODE* head, Type* type, int isInStructure);
FieldList* vardec_parser(ST_NODE* head, Type* type, int isInStructure); //c
void extdeclist_parser(ST_NODE* head, Type* type);
FieldList* fundec_parser(ST_NODE* head, Type* retType);
FieldList* varlist_parser(ST_NODE* head);
FieldList* paramdec_parser(ST_NODE* head);
void compst_parser(ST_NODE* head, Type* retType);
void stmtlist_parser(ST_NODE* head, Type* retType);
void stmt_parser(ST_NODE* head, Type* retTyep);
Type* exp_parser(ST_NODE* head);
FieldList* args_parser(ST_NODE* head);


void add_read();
void add_write();


#endif