#pragma once
#include "toLUA\tolua.h"
#ifndef __cplusplus
#include <stdlib.h>
#endif
#ifdef __cplusplus
extern "C" int tolua_bnd_takeownership(lua_State* L); // from tolua_map.c
#else
int tolua_bnd_takeownership(lua_State* L); /* from tolua_map.c */
#endif
#include <string.h>

TOLUA_API int tolua_natives_open(lua_State* tolua_S);
LUALIB_API int luaopen_natives(lua_State* tolua_S);

inline void returnTable(lua_State* L,unsigned long *ary, unsigned long count) {
	try {
		lua_newtable(L);
		for (unsigned long i = 0; i < count; i++) {
			try {
				const int index = i * 2 + 2;
				lua_pushnumber(L, i + 1 );
				lua_pushnumber(L, ary[index]);
				lua_settable(L, -3);
			}
			catch(...) {
				printf("[Table]Failed to PushNumber at Index: %d", i);
			}
		}
		tolua_pushnumber(L, count);
	}
	catch (...) {
		printf("[Table] Uncatched Exception happend");
	}
}