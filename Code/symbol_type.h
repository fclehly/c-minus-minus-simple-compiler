#ifndef __SYMBOL_TYPE_H__
#define __SYMBOL_TYPE_H__

#define BASIC_INT 0
#define BASIC_FLOAT 1

#include "irc.h"


typedef struct Type_ Type;
typedef struct FieldList_ FieldList;
typedef struct Item_ Item;

enum id_kind {
	BASIC, ARRAY, STRUCTURE, FUNCTION
};

typedef enum id_kind ID_KIND;


struct Type_ {
	ID_KIND kind;
	union {
		int basic;
		struct {
			Type* elem;
			int size;
		} array;
		FieldList* structure;
		struct {
			int num;
			FieldList* params;
			Type* ret;
		} func;
	} u;
};

struct FieldList_ {
	char* name;
	int inStructure;
	Type* type;

	Operand* op;

	FieldList* tail;
};

struct Item_ {
	FieldList* value;
	Item* next;
};

	


#endif