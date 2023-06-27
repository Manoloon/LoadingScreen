#pragma once
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IAsyncLoadScreenModule : public IModuleInterface
{
public:
	static inline IAsyncLoadScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IAsyncLoadScreenModule>("AsyncLoadScreen");
	}

	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartInGameLoadingScreen(bool PlayUntilStopped, float PlayTime)=0;

	/** Stops the loading screen */
	virtual void StopInGameLoadingScreen()=0;
};
