#include "env.h"

ENVIROMENT *make_env()
{
	return do_make_env(NULL);
}

ENVIROMENT *do_make_env(ENVIROMENT *parent)
{
	ENVIROMENT *r = (ENVIROMENT*)malloc(sizeof(ENVIROMENT));
	r->parent = parent;
	r->map = make_map();
	
    return r;
}
