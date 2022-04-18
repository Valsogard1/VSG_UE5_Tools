// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VSG_GitManagerBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UVSG_GitManagerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "VSG_GitManager sample test testing"), Category = "VSG_GitManagerTesting")
	static float VSG_GitManagerSampleFunction(float Param);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get File Size", ToolTip = "Returns the size of file in Bytes", Keywords = "Panda Balanar"), Category = "Valsogard Enterprise | Utilities")
		static int GetFileSize(FString FilePath);

	UFUNCTION(BlueprintCallable, Category = "Valsogard Enterprise | System")
		static void CreateProcess(int32& ProcessId, FString FullPathOfProgramToRun, TArray<FString> CommandlineArguments, bool Detach, bool Hidden, int32 Priority = 0, FString OptionalWorkingDirectory = "");


	// START File Utilities

	UFUNCTION(BlueprintCallable, Category = "Valsogard Enterprise | File Utilities", meta = (Keywords = "save, string, file, vsg"))
		static void SaveStringToFile(FString SaveDirectory, FString FileName, FString StringToSave, bool AllowOverWriting, FString& Report, bool& Success);

	UFUNCTION(BlueprintCallable, Category = "Valsogard Enterprise | File Utilities", meta = (Keywords = "load, string, file, vsg"))
		static void LoadStringFromFile(FString SaveDirectory, FString FileName, FString& Report, bool& Success, FString& StringInFile);

	UFUNCTION(BlueprintCallable, Category = "Valsogard Enterprise | File Utilities", meta = (Keywords = "save, string, file, vsg"))
		static void SaveStringArrayToFile(FString SaveDirectory, FString FileName, TArray<FString> ArrayToSave, bool AllowOverWriting, FString& Report, bool& Success);

	UFUNCTION(BlueprintCallable, Category = "Valsogard Enterprise | File Utilities", meta = (Keywords = "load, string, file, vsg"))
		static void LoadStringArrayFromFile(FString SaveDirectory, FString FileName, FString& Report, bool& Success, TArray<FString>& ArrayInFile);

	UFUNCTION(BlueprintCallable, Category = "Valsogard Enterprise | File Utilities", meta = (Keywords = "copy, file, vsg"))
		static void CopyFile(FString CopyFrom, FString CopyTo);

	UFUNCTION(BlueprintCallable, Category = "Valsogard Enterprise | File Utilities", meta = (Keywords = "delete, file, vsg"))
		static void DeleteFile(bool& Success, FString File);

	UFUNCTION(BlueprintCallable, Category = "Valsogard Enterprise | File Utilities", meta = (Keywords = "move, file, vsg"))
		static void MoveFile(FString MoveFrom, FString MoveTo);

	// END File Utilities
};
