// Copyright Epic Games, Inc. All Rights Reserved.

#include "VSG_GitManagerBPLibrary.h"
#include "VSG_GitManager.h"

UVSG_GitManagerBPLibrary::UVSG_GitManagerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UVSG_GitManagerBPLibrary::VSG_GitManagerSampleFunction(float Param)
{
	return -1;
}

int UVSG_GitManagerBPLibrary::GetFileSize(FString FilePath)
{
	IFileManager& Manager = IFileManager::Get();
	float FileSize = Manager.FileSize(*FilePath);

	return FileSize;
}

void UVSG_GitManagerBPLibrary::CreateProcess(int32& ProcessId, FString FullPathOfProgramToRun, TArray<FString> CommandlineArguments, bool Detach, bool Hidden, int32 Priority, FString OptionalWorkingDirectory)
{
	//Please note ProcessId should really be uint32 but that is not supported by BP yet

	FString Args = "";
	if (CommandlineArguments.Num() > 1)
	{
		Args = CommandlineArguments[0];
		for (int32 v = 1; v < CommandlineArguments.Num(); v++)
		{
			Args += " " + CommandlineArguments[v];
		}
	}
	else if (CommandlineArguments.Num() > 0)
	{
		Args = CommandlineArguments[0];
	}

	uint32 NeedBPUINT32 = 0;
	FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		*FullPathOfProgramToRun,
		*Args,
		Detach,//* @param bLaunchDetached		if true, the new process will have its own window
		false,//* @param bLaunchHidded			if true, the new process will be minimized in the task bar
		Hidden,//* @param bLaunchReallyHidden	if true, the new process will not have a window or be in the task bar
		&NeedBPUINT32,
		Priority,
		(OptionalWorkingDirectory != "") ? *OptionalWorkingDirectory : nullptr,//const TCHAR* OptionalWorkingDirectory, 
		nullptr
	);

	//Not sure what to do to expose UINT32 to BP
	ProcessId = NeedBPUINT32;
}

// START File Utilities

void UVSG_GitManagerBPLibrary::SaveStringToFile(FString SaveDirectory, FString FileName, FString StringToSave, bool AllowOverWriting, FString& Report, bool& Success)
{

	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting)
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			Success = false;
			Report = FString::Printf(TEXT("ERROR File Can Not Be Overwritten"));
			return;
		}
	}

	Success = FFileHelper::SaveStringToFile(StringToSave, *SaveDirectory);
	int64 FileSizeOnDisk = FPlatformFileManager::Get().GetPlatformFile().FileSize(*SaveDirectory);

	if (FileSizeOnDisk > 1000000)
	{
		int64 MBSize = (FileSizeOnDisk / 1000000);
		Report = FString::Printf(TEXT("SUCCESS Saved File that is %i MB"), MBSize);
	}
	else
	{
		Report = FString::Printf(TEXT("SUCCESS Saved File that is %i Bytes"), FileSizeOnDisk);
	}
}

void UVSG_GitManagerBPLibrary::LoadStringFromFile(FString SaveDirectory, FString FileName, FString& Report, bool& Success, FString& StringInFile)
{

	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
	{
		StringInFile = FString::Printf(TEXT("ERROR File Can Not Be Found"));
		Report = FString::Printf(TEXT("ERROR File Can Not Be Found"));
		Success = false;
		return;
	}

	int64 FileSizeOnDisk = FPlatformFileManager::Get().GetPlatformFile().FileSize(*SaveDirectory);

	if (FileSizeOnDisk > 1000000)
	{
		int64 MBSize = (FileSizeOnDisk / 1000000);
		Report = FString::Printf(TEXT("SUCCESS Loaded File that is %i MB"), MBSize);
	}
	else
	{
		Report = FString::Printf(TEXT("SUCCESS Loaded File that is %i Bytes"), FileSizeOnDisk);
	}
	FFileHelper::LoadFileToString(StringInFile, *SaveDirectory);
	Success = true;


}

void UVSG_GitManagerBPLibrary::SaveStringArrayToFile(FString SaveDirectory, FString FileName, TArray<FString> ArrayToSave, bool AllowOverWriting, FString& Report, bool& Success)
{
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting)
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			Success = false;
			Report = FString::Printf(TEXT("ERROR File Can Not Be Overwritten"));
			return;
		}
	}

	Success = FFileHelper::SaveStringArrayToFile(ArrayToSave, *SaveDirectory);

	int64 FileSizeOnDisk = FPlatformFileManager::Get().GetPlatformFile().FileSize(*SaveDirectory);

	if (FileSizeOnDisk > 1000000)
	{
		int64 MBSize = (FileSizeOnDisk / 1000000);
		Report = FString::Printf(TEXT("SUCCESS Saved File that is %i MB"), MBSize);
	}
	else
	{
		Report = FString::Printf(TEXT("SUCCESS Saved File that is %i Bytes"), FileSizeOnDisk);
	}
}

void UVSG_GitManagerBPLibrary::LoadStringArrayFromFile(FString SaveDirectory, FString FileName, FString& Report, bool& Success, TArray<FString>& ArrayInFile)
{
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
	{
		TArray<FString> FileData;
		ArrayInFile = FileData;
		Report = FString::Printf(TEXT("ERROR File Can Not Be Found"));
		Success = false;
		return;
	}

	int64 FileSizeOnDisk = FPlatformFileManager::Get().GetPlatformFile().FileSize(*SaveDirectory);

	if (FileSizeOnDisk > 1000000)
	{
		int64 MBSize = (FileSizeOnDisk / 1000000);
		Report = FString::Printf(TEXT("SUCCESS Loaded File that is %i MB"), MBSize);
	}
	else
	{
		Report = FString::Printf(TEXT("SUCCESS Loaded File that is %i Bytes"), FileSizeOnDisk);
	}

	TArray<FString> FileData;
	FFileHelper::LoadFileToStringArray(FileData, *SaveDirectory);
	ArrayInFile = FileData;
	Success = true;
}

void UVSG_GitManagerBPLibrary::CopyFile(FString CopyFrom, FString CopyTo)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.CopyFile(*CopyTo, *CopyFrom);

}

void UVSG_GitManagerBPLibrary::DeleteFile(bool& Success, FString File)
{
	FString Path = File;
	if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*Path))
	{
		Success = false;
	}
	else
	{
		Success = true;
	}
}

void UVSG_GitManagerBPLibrary::MoveFile(FString MoveFrom, FString MoveTo)
{
	CopyFile(MoveFrom, MoveTo);
	bool t = true;
	DeleteFile(t, MoveFrom);
}

// END File Utilities