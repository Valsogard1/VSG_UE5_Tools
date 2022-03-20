// Copyright Epic Games, Inc. All Rights Reserved.

#include "VSG_EasyLFSManager.h"

#define LOCTEXT_NAMESPACE "FVSG_EasyLFSManagerModule"

void FVSG_EasyLFSManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FVSG_EasyLFSManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVSG_EasyLFSManagerModule, VSG_EasyLFSManager)