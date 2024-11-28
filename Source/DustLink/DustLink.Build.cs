// Copyright Dustbyte Software. All Rights Reserved.

using UnrealBuildTool;

// ReSharper disable UseCollectionExpression

/// <summary>
/// Build rules for the DustLink module. <br/>
/// This class defines the dependencies and build settings required for the DustLink module.
/// </summary>
public class DustLink : ModuleRules
{
	/// <summary>
	/// Constructor for the DustLink module. <br/>
	/// Sets up the module's precompiled headers and manages its public and private dependencies.
	/// </summary>
	/// <param name="Target">The build target rules for this module.</param>
	public DustLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"OnlineSubsystem",
				"OnlineSubsystemSteam"
			}
		);
		
		PrivateDependencyModuleNames.AddRange(new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
			}
		);
	}
}