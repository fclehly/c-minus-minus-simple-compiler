#include "semantic.h"
// #include "symbol_type.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void semantic_analysis(ST_NODE* root) {
	table_init();
	traverse(root);
}

int node_parse(ST_NODE* head) {
	if (head->name != NULL) {
		if (head->type == Def_T) {
			def_parser(head, 0);
			return 0;
		}
		else if (head->type == ExtDef_T) {
			extdef_parser(head);
			return 0;
		}
		else if (head->type == Exp_T) {
			exp_parser(head);
			return 0;
		}
	}
	return 1;
}

void traverse(ST_NODE* root) {
	if (root) {
		int status = node_parse(root);
		if (status) {
			traverse(root->first_child);	
		}
		traverse(root->neighbor);
		
	}
}

void semantic_error(int error_type, int lineno, char* msg, char* token) {
	printf("Error type %d at Line %d: %s: \"%s\"\n", error_type, lineno, msg, token);
}

int type_equals(Type* type1, Type* type2) {
	if (type1 == NULL || type2 == NULL) {
		return 0;
	}
	else {
		if (type1->kind != type2->kind) {
			return 0;
		}
		else {
			if (type1->kind == BASIC) {
				if (type1->u.basic == type2->u.basic) {
					return 1;
				}
				else {
					return 0;
				}
			}
			else if (type1->kind == ARRAY) {
				return 1;
			}
			else if (type1->kind == STRUCTURE) {
				if (!strcmp(type1->u.structure->name, type2->u.structure->name)) {
					return 1;
				}
				else {
					FieldList* p1 = type1->u.structure;
					FieldList* p2 = type2->u.structure;
					int flag = 1;
					while (p1 && p2) {
						if (!type_equals(p1->type, p2->type)) {
							flag = 0;
							break;
						}
						p1 = p1->tail;
						p2 = p2->tail;
					}
					if ((p1 == NULL && p2 != NULL) || (p1 != NULL && p2 == NULL)) {
						flag = 0;
					}
					if (flag == 0) {
						return 0;
					}
					else {
						return 1;
					}
				}
				return 0;
			} 
			else {
				return 0;
			}
		}
	}
	return 0;
}

void logd(char* msg, int level) {
	switch (level) {
		case 3:
			printf("Error: %s\n", msg);
			break;
		case 2:
			printf("Waring: %s\n", msg);
			break;
		case 1:
			printf("Debug: %s\n", msg);
			break;
		case 0:
			printf("Info: %s\n", msg);
			break;
		default:
			// printf("no level\n");
			break;
	}
	
}



void extdef_parser(ST_NODE* head) {
	logd("parsing...extdef", LOG_I);
	ST_NODE* child = head->first_child;
	Type* type = specifier_parser(child);
	if (type != NULL) {
		child = child->neighbor;
		if (child->type == ExtDecList_T) {
			extdeclist_parser(child, type);
		}
		else if (child->type == FunDec_T) {
			FieldList* field = fundec_parser(child, type);
			if (field != NULL) {	
				child = child->neighbor;
				compst_parser(child, type);
			}
		}
		//logd("complete...extdef", LOG_D);
	}
}

Type* specifier_parser(ST_NODE* head) {
	logd("parsing...specifier", LOG_I);
	ST_NODE* child = head->first_child;
	if (child->type == TYPE_T) {
		Type* type = create_type(BASIC);
		if (!strcmp(child->string_value, "int")) {
			type->u.basic = BASIC_INT;
		}
		else {
			type->u.basic = BASIC_FLOAT;
		}
		return type;
	}
	else if (child->type == StructSpecifier_T) {
		Type* type = structspecifier_parser(child);
		return type;
	}
	return NULL;
}

Type* structspecifier_parser(ST_NODE* head) {
	logd("parsing...structspecifier", LOG_I);
	ST_NODE* child = head->first_child;
	child = child->neighbor;
	if (child->type == OptTag_T) {
		char* name = opttag_parser(child);
		child = child->neighbor->neighbor;
		// FieldList* fieldlist = create_fieldlist(name);
		FieldList* field = deflist_parser(child, 1);
		Type* type = create_type(STRUCTURE);
		type->u.structure = field;
		if (name != NULL) {
			FieldList* field1 = create_fieldlist(name);
			field1->type = type;
			table_put(field1);
			// printf("%s\n", table_get(name)->type->u.structure->tail->name);
		}
		return type;

	}
	else if (child->type == Tag_T) {
		child = child->first_child;
		FieldList* field = table_get(child->string_value);
		if (field == NULL) {
			semantic_error(UndefinedStructure, child->lineno, "Undefined structure", child->string_value);
		}
		else {
			return field->type;
			//semantic_error(DuplicatedStructureName, child->lineno, "Duplicated name", child->string_value);
		}
	}
	return NULL;
}

char* opttag_parser(ST_NODE* head) {
	if (head != NULL) {
		ST_NODE* child = head->first_child;
		if (child != NULL && child->type == ID_T) {
			FieldList* field = table_get(child->string_value);
			if (field == NULL) {
				return child->string_value;
			}
			else {
				semantic_error(DuplicatedStructureName, child->lineno, "Duplicated name", child->string_value);
			}
			
		}
	}
	return NULL;
}

FieldList* deflist_parser(ST_NODE* head, int isInStructure) {
	logd("parsing...deflist", LOG_I);
	if (head != NULL) {
		ST_NODE* child = head->first_child;
		// logd("hello", LOG_I);
		if (child != NULL) {
			if (isInStructure) {
				FieldList* field1 = def_parser(child, isInStructure);
				child = child->neighbor;
				if (child != NULL) {
					FieldList* field2 = deflist_parser(child, isInStructure);
					if (field2 != NULL) {
						field1->tail = field2;
					}
				}
				// logd("hello", LOG_I);
				return field1;
				
			}
			else {
				def_parser(child, isInStructure);
				//logd("hhh", LOG_I);
				child = child->neighbor;
				if (child != NULL) {
					deflist_parser(child, isInStructure);
				}
				//logd("aaa", LOG_I);
			}
		}

	}
	logd("complete...deflist", LOG_I);
	return NULL;
}

FieldList* def_parser(ST_NODE* head, int isInStructure) {
	ST_NODE* child = head->first_child;
	Type* type = specifier_parser(child);
	if (type != NULL) {
		child = child->neighbor;
		FieldList* field = declist_parser(child, type, isInStructure);
		if (isInStructure) {
			return field;
		}
		// logd("complete...def", LOG_D);
	}
	return NULL;
}

FieldList* declist_parser(ST_NODE* head, Type* type, int isInStructure) {
	logd("parsing...declist", LOG_I);
	ST_NODE* child = head->first_child;
	FieldList* field1 = dec_parser(child, type, isInStructure);
	child = child->neighbor;
	if (type == NULL) {
		logd("declist type null", LOG_D);
		return NULL;
	}
	if (child != NULL) {
		child = child->neighbor;
		FieldList* field2 = declist_parser(child, type, isInStructure);
		if (isInStructure && field1 != NULL) {
			field1->tail = field2;
		} else {
			return NULL;
		}
	}
	if (isInStructure) {
		return field1;
	}
	return NULL;
}

FieldList* dec_parser(ST_NODE* head, Type* type, int isInStructure) {
	logd("parsing...dec", LOG_I);
	ST_NODE* child = head->first_child;
	FieldList* field = vardec_parser(child, type, isInStructure);
	child = child->neighbor;
	if (child != NULL) {
		child = child->neighbor;
		Type* type = exp_parser(child);
		if (type->kind != field->type->kind) {
			semantic_error(TypeMismatchedAssignment, child->lineno, "Type mismatched assignment", NULL);
		}
		else {
			return field;
		}
	}
	else if (field != NULL) {
		return field;
	}
	return NULL;
}

FieldList* vardec_parser(ST_NODE* head, Type* type, int isInStructure) {
	logd("parsing...vardec", LOG_I);
	ST_NODE* child = head->first_child;
	if (type == NULL) {
		logd("vardec type null", LOG_D);
		return NULL;
	}
	if (child->type == ID_T) {
		FieldList* field = table_get(child->string_value);
		if (field != NULL) {
			if (field->inStructure == 1) {
				semantic_error(RedefinedField, child->lineno, "Redefined field", child->string_value);
			}
			else {
				semantic_error(RedefinedVariable, child->lineno, "Redefined Variable", child->string_value);
			}
			return NULL;
		}
		else {
			FieldList* field = create_fieldlist(child->string_value);
			field->type = type;
			field->inStructure = isInStructure;
			table_put(field);
			return field;
		}
	}
	else if (child->type == VarDec_T) {
		FieldList* field = vardec_parser(child, type, isInStructure);
		Type* p = field->type;
		Type* t = create_type(ARRAY);
		t->u.array.size = child->neighbor->neighbor->int_value;
		t->u.array.elem = p;
		field->type = t;
		return field;
	}
	return NULL;
}

void extdeclist_parser(ST_NODE* head, Type* type) {
	ST_NODE* child = head->first_child;
	vardec_parser(child, type, 0);
	child = child->neighbor;
	if (child != NULL) {
		child = child->neighbor;
		extdeclist_parser(child, type);
	}
}

FieldList* fundec_parser(ST_NODE* head, Type* retType) {
	logd("parsing...fundec", LOG_I);
	ST_NODE* child = head->first_child;
	if (child->type == ID_T) {
		FieldList* field1 = table_get(child->string_value);
		if (field1 != NULL) {
			semantic_error(RedefinedFunction, child->lineno, "Redefined funcition", child->string_value);
			return NULL;
		}
		else {
			if (child->neighbor->neighbor->type == RP_T) {
				logd("generating...no params function", LOG_I);
				FieldList* field = create_fieldlist(child->string_value);
				Type* type1 = create_type(FUNCTION);
				type1->u.func.ret = retType;
				type1->u.func.num = 0;
				type1->u.func.params = NULL;
				field->type = type1;
				table_put(field);
				logd("generated!", LOG_I);
				return field;
			}
			else if (child->neighbor->neighbor->type == VarList_T) {
				child = child->neighbor->neighbor;
				FieldList* fieldlist = varlist_parser(child);
				if (fieldlist != NULL) {
					FieldList* field = create_fieldlist(head->first_child->string_value);
					Type* type1 = create_type(FUNCTION);
					type1->u.func.ret = retType;
					type1->u.func.num = 1;
					type1->u.func.params = fieldlist;
					field->type = type1;
					table_put(field);
					//printf("func: %s\n", table_get(head->first_child->string_value)->type->u.func.params->tail->name);
					return field;
				}
			}
		}
	}
	return NULL;
}

FieldList* varlist_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	FieldList* field1 = paramdec_parser(child);
// /	printf("%s\n", field1->name);
	child = child->neighbor;
	if (child != NULL) {
		child = child->neighbor;
		FieldList* field2 = varlist_parser(child);
		if (field1 != NULL && field2 != NULL) {
			field1->tail = field2;
			return field1;
		}
	}
	if (field1 != NULL) {
		return field1;
	}

	return NULL;
}

FieldList* paramdec_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	Type* type = specifier_parser(child);
	if (type != NULL) {	
		child = child->neighbor;
		FieldList* field = vardec_parser(child, type, 0);
		return field;
	}
	return NULL;
}

void compst_parser(ST_NODE* head, Type* retType) {
	logd("parsing...compst", LOG_I);
	ST_NODE* child = head->first_child;
	child = child->neighbor;
	//logd("starting...parsing...compst...deflist", LOG_I);
	logd(child->name, LOG_D);
	if (child != NULL && child->type == DefList_T) {
		deflist_parser(child, 0);
		child = child->neighbor;
	}
	// logd("start...parsing compst stmtlist", LOG_I);
	stmtlist_parser(child, retType);
	//logd("complete...compst", LOG_I);
}

void stmtlist_parser(ST_NODE* head, Type* retType) {
	logd("parsing...stmtlist", LOG_I);
	if (head != NULL) {
		ST_NODE* child = head->first_child;
		if (child != NULL) {
			stmt_parser(child, retType);
			child = child->neighbor;
			if (child != NULL) {
				stmtlist_parser(child, retType);
			}
		}
	}
}

void stmt_parser(ST_NODE* head, Type* retType) {
	logd("parsing...stmt", LOG_I);
	ST_NODE* child = head->first_child;
	if (child->type == Exp_T) {
		exp_parser(child);

	}
	else if (child->type == CompSt_T) {
		compst_parser(child, retType);
	}
	else if (child->type == RETURN_T) {
		child = child->neighbor;
		Type* type1 = exp_parser(child);
		if (!type_equals(type1, retType)) {
			semantic_error(TypeMismatchedReturn, child->lineno, "Type mismatched for return", "");
		}

	}
	else if (child->type == IF_T) {
		child = child->neighbor->neighbor;
		exp_parser(child);
		child = child->neighbor->neighbor;
		stmt_parser(child, retType);
		child = child->neighbor;
		if (child != NULL) {
			child = child->neighbor;
			stmt_parser(child, retType);
		}
	}
	else if (child->type == WHILE_T) {
		child = child->neighbor->neighbor;
		exp_parser(child);
		child = child->neighbor->neighbor;
		stmt_parser(child, retType);
	}
}

Type* exp_parser(ST_NODE* head) {
	logd("parsing...exp", LOG_I);
	ST_NODE* child = head->first_child;
	if (child->type == Exp_T) {

		Type* type1 = exp_parser(child);
		if (type1 == NULL) {
			return NULL;
		}
		
		child = child->neighbor;
		if (child) {

			
			if (child->type == ASSIGNOP_T) {
				ST_NODE* p = head->first_child->first_child;
				if (p != NULL) {
					if (p->type == ID_T) {

					}
					else if (p->type == Exp_T && p->neighbor != NULL && (p->neighbor->type == Exp_T || p->neighbor->type == DOT_T)) {

					}
					else {
						semantic_error(LeftHandNotVariable, child->lineno, "The left-hand side of an assignment must be a variable.", "");
						return NULL;
					}
				} 
				else {
					return NULL;
				}
				child = child->neighbor;
				Type* type2 = exp_parser(child);
				if (type2 == NULL) {
					return NULL;
				}
				if (!type_equals(type1, type2)) {
					semantic_error(TypeMismatchedAssignment, child->lineno, "Type Mismatched Assignment", NULL);
					return NULL;
				}
				else {
					return type1;
				}
			}
			else if (child->type == DOT_T) {
				if (type1->kind != STRUCTURE) {
					semantic_error(IllegalUseDot, child->lineno, "Illegal use of \".\"", "");
					return NULL;
				}
				child = child->neighbor;
				FieldList* list = type1->u.structure;
				int flag = 0;
				while (list) {
					if (!strcmp(list->name, child->string_value)) {
						flag = 1;
					}
					list = list->tail;
				}
				if (flag == 0) {
					semantic_error(NonExistentField, child->lineno, "Non-existent field", child->string_value);
					return NULL;
				}
				FieldList* field = table_get(child->string_value);
				if (field == NULL) {
					semantic_error(UndefinedVariable, child->lineno, "Undefined Variable", child->string_value);
					return NULL;
				} else {
					return field->type;
				}
			}
			else if(child->type == LB_T) {
				if (type1->kind != ARRAY) {
					semantic_error(IdNotArray, child->lineno, "Identifier is not an array", "");
				}
				child = child->neighbor;
				Type* type2 = exp_parser(child);
				if (!(type2->kind == BASIC && type2->u.basic == BASIC_INT)) {
					semantic_error(ArrayKeyNotInteger, child->lineno, "Index of array is not interger", "");
					return NULL;
				}
				if (type1->u.array.elem != NULL) {
					return type1->u.array.elem;
				}
				return type1;
			}
			else {
				child = child->neighbor;
				Type* type2 = exp_parser(child);
				if (type2 == NULL) {
					return NULL;
				}
				if (!type_equals(type1, type2)) {
					semantic_error(TypeMismatchedOperands, child->lineno, "Type Mismatched Operands", NULL);
					return NULL;
				}
				else {
					return type1;
				}

			}
		}
		
		
	}
	else if (child->type == ID_T) {
		FieldList* field = table_get(child->string_value);
		if (field == NULL) {
			if (child->neighbor == NULL) {
				semantic_error(UndefinedVariable, child->lineno, "Undefined Variable", child->string_value);
			}
			else {
				semantic_error(UndefinedFunction, child->lineno, "Undefined Function", child->string_value);
			}
			return NULL;
		} 
		else {
			child = child->neighbor;
			if (child != NULL) {
				if (child->type == LP_T) {
					if (field->type->kind != FUNCTION) {
						semantic_error(IdNotFunction, child->lineno, "Identifier is not a function", head->first_child->string_value);
						return NULL;
					}
					child = child->neighbor;
					if (field->type->kind == FUNCTION && child->type == Args_T) {
						FieldList* args = args_parser(child);
						FieldList* p1 = field->type->u.func.params;
						FieldList* p2 = args;
						int flag = 0;
						while (p1 && p2) {
							if (!type_equals(p1->type, p2->type)) {
								flag = 1;
								break;
							}
							p1 = p1->tail;
							p2 = p2->tail;
						}
						if ((p1 == NULL && p2 != NULL) || (p1 != NULL) && (p2 == NULL)) {
							flag = 1;
						}
						if (flag) {
							semantic_error(FunctionNotApplicable, child->lineno, "function is not applicable for arguments", head->first_child->string_value);
						}
					} 
					else {
						if (field->type->kind == FUNCTION && field->type->u.func.num != 0) {
							semantic_error(FunctionNotApplicable, child->lineno, "is not applicable for arguments", head->first_child->string_value);
							return NULL;
						}
					}
				}
			}
			if (field->type->kind == FUNCTION) {
				return field->type->u.func.ret;
			}
			return field->type;
			
		}
		
	}
	else if ((child->type == LP_T || child->type == MINUS_T || child->type == LP_T) && child->neighbor->type == Exp_T ) {
		child = child->neighbor;
		 return exp_parser(child);
	} else if (child->type == FLOAT_T) {
		Type* t = create_type(BASIC);
		t->u.basic = BASIC_FLOAT;
		return t;
	} else if (child->type == INT_T) {
		Type* t = create_type(BASIC);
		t->u.basic = BASIC_INT;
		logd("int", LOG_I);
		return t;
	}
	return NULL;
}

FieldList* args_parser(ST_NODE* head) {
	ST_NODE* child = head->first_child;
	Type* type = exp_parser(child);
	if (type == NULL) {
		return NULL;
	}
	else {
		FieldList* field1 = create_fieldlist("null");
		field1->type = type;
		child = child->neighbor;
		if (child != NULL) {
			child = child->neighbor;
			FieldList* field2 = args_parser(child);
			field1->tail = field2;
		}
		return field1;
	}
	return NULL;
}