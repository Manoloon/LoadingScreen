#pragma once
#include "BlueprintLibrary.generated.h"

class UUserWidget;

UCLASS()
class UBlueprintLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void PlayLoadingScreen(bool PlayUntilStopped, float PlayTime,UUserWidget* LoadingWidget);
	UFUNCTION(BlueprintCallable)
	static void StopLoadingScreen();
};
