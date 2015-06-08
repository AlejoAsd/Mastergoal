// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class Mastergoal : ModuleRules
{
	private string ModulePath
    {
        get { return Path.GetDirectoryName( RulesCompiler.GetModuleFilename( this.GetType().Name ) ); }
    }

	private string EvoGoalPath
    {
        get { return Path.GetFullPath( Path.Combine( ModulePath, "../../EvoGoal/" ) ); }
    }

	public Mastergoal(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemUtils" });

		LoadEvoGoal(Target);
	}

	public bool LoadEvoGoal(TargetInfo Target)
    {
        bool isLibrarySupported = false;
 
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;
 
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(EvoGoalPath, "lib");
 
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "EvoGoal." + PlatformString + ".lib")); 
        }
 
        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add( Path.Combine(EvoGoalPath, "Includes") );
        }
 
        Definitions.Add(string.Format("WITH_EVOGOAL_BINDING={0}", isLibrarySupported ? 1 : 0));
 
        return isLibrarySupported;
    }
}
