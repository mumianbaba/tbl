#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tchar.h>
#include <math.h>
#include "tbl.h"
#include "lex.h"


const char *skip(const char *in)
{
	while(*in && *in <= 32 ) ++in;
	
	return in;
}

double read_number()
{
	double sum = 0;
	while(isdigit(*src))
	{
		sum = sum * 10 + (*src - '0');
		++src;
	}

	double sub = 0;
	//假如下一个是.那么是浮点数，开始计算下面的数字
	if(*src == '.')
	{
		++src;
		
		int sign = 0;
		while(isdigit(*src))
		{
			sub = sub * 10 + (*src - '0');
			
			--sign;
			++src;
		}

		sub = sub * pow(10.0, sign);
	}

	return sum + sub;
}

char *read_keyword()
{
	const char *start = src;
	unsigned int len = 0;
	while(isalpha(*src) || *src == '_' || isdigit(*src))
	{
		++len;
		++src;
	}

	char *buf = (char*)calloc(1, (sizeof(char) * len + 1));
		
	strncpy(buf, start, sizeof(char) * len);
		
	return buf;
}

char *read_str()
{
	unsigned int len = 0;
	const char *start = src;
	while(*src && *src!= '\"')
	{
		++len;
		++src;
	}
		
	char *buf = (char*)calloc(1, (sizeof(char) * len + 1));
	strncpy(buf, start, sizeof(char) * len);
		
	return buf;
}

Node *read_token()
{
	Node *node = create_node();
	node->kind = END_LINE;

	if(*src == '(')
	{
		++src;
		node->kind = LEFT_PARENT;

		return node;
	}
	
	if(*src == ')')
	{
		++src;
		node->kind = RIGHT_PARENT;
		
		return node;
	}
	
	if(*src == '{')
	{
		++src;
		node->kind = LEFT_BRACE;
		
		return node;
	}
	
	if(*src == '}')
	{
		++src;
		node->kind = RIGHT_BRACE;
		
		return node;
	}
	
	if(*src == '+')
	{
		++src;
		node->kind = ADD;

		return node;
	}
	
	if(*src == '-')
	{
		++src;
		node->kind = DE;

		return node;
	}
	
	if(*src == '*')
	{
		++src;
		node->kind = MUL;

		return node;
	}
	
	if(*src == '/')
	{
		++src;
		node->kind = SUB;

		return node;
	}

	if(*src == '^')
	{
		++src;
		node->kind = POW;

		return node;
	}
	
	//= 或者 == 
	if(*src == '=')
	{
		node->kind = AS;
		if( *(src + 1) == '=')
		{
			++src;
			node->kind = EQ;
		}
		++src;

		return node;
	}
	
	//<
	if(*src == '<')
	{
		node->kind = LT;
	
		if(*(src + 1) == '=')
		{
			++src;
			node->kind = LE;

			return node;
		}
		
		if(*(src + 1) == '>')
		{
			++src;
			node->kind = NE;

			return node;
		}
		++src;

		return node;
	}
	
	if(*src == '>')
	{
		node->kind = LT;
		
		if(*(src + 1) == '=')
		{
			++src;
			node->kind = BE;
		}

		++src;

		return node;
	}

	if (*src == '\"')
	{
		node->kind = STRING;
		++src;

		return node;
	}
	
	if(isdigit(*src))
	{
		node->id = read_number();
		node->kind = NUMBER;

		return node;
	}
	
	if(isalpha(*src) || *src == '_')
	{
		char *keyword = read_keyword();
		
		if(strcmp(keyword, "not") == 0)
		{
			node->kind = LOG_NOT;
			
			return node;
		}
		
		if(strcmp(keyword, "or") == 0)
		{
			node->kind = LOG_OR;
			
			return node;
		}
		
		if(strcmp(keyword, "and") == 0)
		{
			node->kind = LOG_AND;
			
			return node;
		}
		
		if(strcmp(keyword, "return") == 0)
		{
			node->kind = RETURN;
			
			return node;
		}

		node->kind  = IDENT;
		node->iname = keyword;
		
		return node;
	}

	if(*src == '\0')
	{
		node->kind = END_LINE;

		return node;
	}
	if(*src == ';')
	{
		++src;
		node->kind = END_STM;

		return node;
	}

	if(*src == ',')
	{
		++src;
		node->kind = COMMA;
		return node;
	}

	return node;
}

Node *get()
{
	src = skip(src);
	ppre = src;

	return read_token();
}

BOOL next_token(int token)
{
	src = skip(src);

	ppre = src;
	Node *node = read_token();

	if(node->kind == token)
		return TRUE;

	src = ppre;

	return FALSE;
}

Node *peek()
{
	src = skip(src);

	ppre = src;

	Node *node = read_token();

	src = ppre;

	return node;
}

void unget_token(Node *node)
{
	src = ppre;
	
	free(node);
}

void next()
{
	++src;
}

int expect(char token)
{
	char c = *(src + 1);
	
	if (c != token)
	{
		return FALSE;
	}
	
	return TRUE;
}

void init_lex(const char *exp)
{
	ppre = src = exp;
}

Node *create_node()
{
	Node *node = (Node*)malloc(sizeof(Node));
	
	return node;
}

