//#include "sumbol_type.h"
#include "symbol_table.h"


void table_init() {
    int i;
    for (i = 0; i < TABLE_SIZE; ++i) {
        hashtable[i] = NULL;
    }
}

unsigned int hash_pjw(char* name) {
	unsigned int val = 0, i;
	for (; *name; ++name){
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val;
}

void table_put(FieldList* item) {
    if (item != NULL) {
        int key = hash_pjw(item->name);
        Item* pItem = (Item*) malloc(sizeof(Item));
        pItem->value = item;
        pItem->next = NULL;
        if (hashtable[key] == NULL) {
			//printf("null %d\n", key);
            hashtable[key] = pItem;
        }
        else {
			//printf("not null %d\n", key);
            Item* head = hashtable[key];
            while (head->next) {
                head = head->next;
            }
            head->next = pItem;
        }
    }

}

FieldList* table_get(char* name) {
    if (name) {
        int key = hash_pjw(name);
        if (!hashtable[key]) {
            return NULL;
        }
        Item* head = hashtable[key];
        while (head) {
            if (!strcmp(hashtable[key]->value->name, name)) {
                return head->value;
            }
            head = head->next;
        }
    }
    return NULL;
}

void table_print(int key) {
    Item* head = hashtable[key];
	printf("table[%d]:\n", key);
    if (head) {
		int count = 0;
        while (head) {
            printf("%d %s\n", count, head->value->name);
			count++;
            head = head->next;
        }
    }
    else {
        printf("null\n");
    }
}