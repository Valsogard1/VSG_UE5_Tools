// Copyright Epic Games, Inc. All Rights Reserved.

#include "VSG_GitManager.h"

#define LOCTEXT_NAMESPACE "FVSG_GitManagerModule"

void FVSG_GitManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FVSG_GitManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVSG_GitManagerModule, VSG_GitManager)