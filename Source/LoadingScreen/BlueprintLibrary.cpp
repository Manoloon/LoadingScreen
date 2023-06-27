#include "BlueprintLibrary.h"

#include "AsyncLoadScreen.h"

void UBlueprintLibrary::PlayLoadingScreen(bool PlayUntilStopped, float PlayTime)
{
	IAsyncLoadScreenModule& LoadingScreenModule = IAsyncLoadScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(PlayUntilStopped,PlayTime);
}

void UBlueprintLibrary::StopLoadingScreen()
{
	IAsyncLoadScreenModule& LoadingScreenModule = IAsyncLoadScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}
