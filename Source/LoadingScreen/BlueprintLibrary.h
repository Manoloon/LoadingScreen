#pragma once
#include "LoadingScreen.h"
#include "BlueprintLibrary.generated.h"

UCLASS()
class UBlueprintLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void PlayLoadingScreen(bool PlayUntilStopped, float PlayTime, 
	UUserWidget* LoadingWidget =nullptr);
	UFUNCTION(BlueprintCallable)
	static void StopLoadingScreen();
	
};
