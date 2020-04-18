#ifndef __HEADER__ENV__
#define __HEADER__ENV__

#include <stddef.h>
#include <stdlib.h>
#include "map.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct ENVIROMENT
{
	struct ENVIROMENT *parent;
	Map *map;
	
}ENVIROMENT;

ENVIROMENT *make_env();
ENVIROMENT *do_make_env(ENVIROMENT *parent);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
