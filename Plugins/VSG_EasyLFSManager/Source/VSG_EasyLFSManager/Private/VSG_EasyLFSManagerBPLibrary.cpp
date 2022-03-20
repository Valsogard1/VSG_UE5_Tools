// Copyright Epic Games, Inc. All Rights Reserved.

#include "VSG_EasyLFSManagerBPLibrary.h"
#include "VSG_EasyLFSManager.h"

//~~~ Image Wrapper ~~~
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
//~~~ Image Wrapper ~~~

UVSG_EasyLFSManagerBPLibrary::UVSG_EasyLFSManagerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

int UVSG_EasyLFSManagerBPLibrary::GetFileSize(FString FilePath)
{
	IFileManager& Manager = IFileManager::Get();
	float FileSize = Manager.FileSize(*FilePath);

	return FileSize;
}

void UVSG_EasyLFSManagerBPLibrary::SetMaterialInstanceStaticSwitchParameterValue(UMaterialInstance* Instance, FName ParameterName, bool Value)
{
	FStaticParameterSet StaticParameters = Instance->GetStaticParameters();
	for (auto& SwitchParameter : StaticParameters.StaticSwitchParameters)
	{
		if (SwitchParameter.ParameterInfo.Name == ParameterName)
		{
			SwitchParameter.Value = Value;
			break;;
		}
	}
	Instance->UpdateStaticPermutation(StaticParameters);
}

void UVSG_EasyLFSManagerBPLibrary::CreateProcess(int32& ProcessId, FString FullPathOfProgramToRun, TArray<FString> CommandlineArguments, bool Detach, bool Hidden, int32 Priority, FString OptionalWorkingDirectory)
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

void UVSG_EasyLFSManagerBPLibrary::SaveStringToFile(FString SaveDirectory, FString FileName, FString StringToSave, bool AllowOverWriting, FString& Report, bool& Success)
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

void UVSG_EasyLFSManagerBPLibrary::LoadStringFromFile(FString SaveDirectory, FString FileName, FString& Report, bool& Success, FString& StringInFile)
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

void UVSG_EasyLFSManagerBPLibrary::SaveStringArrayToFile(FString SaveDirectory, FString FileName, TArray<FString> ArrayToSave, bool AllowOverWriting, FString& Report, bool& Success)
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

void UVSG_EasyLFSManagerBPLibrary::LoadStringArrayFromFile(FString SaveDirectory, FString FileName, FString& Report, bool& Success, TArray<FString>& ArrayInFile)
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

void UVSG_EasyLFSManagerBPLibrary::CopyFile(FString CopyFrom, FString CopyTo)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.CopyFile(*CopyTo, *CopyFrom);

}

void UVSG_EasyLFSManagerBPLibrary::DeleteFile(bool& Success, FString File)
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

void UVSG_EasyLFSManagerBPLibrary::MoveFile(FString MoveFrom, FString MoveTo)
{
	CopyFile(MoveFrom, MoveTo);
	bool t = true;
	DeleteFile(t, MoveFrom);
}

// END File Utilities






// START Load Texture 2D From File

static TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString InImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	if (InImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (InImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	else if (InImagePath.EndsWith(".ico"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
	}
	else if (InImagePath.EndsWith(".exr"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
	}
	else if (InImagePath.EndsWith(".icns"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
	}

	return nullptr;
}

UTexture2D* UVSG_EasyLFSManagerBPLibrary::LoadTexture2D_FromFileByExtension(const FString& ImagePath, bool& IsValid, int32& OutWidth, int32& OutHeight)
{
	UTexture2D* Texture = nullptr;
	IsValid = false;

	// To avoid log spam, make sure it exists before doing anything else.
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
	{
		return nullptr;
	}

	TArray<uint8> CompressedData;
	if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
	{
		return nullptr;
	}

	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(ImagePath);

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
	{
		TArray<uint8> UncompressedRGBA;

		if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
		{
			Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);

			if (Texture != nullptr)
			{
				IsValid = true;

				OutWidth = ImageWrapper->GetWidth();
				OutHeight = ImageWrapper->GetHeight();

				void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num());
				Texture->PlatformData->Mips[0].BulkData.Unlock();
				Texture->UpdateResource();
			}
		}
	}

	return Texture;
}

// END Load Texture 2D From File