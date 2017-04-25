#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "symbol_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 0x3fff

Item* hashtable[TABLE_SIZE];

void table_init();

unsigned int hash_pjw(char* name);

void table_put(FieldList* item);

FieldList* table_get(char* name);

void table_print(int key);


#endif