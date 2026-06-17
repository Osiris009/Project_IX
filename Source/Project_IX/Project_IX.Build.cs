// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_IX : ModuleRules
{
	public Project_IX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "CommonUI",
            "GameplayAbilities",  
			"GameplayTags",        
			"GameplayTasks",
            "CommonInput",
            "EnhancedInput",
            "PropertyPath"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore"
        });
        
    }
}
