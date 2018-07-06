// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicColor_BL.h"

void UDynamicColor_BL::DrawUVToRenderTarget(const UObject* WorldContextObject, const FDinamicColorApi & DinamicColorApi, const FLinearColor & Color, UTextureRenderTarget2D * OutputRenderTarget)
{
	DinamicColorApi.DrawUVToRenderTarget(WorldContextObject->GetWorld(), Color, OutputRenderTarget);
}
