#include <windows.h>

#include "GTA5CoopMod.h"
HMODULE Instance;
void  ScriptMain() {
	GTA5CoopMod::GetInstance()->initialize();
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
/*int main() {
	GTA5CoopMod::GetInstance()->initialize();
}*/