#ifndef __PARSER__HEADER__
#define __PARSER__HEADER__
#include "tbl.h"
#include "env.h"
#include "lex.h"

Node *read_exp();
Node *read_var_function();
Node *read_postfix(Node *node);
Node *primer_exp();
Node *read_pow();
Node *read_mul();
Node *read_add();
Node *read_binary();
Node *CompareExp();
Node *NotExp();
Node *AndExp();
Node *Exp();
Node *interior_print(Vector *params, ENVIROMENT *env);

Node *eval(Node *node, ENVIROMENT *env);

void init_parser(const char *exp);
void parser(const char *exp);

#endif
