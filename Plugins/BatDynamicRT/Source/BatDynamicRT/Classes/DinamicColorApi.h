// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DinamicColorApi.generated.h"


USTRUCT(BlueprintType)
struct FDinamicColorApi
{
	GENERATED_USTRUCT_BODY()
	FDinamicColorApi();

	void DrawUVToRenderTarget(class UWorld* World, const FLinearColor& Color, class UTextureRenderTarget2D* OutputRenderTarget) const;

	bool operator != (const FDinamicColorApi& Other) const
	{
		return this != &Other;
	}

	FLinearColor DynamicColor;
};
