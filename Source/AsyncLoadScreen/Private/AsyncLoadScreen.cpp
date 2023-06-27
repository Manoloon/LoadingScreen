﻿#include "AsyncLoadScreen.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"
#include "Widgets/Images/SThrobber.h"

struct FAsyncLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FAsyncLoadingScreenBrush(const FName InTextureName, const FVector2D & InImageSize)
		: FSlateDynamicImageBrush(InTextureName,InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(nullptr,*InTextureName.ToString()));
	}
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		if(UObject* CachedResourceObj = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObj);
		}
	}
};

class SAsyncLoadScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAsyncLoadScreen){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		//Debemos cargar un logo antes de todo , ya que es lo primero en cargar.
		static const FName LoadingScreenName(TEXT("/Game/UI/loading"));
		LoadingScreenBrush = MakeShareable(new FAsyncLoadingScreenBrush(LoadingScreenName,FVector2D(1408,792)));
		FSlateBrush *BGBrush = new FSlateBrush();
		BGBrush->TintColor= FLinearColor(0.04f,0.04f,0.04f,1.0f);

		ChildSlot
			[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)	
				.BorderImage(BGBrush)
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(FMargin(10.0f))
				[
					SNew(SThrobber)
					.Visibility(this, &SAsyncLoadScreen::GetLoadIndicatorVisibility)
				]
			]
		];
	}
private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		//bool Vis = GetMoviePlayer()->IsLoadingFinished();
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}
	
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

class FAsyncLoadScreenModule : public IAsyncLoadScreenModule
{
public:
	virtual void StartupModule() override
	{
		// Forzamos la referencia del logo.
		LoadObject<UObject>(nullptr,TEXT("/Game/UI/loading"));

		if(IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}
	// This function should derive from a functionlibrary .
	virtual void StartInGameLoadingScreen(bool PlayUntilStopped, float PlayTime) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !PlayUntilStopped;
		LoadingScreen.bWaitForManualStop = PlayUntilStopped;
		LoadingScreen.bAllowEngineTick = PlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SNew(SAsyncLoadScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 3.f;
		//LoadingScreen.WidgetLoadingScreen = SNew(SAsyncLoadScreen);
		LoadingScreen.WidgetLoadingScreen = SNew(SAsyncLoadScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
};

IMPLEMENT_GAME_MODULE(FAsyncLoadScreenModule, AsyncLoadScreen);
