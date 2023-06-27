#pragma once
#include "LoadingScreen.h"
#include "BlueprintLibrary.generated.h"

UCLASS()
class UBlueprintLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void PlayLoadingScreen(bool PlayUntilStopped, float PlayTime);
	UFUNCTION(BlueprintCallable)
	static void StopLoadingScreen();
	
};
