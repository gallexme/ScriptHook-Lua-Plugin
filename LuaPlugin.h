#pragma once
#include "Singleton.h"
#include "natives.h"
#include "types.h"
#include "enums.h"
#include <string>
#include "main.h"
 class LuaPlugin : public Singleton<LuaPlugin>
{
public:
	void initialize();
		
private:
 };

