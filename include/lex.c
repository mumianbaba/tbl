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

const char *skip_block(const char *data, const char *start, const char *end)
{
	const char *s1 = strstr(data, start);
	if (!s1)
	{
		return NULL;
	}

	s1 = s1 + strlen(start);

	const char *s2 = strstr(s1, end);
	if (!s2)
	{
		return NULL;
	}

	s2 = s2 + strlen(end);

	return s2;
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

	if (*src == '[')
	{
		++src;
		node->kind = LEFT_BRACKET;

		return node;
	}

	if (*src == ']')
	{
		++src;
		node->kind = RIGHT_BRACKET;

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

	if (*src == '%')
	{
		++src;
		node->kind = MOD;

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

		if (*(src) == '/')
		{
			src = skip_block(src, "/", "\n");

			if (src == NULL)
			{
				printf("single annotation mismatching!");
				exit(1);
			}

			src = skip(src);
			return read_token();
		}

		if (*(src) == '*')
		{
			src = skip_block(src, "*", "*/");

			if (src == NULL)
			{
				printf("multi annotation mismatching!");
				exit(1);
			}

			src = skip(src);
			return read_token();
		}

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
		++src;
		node->kind = AS;
		if( *(src) == '=')
		{
			++src;
			node->kind = EQ;
		}

		return node;
	}
	
	//<
	if(*src == '<')
	{
		++src;

		node->kind = LT;
	
		if(*(src) == '=')
		{
			++src;
			node->kind = LE;

			return node;
		}
		
		if(*(src) == '>')
		{
			++src;
			node->kind = NE;

			return node;
		}

		return node;
	}
	
	if(*src == '>')
	{
		++src;
		node->kind = BT;
		
		if(*(src) == '=')
		{
			++src;
			node->kind = BE;
		}

		return node;
	}

	if (*src == '\"')
	{
		node->kind = STRING;
		++src;

		return node;
	}

	if (*src == '.')
	{
		node->kind = POINT;
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
		if (strcmp(keyword, "true") == 0)
		{
			node->kind = BOOL_TRUE;
			node->state = 1;

			return node;
		} 
		

		if (strcmp(keyword, "false") == 0)
		{
			node->kind = BOOL_FALSE;
			node->state = 0;

			return node;
		}
		
		if(strcmp(keyword, "return") == 0)
		{
			node->kind = RETURN;
			
			return node;
		}

		if (strcmp(keyword, "if") == 0)
		{
			node->kind = KEYWORD_IF;

			return node;
		}

		if (strcmp(keyword, "break") == 0)
		{
			node->kind = KEYWORD_BREAK;

			return node;
		}

		if (strcmp(keyword, "for") == 0)
		{
			node->kind = KEYWORD_FOR;

			return node;
		}
		if (strcmp(keyword, "while") == 0)
		{
			node->kind = KEYWORD_WHILE;

			return node;
		}
		if (strcmp(keyword, "do") == 0)
		{
			node->kind = KEYWORD_DO;

			return node;
		}

		if (strcmp(keyword, "else") == 0)
		{
			node->kind = KEYWORD_ELSE;

			return node;
		}

		if (strcmp(keyword, "def") == 0)
		{
			node->kind = KEYWORD_DEF;

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
	Node *node = (Node*)calloc(1, sizeof(Node));
	
	return node;
}

