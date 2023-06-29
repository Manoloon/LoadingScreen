#include "AsyncLoadScreen.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"
#include "Widgets/Images/SThrobber.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

struct FAsyncLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FAsyncLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(nullptr, *InTextureName.ToString()));
	}
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		if (UObject* CachedResourceObj = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObj);
		}
	}
};

class SAsyncStartScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAsyncStartScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenAsset(TEXT("/Game/UI/UELogo"));
		LoadingScreenBrush = MakeShareable(new FAsyncLoadingScreenBrush(LoadingScreenAsset, FVector2D(1726, 300)));
		static const FName ThrobberImageAsset(TEXT("/Game/UI/Throbber"));
		ThrobberImageBrush = MakeShareable(new FAsyncLoadingScreenBrush(ThrobberImageAsset, FVector2D(16, 16)));

		ChildSlot
			[SNew(SOverlay)
				+ SOverlay::Slot()
					  .HAlign(HAlign_Fill)
					  .VAlign(VAlign_Fill)
						  [SNew(SImage)
								  .ColorAndOpacity(FLinearColor::Black)]
				+ SOverlay::Slot()
					  .HAlign(HAlign_Center)
					  .VAlign(VAlign_Center)
						  [SNew(SImage)
								  .Image(LoadingScreenBrush.Get())]
				+ SOverlay::Slot()
					  .HAlign(HAlign_Fill)
					  .VAlign(VAlign_Fill)
						  [SNew(SVerticalBox)
							  + SVerticalBox::Slot()
									.VAlign(VAlign_Bottom)
									.HAlign(HAlign_Right)
									.Padding(FMargin(10.0f))
										[SNew(SThrobber)
												.PieceImage(ThrobberImageBrush.Get())
												.Animate(SThrobber::Horizontal)
												.NumPieces(4)
												.Visibility(this,
													&SAsyncStartScreen::GetLoadIndicatorVisibility)]]];
	}

private:
	// ReSharper disable once CppMemberFunctionMayBeStatic is used by .Visibility in line 65
	EVisibility GetLoadIndicatorVisibility() const
	{
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
	TSharedPtr<FSlateDynamicImageBrush> ThrobberImageBrush;
};

class FAsyncLoadScreenModule : public IAsyncLoadScreenModule
{
public:
	virtual void StartupModule() override
	{
		LoadObject<UObject>(nullptr, TEXT("/Game/UI/UELogo.png"));
		LoadObject<UObject>(nullptr, TEXT("/Game/UI/Throbber"));

		if (IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}
	// This function should derive from a functionlibrary .
	virtual void StartInGameLoadingScreen(bool PlayUntilStopped, float PlayTime,
		const TSharedPtr<SWidget> SlateWidget) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !PlayUntilStopped;
		LoadingScreen.bWaitForManualStop = PlayUntilStopped;
		LoadingScreen.bAllowEngineTick = PlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SlateWidget;
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
		LoadingScreen.WidgetLoadingScreen = SNew(SAsyncStartScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
};
#undef LOCTEXT_NAMESPACE
IMPLEMENT_GAME_MODULE(FAsyncLoadScreenModule, AsyncLoadScreen);