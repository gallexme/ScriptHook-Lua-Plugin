#include <windows.h>

#include "LuaPLugin.h"
HMODULE Instance;
void  ScriptMain() {
	LuaPlugin::GetInstance()->initialize();
}
BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		break;
	}
	return TRUE;
}
