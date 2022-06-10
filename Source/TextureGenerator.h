// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetTypeCategories.h"
#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "Factories/Factory.h"
#include "CurveImporter.generated.h"

struct PerlinNoise
{
	TArray<int32> Color;
	float FreqX;
	float FreqY;
	float OffsetX;
	float OffsetY;
};

/**
 * 
 */
UCLASS()
class SECONDPROJECT_API UCurveImporter : public UFactory
{
	GENERATED_BODY()

public:
	UCurveImporter();

	UObject* FactoryCreateFile
	(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		const FString& Filename,
		const TCHAR* Params,
		FFeedbackContext* Warn,
		bool& bOutOperationCanceled
	) override;
	
	void PopulatePixelLayer(const PerlinNoise InNoiseData, const int32 InTextureSize, TArray<FColor>& InPixels);

	FText GetDisplayName() const override { return FText::FromString(TEXT("Perlin Noise Generator")); }
	uint32 GetMenuCategories() const override { return EAssetTypeCategories::UI; }
};
