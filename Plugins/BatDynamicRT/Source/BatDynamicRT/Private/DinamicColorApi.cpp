// Fill out your copyright notice in the Description page of Project Settings.

#include "DinamicColorApi.h"
#include "DynamicColorShader.h"
#include "Classes/Engine/TextureRenderTarget2D.h"

static const uint32 kGridSubdivisionX = 32;
static const uint32 kGridSubdivisionY = 16;

static void DrawUVDisplacementToRenderTarget_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	const FDinamicColorApi& DinamicColorApi,
	const FName& TextureRenderTargetName,
	FTextureRenderTargetResource* OutTextureRenderTargetResource,
	ERHIFeatureLevel::Type FeatureLevel)
{
	check(IsInRenderingThread());

#if WANTS_DRAW_MESH_EVENTS
	FString EventName;
	TextureRenderTargetName.ToString(EventName);
	SCOPED_DRAW_EVENTF(RHICmdList, SceneCapture, TEXT("DynamicColorGeneration %s"), *EventName);
#else
	SCOPED_DRAW_EVENT(RHICmdList, DrawUVDisplacementToRenderTarget_RenderThread);
#endif

	// Set render target.
	SetRenderTarget(
		RHICmdList,
		OutTextureRenderTargetResource->GetRenderTargetTexture(),
		FTextureRHIRef(),
		ESimpleRenderTargetMode::EUninitializedColorAndDepth,
		FExclusiveDepthStencil::DepthNop_StencilNop);

	FIntPoint DisplacementMapResolution(OutTextureRenderTargetResource->GetSizeX(), OutTextureRenderTargetResource->GetSizeY());

	// Update viewport.
	RHICmdList.SetViewport(
		0, 0, 0.f,
		DisplacementMapResolution.X, DisplacementMapResolution.Y, 1.f);

	// Get shaders.
	TShaderMap<FGlobalShaderType>* GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
	TShaderMapRef< DynamicColorVS > VertexShader(GlobalShaderMap);
	TShaderMapRef< DynamicColorPS > PixelShader(GlobalShaderMap);

	// Set the graphic pipeline state.
	FGraphicsPipelineStateInitializer GraphicsPSOInit;
	RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
	GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
	GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
	GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
	GraphicsPSOInit.PrimitiveType = PT_TriangleList;
	GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GetVertexDeclarationFVector4();
	GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
	GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
	SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

	// Update viewport.
	RHICmdList.SetViewport(
		0, 0, 0.f,
		OutTextureRenderTargetResource->GetSizeX(), OutTextureRenderTargetResource->GetSizeY(), 1.f);

//	// Update shader uniform parameters.
//	VertexShader->SetParameters(RHICmdList, VertexShader->GetVertexShader(), CompiledCameraModel, DisplacementMapResolution);
//	PixelShader->SetParameters(RHICmdList, PixelShader->GetPixelShader(), CompiledCameraModel, DisplacementMapResolution);

	// Call our function to set up parameters
	PixelShader->SetColor(RHICmdList, DinamicColorApi.DynamicColor);


	// Draw grid.
	uint32 PrimitiveCount = kGridSubdivisionX * kGridSubdivisionY * 2;
	RHICmdList.DrawPrimitive(PT_TriangleList, 0, PrimitiveCount, 1);

	// Resolve render target.
	RHICmdList.CopyToResolveTarget(
		OutTextureRenderTargetResource->GetRenderTargetTexture(),
		OutTextureRenderTargetResource->TextureRHI,
		false, FResolveParams());
}

FDinamicColorApi::FDinamicColorApi()
{
}

void FDinamicColorApi::DrawUVToRenderTarget(UWorld* World, const FLinearColor & Color, UTextureRenderTarget2D * OutputRenderTarget) const
{
	const FName TextureRenderTargetName = OutputRenderTarget->GetFName();
	FTextureRenderTargetResource* TextureRenderTargetResource = OutputRenderTarget->GameThread_GetRenderTargetResource();

	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();

	FDinamicColorApi DinamicColorApi_copy = *this;
	DinamicColorApi_copy.DynamicColor = Color;

	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[DinamicColorApi_copy, TextureRenderTargetResource, TextureRenderTargetName, FeatureLevel](FRHICommandListImmediate& RHICmdList)
	{
		DrawUVDisplacementToRenderTarget_RenderThread(
			RHICmdList,
			DinamicColorApi_copy,
			TextureRenderTargetName,
			TextureRenderTargetResource,
			FeatureLevel);
	}
	);
}
