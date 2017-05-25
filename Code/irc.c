#include "irc.h"
#include <stdio.h>
#include "st.h"
#include "symbol_type.h"
#include "symbol_table.h"
#include "debug.h"

Operand* create_operand(OpKind kind) {
    Operand* op = (Operand*) malloc(sizeof(Operand));
    op->kind = kind;
    return op;
}

InterCode* create_intercode(IrcKind kind) {
    InterCode* ic = (InterCode*) malloc(sizeof(InterCode));
    ic->kind = kind;
    return ic;
}

CodeList* create_codelist() {
    CodeList* codes = (CodeList*) malloc(sizeof(CodeList));
    codes->prev = NULL;
    codes->next = NULL;
    codes->code = NULL;
    return codes;
}

void ircodes_init() {
    ircodes_tail = ircodes_head = create_codelist();

}

void ircode_append(CodeList* codes) {
    codes->prev = ircodes_tail;
    ircodes_tail->next = codes;
	while (ircodes_tail->next) {
		ircodes_tail = ircodes_tail->next;
	}
}

void codelist_append(CodeList* list1, CodeList* list2) {
	if (list1 != NULL && list2 != NULL) {
		CodeList* tail = list1;
		while (tail->next != NULL) {
			tail = tail->next;
		}
		list2->prev = tail;
		tail->next = list2;
	}
}

int node_translate(ST_NODE* head) {
	if (head->name != NULL) {
		if (head->type == FunDec_T) {
			CodeList* list = translate_funcdec(head);
			ircode_append(list);
			return 0;
		}
		else if (head->type == Stmt_T) {
			CodeList* list = translate_stmt(head);
			ircode_append(list);
			return 0;
		}
		else if (head->type == Exp_T) {
			CodeList* list = translate_exp(head, NULL);
			ircode_append(list);
			return 0;
		}
	}
	return 1;
}

void irc_traverse(ST_NODE* head) {
	if (head) {
		int status = node_translate(head);
		if (status) {
			irc_traverse(head->first_child);	
		}
		irc_traverse(head->neighbor);
	}
}

void ir_generate(ST_NODE* head) {
    ircodes_init();
	t_max = v_max = l_max = 1;
	irc_traverse(head);
}

void relop_output(FILE* fp, RELOP_TYPE relop) {
	switch (relop) {
		case GT:
			fprintf(fp, ">");
			break;
		case LT:
			fprintf(fp, "<");
			break;
		case GE:
			fprintf(fp, ">=");
			break;
		case LE:
			fprintf(fp, "<=");
			break;
		case EQ:
			fprintf(fp, "==");
			break;
		case NE:
			fprintf(fp, "!=");
			break;
		default:
			break;
	}
}

void op_output(FILE* fp, Operand* op) {
	if (op) {
		switch (op->kind) {
			case OP_VARIABLE:
				fprintf(fp, "v%d", op->u.var_no);
				break;
			case OP_CONSTANT:
				fprintf(fp, "%d", op->u.constant);
				break;
			case OP_ADDRESS:
				//fprintf(fp, "%")
			case OP_LABEL:
				fprintf(fp, "label%d", op->u.var_no);
				break;
			case OP_FUNCTION:
				fprintf(fp, "%s", op->u.func_name);
				break;
			case OP_TEMP:
				fprintf(fp, "t%d", op->u.var_no);
				break;
			default:
				break;
		}
	}
}

void ir_output(FILE* fp) {
	CodeList* p = ircodes_head->next;
	while (p) {
		if (p->code) {
			InterCode* code = p->code;
			switch (code->kind) {
				case IRC_ASSIGN:
					op_output(fp, code->u.assign.left);
					fprintf(fp, " = ");
					op_output(fp, code->u.assign.right);
					break;
				case IRC_ADD:
					op_output(fp, code->u.binop.result);
					fprintf(fp, " = ");
					op_output(fp, code->u.binop.op1);
					fprintf(fp, " + ");
					op_output(fp, code->u.binop.op2);
					break;
				case IRC_SUB:
					op_output(fp, code->u.binop.result);
					fprintf(fp, " = ");
					op_output(fp, code->u.binop.op1);
					fprintf(fp, " - ");
					op_output(fp, code->u.binop.op2);
					break;
				case IRC_MUL:
					op_output(fp, code->u.binop.result);
					fprintf(fp, " = ");
					op_output(fp, code->u.binop.op1);
					fprintf(fp, " * ");
					op_output(fp, code->u.binop.op2);
					break;
				case IRC_DIV:
					op_output(fp, code->u.binop.result);
					fprintf(fp, " = ");
					op_output(fp, code->u.binop.op1);
					fprintf(fp, " / ");
					op_output(fp, code->u.binop.op2);
					break;
				case IRC_FUNCTION:
					fprintf(fp, "FUNCTION ");
					op_output(fp, code->u.singleop.op);
					fprintf(fp, " :");
					break;
				case IRC_CALL:
					op_output(fp, code->u.assign.left);
					fprintf(fp, " = CALL ");
					op_output(fp, code->u.assign.right);
					break;
				case IRC_DEC:
					fprintf(fp, "DEC ");
					op_output(fp, code->u.dec.op);
					fprintf(fp, " %d", code->u.dec.size);
					break;
				case IRC_RETURN:
					fprintf(fp, "RETURN ");
					op_output(fp, code->u.singleop.op);
					break;
				case IRC_IFGOTO:
					fprintf(fp, "IF ");
					op_output(fp, code->u.ifgoto.op1);
					fprintf(fp, " ");
					relop_output(fp, code->u.ifgoto.relop);
					fprintf(fp, " ");
					op_output(fp, code->u.ifgoto.op2);
					fprintf(fp, " GOTO ");
					op_output(fp, code->u.ifgoto.label);
				case IRC_GOTO:
					fprintf(fp, "GOTO ");
					op_output(fp, code->u.singleop.op);
					break;
				case IRC_LABEL:
					fprintf(fp, "LABEL ");
					op_output(fp, code->u.singleop.op);
					fprintf(fp, " :");
					break;
				case IRC_READ:
					fprintf(fp, "READ ");
					op_output(fp, code->u.singleop.op);
					break;
				case IRC_WRITE:
					fprintf(fp, "WRITE ");
					op_output(fp, code->u.singleop.op);
					break;
				case IRC_PARAM:
					fprintf(fp, "PARAM ");
					op_output(fp, code->u.singleop.op);
					break;
				case IRC_ARG:
					fprintf(fp, "ARG ");
					op_output(fp, code->u.singleop.op);
					break;
				default:
					break;
			}
		}
		fprintf(fp, "\n");
		p = p->next;
	}
}

Operand* new_temp() {
	Operand* temp = create_operand(OP_TEMP);
	temp->u.var_no = t_max++;
	return temp;
}

Operand* new_label() {
	Operand* label = create_operand(OP_LABEL);
	label->u.var_no = l_max++;
	return label;
}

Operand* new_var() {
	Operand* var = create_operand(OP_VARIABLE);
	var->u.var_no = v_max++;
	return var;
}

CodeList* translate_exp(ST_NODE* head, Operand* place) {
	if (head != NULL && head->type == Exp_T) {
		CodeList* list = create_codelist();
		ST_NODE* child = head->first_child;
		if (child->type == INT_T) {
			if (place != NULL) {
				int value = child->int_value;
				Operand* constant = create_operand(OP_CONSTANT);
				constant->u.constant = value;
				InterCode* code = create_intercode(IRC_ASSIGN);
				code->u.assign.left = place;
				code->u.assign.right = temp;
				list->code = code;
				return list;

			}
			return NULL;
		}
		else if (child->type == FLOAT_T) {

		}
		else if (child->type == ID_T) {

			ST_NODE* child2 = child->neighbor;
			if (child2 != NULL) {
				
			}
			else {
				FieldList* field = table_get(child->string_value);
				InterCode code = create_intercode(IRC_ASSIGN);
				code->u.assign.left = place;
				code->u.assign.right = field->op;
				list->code = code;
				return list;
			}

			
		}
		else if (child->type == MINUS_T) {
			Operand* t1 = new_temp();
			CodeList* list1 = translate_exp(child->neighbor, t1);
			InterCode code2 = create_intercode(IRC_SUB);
			code2->u.binop.result = place;
			Operand* op1 = create_operand(OP_CONSTANT);
			op1->constant = 0;
			code2->u.binop.op1 = op1;
			code2->u.binop.op2 = t1;
			CodeList* list2 = create_codelist();
			list2->code = code2;
			codelilst_append(list1, list2);
			return list1;
		}
		else if (child->type == NOT_T) {
			Operand* label1 = new_label();
			Operand* label2 = new_label();

		}
		else if (child->type == Exp_T) {
			ST_NODE* child2 = child->neighbor;
			switch (child2->type) {
				case ASSIGNOP_T:
					if (child->first_child->type == ID_T) {
						ST_NODE* left_id = child->first_child;
						FieldList* field = table_get(left_id->string_value);
						if (field->op == NULL) {
							field->op = new_var();
						}
						Operand* variable = field->op;
						Operand* t1 = new_temp();
						CodeList* list1 = translate_exp(child2->neighbor, t1);
						InterCode* code1 = create_intercode(IRC_ASSIGN);
						code1->u.assign->left = variable;
						code1->u.assign->right = t1;
						CodeList* list2 = create_codelist();
						list2->code = code1;
						if (place != NULL) {
							InterCode* code2 = create_intercode(IRC_ASSIGN);
							code2->u.assign.left = place;
							code2->u.assign.right = variable;
							CodeList* list3 = create_codelist();
							list3->code = code2;
							codelist_append(list2, list3);
						}
						codelist_append(list1, list2);
						return list1;

					}
					else {

					}
					break;
				case AND_T:
				case OR_T:
				case RELOP_T:
					break;
				case PLUS_T:
				case MINUS_T:
				case STAR_T:
				case DIV_T:
					Operand* t1 = new_temp();
					Operand* t2 = new_temp();
					CodeList* list1 = translate_exp(child, t1);
					CodeList* list2 = translate_exp(child2->neighbor, t1);

					IrcKind kind = IRC_PLUS;
					if (child2->type == MINUS) {
						kind = IRC_SUB;
					}
					else if (child2->type == STAR_T) {
						kind = IRC_MUL;
					}
					else if (child2->type == DIV_T) {
						kind = IRC_DIV;
					}
					InterCode* code = create_intercode(kind);
					code->u.binop.result = place;
					code->u.binop.op1 = t1;
					code->u.binop.op2 = t2;

					CodeList* list3 = create_codelist();
					list3->code = code;
					codelist_append(list2, list3);
					codelist_append(list1, list2);
					return list1;


					break;
				case LB_T:
				case DOT_T:
				default:
					break;
			
			}
		}
	}
    
	return NULL;
}

CodeList* translate_stmt(ST_NODE* head) {
    if (head != NULL && head->type == Stmt_T) {
		ST_NODE* child = head->first_child;
		if (child->type == Exp_T) {

		}
		else if (child->type == CompSt_T) {

		}
		else if (child->type == IF_T) {

		}
		else if (child->type == WHILE_T) {

		}
	}
	return NULL;
}

CodeList* translate_cond(ST_NODE* head, Operand* label_true, Operand* label_false) {
	if (head != NULL) {
		ST_NODE* child = head->first_child;
		if (child->type == NOT_T) {
			return translate_cond(child->neighbor, label_false, label_true);
		} 
		else if (child->neighbor != NULL && child->neighbor->type == RELOP_T) {
			Operand* t1 = new_temp();
			Operand* t2 = new_temp();	
			CodeList* list1 = translate_exp(child, t1);
			CodeList* list2 = translate_exp(child->neighbor->neighbor, t2);
			RELOP_TYPE relop = child->neighbor->relop_value;
			InterCode* code3 = create_intercode(IRC_IFGOTO);
			code3->u.ifgoto.op1 = t1;
			code3->u.ifgoto.op2 = t2;
			code3->u.ifgoto.relop = relop;
			code3->u.ifgoto.label = label_true;
			CodeList* list3 = create_codelist();
			list3->code = code3;
			if (label_false != NULL) {
				InterCode* code4 = create_intercode(IRC_GOTO);
				code4->u.singleop->op = label_false;
				CodeList* list4 = create_codelist();
				list4->code = code4;
				codelist_append(list3, list4);
			}
			codelist_append(list2, list3);
			codelist_append(list1, list2);
			return list1;
			
		}
		else if (child->neighbor != NULL && child->neighbor->type == AND_T) {
			Operand* label1 = new_label();
			CodeList* list1 = translate_cond(child, label1, label_false);
			CodeList* list2 = transalte_cond(child->neighbor->neighbor, label_true, label_false);
			CodeList* label_list = create_codelist();
			InterCode* label_code = create_intercode(IRC_LABEL);
			label_code->u.singleop.op = label1;
			label_list->code = label_code;
			codelist_append(label_list, list2);
			codelist_append(list1, label_list);
			return list1;
		}
		else if (child->neighbor != NULL && child->neighbor->type == OR_T) {
			Operand* label1 = new_label();
			CodeList* list1 = translate_cond(child, label_true, label1);
			CodeList* list2 = transalte_cond(child->neighbor->neighbor, label_true, label_false);
			CodeList* label_list = create_codelist();
			InterCode* label_code = create_intercode(IRC_LABEL);
			label_code->u.singleop.op = label1;
			label_list->code = label_code;
			codelist_append(label_list, list2);
			codelist_append(list1, label_list);
			return list1;
		} 
		else {
			Operand* t1 = new_temp();
			CodeList* list1 = translate_exp(head, t1);
			CodeList* list2 = create_codelist();
			InterCode* code2 = create_intercode(IRC_IFGOTO);
			code2->u.ifgoto.relop = NE;
			Operand* zero_op = create_operand(OP_CONSTANT);
			zero_op->u.constant = 0;
			code2->u.ifgoto.op2 = zero_op;
			code2->u.ifgoto.op1 = t1;
			code2->u.ifgoto.label = label_true;
			list2->code = code2;

			InterCode* code3 = create_intercode(IRC_GOTO);
			code3->u.singleop->op = label_false;
			CodeList* list3 = create_codelist();
			list3->code = code4;

			codelist_append(list2, list3);
			codelist_append(list1, list2);

			return list1;


		}
	}
    return NULL;
}

CodeList* translate_args(ST_NODE* head) {
	if (head != NULL && head->type == Args_T) {
		ST_NODE* child = head->first_child;
		if (child->type == Exp_T) {

		}
	}
    return NULL;
}

CodeList* translate_funcdec(ST_NODE* head) {
	return NULL;
}