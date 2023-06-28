#include "BlueprintLibrary.h"
#include "AsyncLoadScreen.h"
#include "Blueprint/UserWidget.h"

void UBlueprintLibrary::PlayLoadingScreen(bool PlayUntilStopped, float PlayTime,UUserWidget* LoadingWidget)
{
	if(!IsValid(LoadingWidget))
	{
	 return;
	}
	IAsyncLoadScreenModule& LoadingScreenModule = IAsyncLoadScreenModule::Get();
	const TSharedRef<SWidget> LoadingScreen = LoadingWidget->TakeWidget();
	LoadingScreenModule.StartInGameLoadingScreen(PlayUntilStopped,PlayTime,LoadingScreen);
}

void UBlueprintLibrary::StopLoadingScreen()
{
	IAsyncLoadScreenModule& LoadingScreenModule = IAsyncLoadScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}
