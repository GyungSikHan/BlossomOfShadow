// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPG : ModuleRules
{
	public RPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);
        PublicDependencyModuleNames.Add("Core");

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("InputCore");
        PrivateDependencyModuleNames.Add("EnhancedInput");

        PublicDependencyModuleNames.Add("Niagara");
        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("GameplayTasks");
        PublicDependencyModuleNames.Add("NavigationSystem");
        PublicDependencyModuleNames.Add("UMG");
        PublicDependencyModuleNames.Add("ProceduralMeshComponent");
        PublicDependencyModuleNames.Add("SlateCore");
        PublicDependencyModuleNames.Add("GameplayCameras");

        //½Ã³×¸¶Æ½
        PublicDependencyModuleNames.Add("MovieScene");
        PublicDependencyModuleNames.Add("LevelSequence");
    }
}
