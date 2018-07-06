// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DinamicColorApi.h"
#include "DynamicColor_BL.generated.h"

/**
 * 
 */
UCLASS()
class BATDYNAMICRT_API UDynamicColor_BL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Dynamic color", meta = (WorldContext = "WorldContextObject"))
	static void DrawUVToRenderTarget(
			const UObject* WorldContextObject,
			const FDinamicColorApi& DinamicColorApi,
			const FLinearColor& Color,
			class UTextureRenderTarget2D* OutputRenderTarget
		);
	
	
};
