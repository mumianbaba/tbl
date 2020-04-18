#ifndef __LEX__HEADER
#define __LEX__HEADER
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <math.h>
#include "tbl.h"

static const char *src;
static const char *ppre;

const char *skip(const char *in);
double read_number();
char *read_keyword();
char *read_str();
Node *read_token();
Node *get();
BOOL next_token(int token);
Node *peek();
void unget_token(Node *node);
int expect(char token);
void init_lex(const char *exp);

#endif
