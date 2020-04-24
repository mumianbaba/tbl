#include "tbl.h"
#include "lex.h"
#include "env.h"
#include "parser.h"
#include "vector.h"

ENVIROMENT *basic_env;

Node *Exp();

typedef Node*(*FUNCTION_POINT)(Vector *, ENVIROMENT *);

typedef struct INLINE_CALL
{
	char *func_name;
	FUNCTION_POINT *func;
}INLINE_CALL;

static INLINE_CALL call_list[] =
{
	{"print", interior_print}
};

//需要支持闭包，while(kind == ()) xxx, 把函数节点拼接起来
Node *read_func_call(Node *node)
{
	if(peek()->kind == RIGHT_PARENT)
	{
		return node;
	}

	node->formal = make_vector(); //
	
	Node *gnode  = map_get(basic_env->map, node->fname);

	if (gnode == NULL)
	{
		printf("error, undefine function :%s\n", node->fname);

		exit(1);
	}
	node->actual = gnode->actual;
	
	int narg = 0; //用来判断参数的位置 
 	while(1)
	{
		if(narg > vec_len(node->actual))
		{
			printf("error, argument count!\n");

			exit(1);
		}

		Node *tok = Exp();
		
		vec_push(node->formal, tok);
		
		Node *ttok = get();
		if(ttok->kind == COMMA)
		{
			continue;
		}
		
		if(ttok->kind == RIGHT_PARENT)
		{
			unget_token(ttok);

			return node;
		}
		
		printf("error token\n");

		exit(1);
	}
	
	return node;
}

Node *read_var_function()
{
	Node *tok = get();
	
	tok->kind = IDENT;
	
	Node *peek_tok = peek();
	if(peek_tok->kind == LEFT_PARENT)
	{
		tok->kind  = FUNC;
		tok->fname = tok->iname;
	}

	return read_postfix(tok);
}

Node *read_postfix(Node* node)
{
	Node *tok = get();
	if(tok->kind == LEFT_PARENT)
	{
		Node *fnode = read_func_call(node);

		if(!next_token(RIGHT_PARENT))
		{
			printf("expect )\n");
			exit(1);
		}

		return fnode;
	}
	unget_token(tok);

	return node;
}

Node *read_function_name(Node *node)
{
	Node *tok = get();
	if(tok->kind != IDENT)
	{
		printf("error function name define!");
		exit(1);
	}
	
	Node *fnode = map_get(basic_env->map, tok->iname);
	if(fnode != NULL)
	{
		printf("function name %s has redefine\n", tok->iname);	
		exit(1);
	}

	//需要判断是否重定义 
	node->fname = tok->iname;
	
	return tok;
}

Node *read_function_args(Node *node)
{
	while(1)
	{
		Node *tok = get();
		
		if(tok->kind != IDENT)
		{
			printf("expect vaiable!\n");
			
			exit(1);
		}
		
		vec_push(node->actual, tok->iname);
		
		tok = get();
		if(tok->kind == COMMA)
		{
			continue;
		}
		
		if(tok->kind == RIGHT_PARENT)
		{
			unget_token(tok);

			return node;
		}
		
		printf("error token\n");

		exit(1);
	}
}

Node *read_condition()
{
	int kind = peek()->kind;

	//条件语句的条件只能是以下的形式
	if (kind == IDENT || kind == FUNC || kind == BOOL_FALSE || kind == BOOL_FALSE)
	{
		return read_binary();
	}

	printf("error unexpect kind\n");

	exit(1);
}

Node *read_compound_exp()
{
	int kind = get()->kind;
	if (kind != LEFT_BRACE)
	{
		printf("expect {\n");
		exit(1);
	}

	Node *compund_node = create_node();

	compund_node->kind = CONPOUND;

	Vector *list = make_vector();

	kind = peek()->kind;
	while(kind != RIGHT_BRACE)
	{
		Node *node = read_stmt();

		vec_push(list, node);

		kind = peek()->kind;
	}

	kind = get()->kind;
	if (kind != RIGHT_BRACE)
	{
		printf("expect }\n");
		exit(1);
	}

	compund_node->body = list;

	return compund_node;
}

Node *read_stmt()
{
	int kind = peek()->kind;
	switch (kind)
	{
		case KEYWORD_IF: return  read_if_exp();
		case KEYWORD_FOR: return read_for_exp();
		case KEYWORD_DO: return read_dowhile_exp();
		case LEFT_BRACE: return read_compound_exp();

		case RETURN: return read_return();
	}

	if (kind == IDENT)
	{
		Node *node = read_binary();

		if (!next_token(END_STM))
		{
			printf("error unkown ;\n");
			exit(1);
		}

		return node;
	}

	printf("error unkown statement\n");
	exit(1);
}

Node *read_if_exp()
{
	Node *tok = get();
	if (tok->kind != KEYWORD_IF)
	{
		printf("error, not an if exp\n");

		exit(1);
	}

	Node *ifnode = create_node();

	tok = get();
	if (tok->kind != LEFT_PARENT)
	{
		printf("error, expect (");

		exit(1);
	}
	ifnode->node_condition = read_condition();;
	tok = get();
	if (tok->kind != RIGHT_PARENT)
	{
		printf("error, expect (");

		exit(1);
	}

	ifnode->node_then = read_stmt();

	tok = get();
	if (tok->kind == KEYWORD_ELSE)
	{
		ifnode->node_else = read_stmt();

		return ifnode;
	}
	unget_token(tok);

	return ifnode;
}

Node *read_while_exp()
{
	Node *tok = get();
	if (tok->kind != KEYWORD_WHILE)
	{
		printf("error, not an if exp\n");

		exit(1);
	}

	return tok;
}

Node *read_dowhile_exp()
{
	Node *tok = get();
	if (tok->kind != KEYWORD_DO)
	{
		printf("error, not an if exp\n");

		exit(1);
	}

	return tok;
}

Node *read_for_exp()
{
	Node *tok = get();
	if (tok->kind != KEYWORD_FOR)
	{
		printf("error, not an if exp\n");

		exit(1);
	}

	return tok;
}

Node *read_function_def()
{
	Node *tok = get();
	if (tok->kind != KEYWORD_DEF)
	{
		printf("error, not an if exp\n");

		exit(1);
	}

	Node *node    = create_node();
	node->actual  = make_vector(); 
	node->formal  = make_vector(); 
	node->fbody   = NULL;
	
	read_function_name(node);

	tok = get();
	if (tok->kind != LEFT_PARENT)
	{
		printf("error, not an if exp\n");

		exit(1);
	}

	read_function_args(node);

	tok = get();
	if (tok->kind != RIGHT_PARENT)
	{
		printf("error, not an if exp\n");

		exit(1);
	}
	
	map_put(basic_env->map, node->fname, node);
	node->fbody = read_compound_exp();

	return node;
}

Node *read_return()
{
	Node *node = get();
	if(node->kind != RETURN)
	{
		printf("expect return\n");
		exit(1);
	}
	
	node->order = Exp();
	
	if(!next_token(END_STM))
	{
		printf("expect ;\n");
		exit(1);
	}
	
	return node;
}

Node *read_string()
{
	Node *tok = get();
	if (tok->kind != STRING)
	{
		printf("expect \"\n");
		exit(1);
	}

	Node *node = create_node();

	node->kind = STRING;
	node->sval = read_str();
	node->slen = strlen(node->sval);

	if (!next_token(STRING))
	{
		printf("expect ;\n");
		exit(1);
	}

	return node;
}

Node *read_bool()
{
	Node *node = get();
	if (node->kind != BOOL_TRUE && node->kind != BOOL_FALSE)
	{
		printf("expect \"\n");
		exit(1);
	}

	return node;
}

Node *primer_exp()
{
	Node *node = peek();
	int kind = node->kind;
	free(node);

    if(kind == NUMBER)
    {
    	Node *inode = create_node();
    	inode->kind = NUMBER;
    	inode->id   = read_number();
    	
        return inode;
    }

	if (kind == BOOL_TRUE)
	{
		Node *node = read_bool();
		node->kind = LOG_BOOL;
		node->state = 1;

		return node;
	}

	if (kind == BOOL_FALSE)
	{
		Node *node = read_bool();
		node->kind = LOG_BOOL;
		node->state = 0;

		return node;
	}
    
    if(kind == LEFT_PARENT)
    {
		get();
        Node *exp = Exp();

        if(!next_token(RIGHT_PARENT))
        {
            printf("error expect )\n");
            
            exit(1);
        }
        
        return exp;
    }
    
    if(kind == ADD || kind == DE)
    {
        //注意正负号
        int sined = 1;
        if(kind == DE)
        {
            sined = -1;
        }
        
        Node *inode = create_node();
        	
        inode->kind = NUMBER;
        inode->id = read_number() * sined;
        
        return inode;
    }
    
    if(kind == RETURN)
    {
        return read_return();
	}

	if(kind == IDENT)
	{
		return read_var_function();
	}

	if (kind == STRING)
	{
		return read_string();
	}

   printf("error unkown token\n");

   exit(1);
}

Node *read_unary()
{
	Node *tok = get();
	if(tok->kind == LOG_NOT)
	{
		Node *tnode  = create_node();
		tnode->kind  = LOG_NOT;
		tnode->order = primer_exp();

		return tnode;

	}
	unget_token(tok);

	return primer_exp();
}

Node *read_pow()
{
    Node *node = read_unary();

    Node *tok = get();
    while(tok->kind == POW)
    {
        Node *binary = create_node();

        binary->left  = node;
        binary->right = read_unary();
        binary->kind  = POW;
		node = binary;
        
        tok = get();
    }
    unget_token(tok);
    
    return node;
}

Node *read_mul()
{
    Node *node = read_pow();

 	Node *tok = get();
    while(tok->kind == MUL)
    {
        Node *binary = create_node();

        binary->left = node;
        binary->right = read_pow();
        binary->kind = MUL;
		node = binary;
        
        tok = get();
    }
    unget_token(tok);
    
    return node;
}

Node *read_add()
{
	Node *node = read_mul();

    Node *tok = get();
	while(tok->kind == ADD)
	{
		Node *binary = create_node();

		binary->left  = node;
		binary->right = read_mul();
		binary->kind  = ADD;
		
		node = binary;
        
		tok = get();
	}
	unget_token(tok);

    return node;
}

Node *CompareExp()
{
	Node *node = read_add();
	
	Node *tok = get();
	while(tok->kind >= LT && tok->kind <= NE)
	{
		Node *binary = create_node();
        
		binary->left  = node;
		binary->right = CompareExp();
		binary->kind = tok->kind;

		node = binary;

		tok = get();
	}
	unget_token(tok);

	return node;
}

Node *AndExp()
{
	Node *node = CompareExp();
	
	Node *tok = get();
	while(tok->kind == LOG_AND)
	{
		Node *binary = create_node();
        
		binary->kind  = tok->kind;
        binary->left  = node;
        binary->right = AndExp();
		node = binary;
        
        tok = get();
	}
	unget_token(tok);
	
	return node;
}

Node *Exp()
{
	Node *node = AndExp();

	Node *tok = get();
	if(tok->kind == LOG_OR)
	{
	    Node *binary = create_node();
        
		binary->kind  = tok->kind;
        binary->left  = node;
        binary->right = Exp();
		node = binary;
        
       tok = get();
	}
	unget_token(tok);
	
	return node;
}

Node *read_binary()
{
	Node *node = Exp();

	Node *tok = get();
	if (tok->kind == AS)
	{
		Node *binary = create_node();

		binary->left = node;
		binary->right = read_binary();
		binary->kind = tok->kind;

		node = binary;

		tok = get();
	}
	unget_token(tok); 

	return node;
}

Node *interior_print(Vector *params, ENVIROMENT *env)
{
	Node *fnode = create_node();

	fnode->fname = "print";
	fnode->formal = params;

	int i = 0;

	int count = vec_len(params);
	
	for (i = 0; i < count; i++)
	{
		Node *param_node = (Node*)vec_get(params, i);

		if (param_node->kind == STRING)
		{ 
			printf("%s", param_node->sval);
		}

		if (param_node->kind == NUMBER)
		{
			printf("%f", param_node->id);
		}
		if (param_node->kind == LOG_BOOL)
		{
			printf("%s", param_node->state == 1 ? "true": "false");
		}

		if (i < count - 1)
		{
			printf(", ");
		}
	}

	printf("\n");

	return fnode->returning;
}

FUNCTION_POINT *is_interior(const char *fname)
{
	int i = 0;
	for (i = 0; i < sizeof(call_list) / sizeof(INLINE_CALL); i++)
	{
		if (strcmp(call_list[i].func_name, fname) == 0)
		{
			return call_list[i].func;
		}
	}

	return NULL;
}

void init_interior_function()
{
	Node *fnode = create_node();

	fnode->fname = "print";
	fnode->actual = make_vector();
	fnode->formal = make_vector();
	map_put(basic_env->map, fnode->fname, fnode);
}

int convert_node(Node *node)
{
	if (node == NULL)
	{
		return 0;
	}

	if (node->kind == LOG_BOOL)
	{
		return node->state;
	}

	if (node->kind == NUMBER)
	{
		return (node->id != 0);
	}

	if (node->kind == STRING)
	{
		return (node->sval != NULL);
	}

	if (node->kind == FUNC)
	{
		return (node->body != NULL);
	}

	printf("can convert type \n");

	exit(1);
}

Node *eval(Node *node, ENVIROMENT *env)
{
	switch(node->kind)
	{
		case IDENT:
		{

			ENVIROMENT *nenv = env;
			Node *node_t = NULL;
			while (nenv)
			{
				node_t = map_get(nenv->map, node->iname);
				if (node_t)
				{
					break;
				}

				if (nenv->parent == NULL)
				{
					break;
				}

				nenv = nenv->parent;
			}
			
			if(node_t == NULL)
			{
				printf("error, %s is not define\n", node->iname);
				exit(1);
			}
			
			return node_t;
		}

		case KEYWORD_IF:
		{
			Node *condition = eval(node->node_condition, env);
			if (condition->state == 1)
			{
				eval(node->node_then, env);
			}
			else
			{
				if (node->node_else)
				{
					eval(node->node_else, env);
				}
			}
		}

		case AS:
		{
			Node *right = eval(node->right, env);
	
			if(node->left->kind != IDENT)
			{
				printf("error left operortor error\n");

				exit(1);
			}

			node->left->type = right->kind;
			node->left->ival = right;

			map_put(env->map, node->left->iname, node->left->ival);
				
			return node->left;
		}

		case CONPOUND:
		{
			ENVIROMENT *new_env = make_env();
			new_env->parent = env;
			
			int i = 0;
			for (i = 0; i < vec_len(node->body); i++)
			{
				Node *cnode = (Node*)vec_get(node->body, i);
				eval(cnode, env);
			}
		}
		
		case FUNC:
		{
			ENVIROMENT *new_env = make_env();
			
			new_env->parent = env;

			Node *fnode = NULL;
			ENVIROMENT *nenv = env;
			while (nenv)
			{
				fnode = map_get(nenv->map, node->fname);
				if(fnode)
				{
					break;
				}

				if(nenv->parent == NULL)
				{
					break;
				}

				nenv = nenv->parent;
			}

			if (fnode == NULL)
			{
				printf("unkonw function name:%s\n", node->fname);
				exit(1);
			}
			
			node->actual = fnode->actual;

			node->body = fnode->body;
			map_put(new_env->map, node->fname, node);
			
			int i = 0;
			for(i = 0; i < vec_len(node->actual); i++)
			{
				char *vname = (char*)vec_get(node->actual, i);
				Node *vnode = (Node*)vec_get(node->formal, i);
				
				//先计算节点的值，再存到当前的环境 
				map_put(new_env->map, vname, eval(vnode, env));
			}
			map_put(new_env->map, "return",  node->returning);

			//假如是内部函数，则直接执行
			FUNCTION_POINT  func_point = is_interior(node->fname);
			if(func_point != NULL)
			{ 
				Vector *formal = make_vector();
				for (i = 0; i < vec_len(node->formal); i++)
				{
					Node *vnode = (Node*)vec_get(node->formal, i);
					vec_push(formal, eval(vnode, env));
				}

				return func_point(formal, new_env);
			}
			
			//取出函数的定义执行
			for(i = 0; i < vec_len(fnode->fbody->body); i++)
			{
				Node *vnode = (Node*)vec_get(fnode->fbody->body, i);
				
				if(vnode->kind == RETURN)
				{
					Node *ret = eval(vnode->order, new_env);
					
					return ret;
				}
				
				eval(vnode, new_env);
			}
			
			return NULL;
		}

		#define  L (eval(node->left, env))
		#define  R (eval(node->right, env))

		case ADD:
			{
				Node* left  = eval(L, env);
				Node* right = eval(R, env);

				Node *nnode = create_node();

				if (left->kind != right->kind)
				{
					printf("can not add to operator!\n");
					exit(1);
				}

				if (left->kind == STRING && right->kind == STRING)
				{
					nnode->kind = STRING;
					
					nnode->sval = (char*)calloc(strlen(left->sval) + strlen(right->sval) + 1, 1);
					strcat(nnode->sval, left->sval);
					strcat(nnode->sval, right->sval);

					return nnode;
				}
				
				if (left->kind == NUMBER && right->kind == NUMBER)
				{
					nnode->kind = NUMBER;
					nnode->id = left->id + right->id;

					return nnode;
				}

				return nnode;
			}
			
		case DE:
			{
				Node* left = eval(L, env);
				Node* right = eval(R, env);
				
				Node *nvar = create_node();
				nvar->kind = NUMBER;
				nvar->id = left->id - right->id;
				
				return nvar;
			}

		case MUL:
			{
				Node* left = eval(L, env);
				Node* right = eval(R, env);
				
				Node *nvar = create_node();
				nvar->kind = NUMBER;
				nvar->id = left->id * right->id;
				
				return nvar;
			}
		case SUB:
			{
				Node* left = eval(L, env);
				Node* right = eval(R, env);
				
				Node *nvar = create_node();
				nvar->kind = NUMBER;
				nvar->id = left->id / right->id;
				
				return nvar;
			}
			
		case POW:
			{
				Node* left = eval(L, env);
				Node* right = eval(R, env);
				
				Node *nvar = create_node();
				nvar->kind = NUMBER;
				nvar->id = pow(left->id, right->id);
				
				return nvar;
			}
		case NUMBER:
		{
			return node;
		}
		case STRING:
		{
			return node;
		}
		case LOG_BOOL:
		{
			return node;
		}

		case LOG_AND:
			{
				Node* left  = eval(L, env);
				Node* right = eval(R, env);
				
				Node *bnode = create_node();
				bnode->kind = LOG_BOOL;
				
				bnode->state = convert_node(left) && convert_node(right);

				return bnode;
			}
		case LOG_OR:
			{
				Node* left = eval(L, env);
				Node* right = eval(R, env);
				
				Node *bnode = create_node();

				bnode->kind = LOG_BOOL;
				bnode->state = bnode->state = convert_node(left) || convert_node(right);

				return bnode;
			}
		case LOG_NOT:
			{
				Node *bnode = create_node();

				bnode->kind = LOG_BOOL;

				Node *log_node = eval(node->order, env);
				bnode->state = !convert_node(log_node);

				return bnode;
			}
			
		case LT:
			{
				Node* left = eval(L, env);
				Node* right = eval(R, env);

				if (left->kind != NUMBER || right->kind != NUMBER)
				{
					printf("error, compare must be number!\n");
					exit(1);
				}
				Node *bnode = create_node();

				bnode->kind = LOG_BOOL;
				bnode->state = left->id < right->id;
				
				return bnode;
			}
		case BT:
			{
				Node* left = eval(L, env);
				Node* right = eval(R, env);

				if (left->kind != NUMBER || right->kind != NUMBER)
				{
					printf("error, compare must be number!\n");
					exit(1);
				}

				Node *bnode = create_node();

				bnode->kind = LOG_BOOL;
				bnode->state = left->id > right->id;

				return bnode;
			}
		case LE:
			{
				Node* left  = eval(L, env);
				Node* right = eval(R, env);

				if (left->kind != NUMBER || right->kind != NUMBER)
				{
					printf("error, compare must be number!\n");
					exit(1);
				}
				
				Node *bnode = create_node();

				bnode->kind  = LOG_BOOL;
				bnode->state = (int)(left->id <= right->id);

				return bnode;
			}
		case BE:
			{
				Node* left =  eval(L, env);
				Node* right = eval(R, env);

				if (left->kind != NUMBER || right->kind != NUMBER)
				{
					printf("error, compare must be number!\n");
					exit(1);
				}
				
				Node *bnode = create_node();

				bnode->kind  = LOG_BOOL;
				bnode->state = (int)(left->id >= right->id);

				return bnode;
			}
		case EQ:
			{
				Node* left  = eval(L, env);
				Node* right = eval(R, env);

				if (left->kind != right->kind)
				{
					printf("error, can not compare kind!\n");
					exit(1);
				}
				
				Node *bnode = create_node();

				bnode->kind = LOG_BOOL;
				bnode->state = 0;

				if (left->kind == STRING)
				{
					if (strcmp(left->sval, right->sval) == 0)
					{
						bnode->state = 1;

						return bnode;
					}
				}

				bnode->state = (int)(left->id == right->id);
				
				return bnode;
			}

	#undef L
	#undef R
	
		default:
		{
			printf("---UNKONW--\n");
			exit(1);
		}

	}
}

void init_parser(const char *exp)
{
	init_lex(exp);

	basic_env = make_env();
}

//把所有节点放到开始节点中 
void interpreter()
{
	//可以把所有的节点存起来， 再做处理，这里是边解释，边执行。
	Vector *vast = make_vector();

	int kind = peek()->kind;
	while(kind != END_LINE)
	{
		switch (kind)
		{
			case KEYWORD_DEF:
			{
				read_function_def();

				break;
			}

			case KEYWORD_IF:
			{
				Node *node = read_if_exp();

				eval(node, basic_env);

				break;
			}

			case KEYWORD_FOR:
			{
				Node *node = read_for_exp();

				eval(node, basic_env);

				break;
			}

			case KEYWORD_DO:
			{
				Node *node = read_dowhile_exp();

				eval(node, basic_env);

				break;
			}

			case IDENT:
			case FUNC:
			{
				Node *node = read_binary();

				if (!next_token(END_STM))
				{
					printf("expect ;\n");

					exit(1);
				}

				eval(node, basic_env);

				break;
			}

			//允许空语句
			case END_STM: break;

			default:
			{
				printf("un epxpect kind!\n");
				exit(1);
				break;
			}
		}

		kind = peek()->kind;
	}
}

void parser(const char *exp)
{
	init_parser(exp);

	init_interior_function();

	interpreter();
}