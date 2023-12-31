﻿#pragma once
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IAsyncLoadScreenModule : public IModuleInterface
{
public:
	static inline IAsyncLoadScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IAsyncLoadScreenModule>("AsyncLoadScreen");
	}

	virtual void StartInGameLoadingScreen(bool PlayUntilStopped, float PlayTime, const TSharedPtr<SWidget> SlateWidget) = 0;

	/** Stops the loading screen */
	virtual void StopInGameLoadingScreen() = 0;
};