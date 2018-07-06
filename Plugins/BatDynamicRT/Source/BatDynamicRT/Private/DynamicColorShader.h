// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

///**
// * 
// */
//class DynamicColorShader
//{
//public:
//	DynamicColorShader();
//	~DynamicColorShader();
//};

#include "Classes/Engine/World.h"
#include "Public/GlobalShader.h"
#include "Public/PipelineStateCache.h"
#include "Public/RHIStaticStates.h"
#include "Public/SceneUtils.h"
#include "Public/SceneInterface.h"
#include "Public/ShaderParameterUtils.h"

class DynamicColorBase : public FGlobalShader
{
protected:
	DynamicColorBase() { }
	DynamicColorBase(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer) {}


	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return true;
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}
};

// This can go on a header or cpp file
class DynamicColorVS : public DynamicColorBase
{
	DECLARE_EXPORTED_SHADER_TYPE(DynamicColorVS, Global, /*MYMODULE_API*/);

	DynamicColorVS() {}
	DynamicColorVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}
};

class DynamicColorPS : public DynamicColorBase
{
	DECLARE_EXPORTED_SHADER_TYPE(DynamicColorPS, Global, /*MYMODULE_API*/);

	FShaderParameter MyColorParameter;

	DynamicColorPS() {}
	DynamicColorPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment);

	static bool ShouldCache(EShaderPlatform Platform)
	{
		// Could skip compiling for Platform == SP_METAL for example
		return true;
	}

	// FShader interface.
	virtual bool Serialize(FArchive& Ar) override;

public:
	void SetColor(FRHICommandList& RHICmdList, const FLinearColor& Color);
};
