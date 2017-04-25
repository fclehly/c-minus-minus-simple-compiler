#ifndef __SYMBOL_TYPE_H__
#define __SYMBOL_TYPE_H__

#define BASIC_INT 0
#define BASIC_FLOAT 1


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
		Type* ret;
	} u;
};

struct FieldList_ {
	char* name;
	Type* type;
	FieldList* tail;
};

struct Item_ {
	FieldList* value;
	Item* next;
};


#endif