#ifndef __TTL_HEADER__
#define __TTL_HEADER__

#include <stdio.h> 
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 1
#endif

typedef struct {
    void **body;
    int len;
    int nalloc;
} Vector;

enum
{
	NONE,
	STRING,
	NUMBER,
	BINARY,
	UNARY,
	FUNC,
	ADD,
	DE,
	SUB,
	MUL,
	POW,
	LT,
	BT,
	LE,
	BE,
	EQ,
	AS,
	NE,
	LOG_AND,
	LOG_NOT,
	LOG_OR,
	COMMA,
	LEFT_PARENT,
	RIGHT_PARENT,
	LEFT_BRACE,
	RIGHT_BRACE,
	SINGLE_ANNOTATION,
	MULTI_ANNOTATION,
	MULTI_ANNOTATION_END,
	BRACK_LEFT,
	BRACK_RIGHT,
	QUOTATION,
	IDENT,
	RETURN,
	CONCATENATE,
	END_STM,
	NEW_LINE,
	END_LINE
};

typedef struct Node {

    int kind;
    union 
	{
		struct
		{
			//double
			double id; 
		};

		struct
		{
			int slen;
			char *sval;
		};

		struct 
		{
			int type;
			char *iname;
			struct Node *ival;
		};

		struct 
		{
			struct Node *left;
			struct Node *right;
			int opreand;
		};

		struct 
		{
			int tkind;
			struct Node *order;
		};

		struct 
		{
			Vector *actual;
			Vector *formal;
			Vector *body;
			struct Node *returning;
			char *fname;
		};
	};

}Node;

Node *create_node();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif