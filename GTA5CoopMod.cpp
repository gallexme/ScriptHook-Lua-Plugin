#include "GTA5CoopMod.h"

#include <ctime>
#include <vector>
#include "LUA\lua.hpp"
#include "natives_module.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

const char* FunctionToRun = "";
int get_key_pressed(lua_State *l)
{
	int key = lua_tointeger(l, 1);
	//return (GetKeyState(nVirtKey) & 0x8000) != 0;
	lua_pushboolean(l, (GetAsyncKeyState(key) & 0x8000) != 0);
	return 1;
}
int wait(lua_State *l)
{
	int key = lua_tointeger(l, 1);
	//return (GetKeyState(nVirtKey) & 0x8000) != 0;
	scriptWait(key);
	return 1;
}
void runNewThread(int b) {
	if (FunctionToRun != "") {

	}
}
int Lua_run_in_seperate_thread(lua_State *L) {
	if (lua_isstring(L, 1)) {
		FunctionToRun = lua_tostring(L, 1);
		lua_State* newState = lua_newthread(L);
		//HANDLE hThread = ::CreateThread(NULL, 0, (::LPTHREAD_START_ROUTINE)&runNewThread, (::HMODULE)Instance, 0, NULL);

	}
	return 0;

}
void run_function(lua_State *L, const char *func)
{
	lua_getglobal(L, func);
	int error = lua_pcall(L, 0, 0, 0);

	if (error) {

		fprintf(stderr, "%s: %s\n", func, lua_tostring(L, -1));
		lua_pop(L, 1);
		scriptWait(500);
	}
	if (error == LUA_ERRMEM) {
		fprintf(stderr, "C daemon: Memory error");
		scriptWait(500);
	}
}
void run_file(lua_State *L, char *file)
{
	int error = luaL_loadfile(L, file) || lua_pcall(L, 0, 0, 0);

	if (error) {
		fprintf(stderr, "%s: %s\n", file, lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	if (error == LUA_ERRMEM) {
		fprintf(stderr, "C daemon: Memory error");
	}
}
lua_State * generateLuaState() {
	lua_State *lua_state;
	lua_state = luaL_newstate();
	// load Lua libraries
	luaL_openlibs(lua_state);
	tolua_natives_open(lua_state);
	if (luaL_dofile(lua_state, "scripts/main.lua")) {
		fprintf(stderr, "Main.lua: %s\n", lua_tostring(lua_state, -1));
		lua_pop(lua_state, 1);

	}
	lua_pushcfunction(lua_state, &get_key_pressed);
	lua_setglobal(lua_state, "get_key_pressed");
	lua_pushcfunction(lua_state, &wait);
	lua_setglobal(lua_state, "wait");
	run_function(lua_state, "init");
	return lua_state;
}



void initializeLua(lua_State *lua_state) {
	// create new Lua state
	long lastPressedReload = 0;
	printf("Loaded main.lua");
	try {
		while (true) {
#ifdef SDK
			if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0 && (GetAsyncKeyState(0x52) & 0x8000) != 0 && (clock() - lastPressedReload) > 500) {
				lastPressedReload = clock();
				run_function(lua_state, "unload");
				lua_state = NULL;

				lua_state = generateLuaState();

			}

			else {
#endif
				run_function(lua_state, "tick");
				scriptWait(0);
#ifdef SDK
			}
#endif
		}
	}
	catch (...) {
		printf("%s\n", lua_tostring(lua_state, -1));
		scriptWait(500);
	}
	// close the Lua state
	lua_close(lua_state);

}
void GTA5CoopMod::initialize()
{
#ifdef SDK
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif
	lua_State *l = generateLuaState();
	initializeLua(l);


}
