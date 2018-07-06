// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicColorShader.h"

//DynamicColorShader::DynamicColorShader()
//{
//}
//
//DynamicColorShader::~DynamicColorShader()
//{
//}

DynamicColorVS::DynamicColorVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: DynamicColorBase(Initializer)
{

}

DynamicColorPS::DynamicColorPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: DynamicColorBase(Initializer)
{
	MyColorParameter.Bind(Initializer.ParameterMap, TEXT("DynamicColor"), SPF_Mandatory);
}

void DynamicColorPS::ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
{
	FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	// Add your own defines for the shader code
	OutEnvironment.SetDefine(TEXT("MY_DEFINE"), 1);
}


// FShader interface.
bool DynamicColorPS::Serialize(FArchive& Ar)
{
	bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
	Ar << MyColorParameter;
	return bShaderHasOutdatedParameters;
}

void DynamicColorPS::SetColor(FRHICommandList& RHICmdList, const FLinearColor& Color)
{
	SetShaderValue(RHICmdList, GetPixelShader(), MyColorParameter, Color);
}


IMPLEMENT_SHADER_TYPE(, DynamicColorVS, TEXT("/Plugin/BatDynamicRT/Private/DynamicColor.usf"), TEXT("MainVS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, DynamicColorPS, TEXT("/Plugin/BatDynamicRT/Private/DynamicColor.usf"), TEXT("MainPS"), SF_Pixel);

