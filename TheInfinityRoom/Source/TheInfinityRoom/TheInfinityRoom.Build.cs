// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class TheInfinityRoom : ModuleRules
{
	public TheInfinityRoom(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks", "Niagara", "OnlineSubsystem", "OnlineSubsystemUtils" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PrivateIncludePaths.Add("TheInfinityRoom");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string SteamSDKPath = Path.Combine(ModuleDirectory, "../../ThirdParty/Steamworks/sdk");
            PublicIncludePaths.Add(Path.Combine(SteamSDKPath, "public", "steam"));
            PublicAdditionalLibraries.Add(Path.Combine(SteamSDKPath, "redistributable_bin", "win64", "steam_api64.lib"));
            RuntimeDependencies.Add(Path.Combine(SteamSDKPath, "redistributable_bin", "win64", "steam_api64.dll"));

            DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        }

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
