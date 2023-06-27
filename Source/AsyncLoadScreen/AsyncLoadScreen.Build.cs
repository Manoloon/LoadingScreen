// async loading screen Pablo Sanchez

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class AsyncLoadScreen : ModuleRules
{
	public AsyncLoadScreen(ReadOnlyTargetRules target)
		: base(target)
	{
		PrivatePCHHeaderFile = "Public/AsyncLoadScreen.h";

		PCHUsage = PCHUsageMode.UseSharedPCHs;

		PrivateIncludePaths.Add("AsyncLoadScreen/Private");

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"MoviePlayer",
				"Core",
				"CoreUObject",
				"Engine"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Slate",
				"SlateCore",
				"InputCore"
			}
		);
	}
}