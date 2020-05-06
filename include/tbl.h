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

typedef struct Map {
	struct Map *parent;
	char **key;
	void **val;
	int size;
	int nelem;
	int nused;
} Map;

enum
{
	NONE,
	STRING,
	NUMBER,
	BINARY,
	UNARY,
	FUNC,
	FUNC_CALL,
	ADD,
	DE,
	MOD,
	SUB,
	MUL,
	POW,
	LT,
	BT,
	LE,
	BE,
	EQ,
	NE,
	AS,
	LOG_AND,
	LOG_NOT,
	LOG_OR,
	LOG_BOOL,
	BOOL_TRUE,
	BOOL_FALSE,
	COMMA,
	LEFT_PARENT,
	RIGHT_PARENT,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	LEFT_BRACE,
	RIGHT_BRACE,
	SINGLE_ANNOTATION,
	MULTI_ANNOTATION,
	MULTI_ANNOTATION_END,
	BRACK_LEFT,
	BRACK_RIGHT,
	QUOTATION,
	POINT,
	IDENT,
	TABLE,
	TABLE_DREF,
	KEYWORD_IF,
	KEYWORD_FOR,
	KEYWORD_WHILE,
	KEYWORD_DO,
	KEYWORD_BREAK,
	KEYWORD_ELSE,
	KEYWORD_ELSEIF,
	KEYWORD_DEF,
	RETURN,
	KRYWORD_NULL,
	CONPOUND,
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
			struct Node *while_condition;
			struct Node *while_body;
		};

		//Table 解引用节点
		struct
		{
			struct Node *header;
			struct Node *offset;
		};

		//Table类型节点
		struct
		{
			Vector *arr;
			Map *hash_map;
		};
		
		struct 
		{
			struct Node *for_begin;
			struct Node *for_condition;
			struct Node *for_body;
			struct Node *for_end;
		};

		struct
		{
			struct Node *node_condition;
			struct Node *node_then;
			struct Node *node_else;
		};

		//组合语句
		struct
		{
			Vector *body;
		};

		//bool 值
		struct
		{
			int state;
		};

		struct 
		{
			int tkind;
			struct Node *order;
		};

		//函数调用节点
		struct
		{
			struct Node *func_point;
			Vector *formal;
		};

		struct 
		{
			Vector *actual;
			struct Node *fbody;
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
