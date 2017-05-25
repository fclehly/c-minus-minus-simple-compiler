#ifndef __IRC_H__
#define __IRC_H__

#include <stdio.h>
#include "st.h"

int t_max, v_max, l_max;

enum op_kind {
    OP_VARIABLE, OP_CONSTANT, OP_ADDRESS, OP_LABEL, OP_FUNCTION, OP_TEMP
};

enum irc_kind {
    IRC_ASSIGN, IRC_ADD, IRC_SUB, IRC_MUL, IRC_DIV,
    IRC_FUNCTION, IRC_CALL, IRC_DEC, IRC_RETURN,
    IRC_IFGOTO, IRC_GOTO, IRC_LABEL,
    IRC_READ, IRC_WRITE, IRC_PARAM, IRC_ARG
};

typedef enum op_kind OpKind;

typedef enum irc_kind IrcKind;

typedef struct Operand_ Operand;

struct Operand_ {
    OpKind kind;
    union {
        int var_no;
        int constant;
        char* func_name;
    } u;
};

typedef struct InterCode InterCode;

struct InterCode {
    IrcKind kind;
    union {
        //assign, call, 
        struct {
            Operand* left, *right;
        } assign;
        //dec
        struct {
            Operand* op;
            int size;
        } dec;
        //if goto
        struct {
            RELOP_TYPE relop;
            Operand* op1, *op2, *label;
        } ifgoto;
        //return, lable, read, write, goto, param, arg, function
        struct {
            Operand* op;
        } singleop;
        //add, sub, mul, div
        struct {
            Operand* result, *op1, *op2;
        } binop;

    } u;
};

typedef struct InterCodes_ CodeList;

struct InterCodes_ {
    InterCode* code;
    CodeList* prev, *next;
};

CodeList* ircodes_head;
CodeList* ircodes_tail;

Operand* create_operand(OpKind kind);
InterCode* create_intercode(IrcKind kind);
CodeList* create_codelist();


void ircodes_append(CodeList* codes);
void ircodes_init();
void ir_generate(ST_NODE* head);
void ir_output(FILE* fp);
void op_output(FILE* fp, Operand* op);
void relop_output(FILE* fp, RELOP_TYPE relop);
void irc_traverse(ST_NODE* head);

void codelist_append(CodeList* list1, CodeList* list2);

Operand* new_temp();
Operand* new_label();
Operand* new_var();

CodeList* translate_exp(ST_NODE* head, Operand* place);
CodeList* translate_stmt(ST_NODE* head);
CodeList* translate_cond(ST_NODE* head, Operand* label_true, Operand* label_false);
CodeList* translate_args(ST_NODE* head);
CodeList* translate_funcdec(ST_NODE* head);


#endif