// Copyright 2014 Rui Ueyama. Released under the MIT license.

// This is an implementation of hash table.

#include <stdlib.h>
#include <string.h>
#include "tbl.h"

#define INIT_SIZE 16
#define TOMBSTONE ((void *)-1)

#ifndef UINT32
#define UINT32 unsigned int
#endif

static UINT32 hash(char *p);

static Map *do_make_map(Map *parent, int size) ;

static void maybe_rehash(Map *m);

Map *make_map();

Map *make_map_parent(Map *parent);

static void *map_get_nostack(Map *m, char *key);

void *map_get(Map *m, char *key);

void map_put(Map *m, char *key, void *val);

void map_remove(Map *m, char *key);

size_t map_len(Map *m);
