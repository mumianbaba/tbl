#ifndef __PARSER__HEADER__
#define __PARSER__HEADER__
#include "tbl.h"
#include "env.h"
#include "lex.h"

Node *read_var_function();
Node *read_postfix(Node *node);
Node *primer_exp();
Node *read_pow();
Node *read_mul();
Node *read_add();
Node *read_binary();
Node *CompareExp();
Node *read_if_exp();
Node *read_for_exp();
Node *read_dowhile_exp();
Node *read_compound_exp();
Node *read_return();
Node *read_break();
Node *read_while_exp();
Node *AndExp();
Node *Exp();
Node *read_stmt();
Node *interior_print(Vector *params, ENVIROMENT *env);

Node *eval(Node *node, ENVIROMENT *env);

void init_parser(const char *exp);
void parser(const char *exp);

#endif
