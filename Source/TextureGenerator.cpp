// Fill out your copyright notice in the Description page of Project Settings.

#include "CurveImporter.h"
#include "ImageUtils.h"
#include "Misc/FileHelper.h"
#include "Misc/FeedbackContext.h"
#include "MyObject.h"
#include "XmlFile.h"
#include "Math/UnrealMathUtility.h"



UCurveImporter::UCurveImporter()
{
	bEditorImport = true;	
	//bCreateNew = true;
	SupportedClass = UTexture2D::StaticClass();	
	Formats.Add(FString("foo;Formato foo"));
}


UObject* UCurveImporter::FactoryCreateFile(UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	const FString& Filename,
	const TCHAR* Params,
	FFeedbackContext* Warn,
	bool& bOutOperationCanceled)
{	
	FXmlFile SourceFile;
	bool bParsed = SourceFile.LoadFile(Filename, EConstructMethod::ConstructFromFile);	

	/* side size of the final texture */
	int TextureSize = 32;

	/* pixels that will be drawn */
	TArray<FColor> Pixels;

	if (bParsed)
	{
		FXmlNode* Root = SourceFile.GetRootNode();		

		TextureSize = FCString::Atoi(*Root->GetAttribute(TEXT("size")));
		Pixels.AddZeroed(TextureSize * TextureSize);
		
		TArray<FXmlNode*> Layers = Root->GetFirstChildNode()->GetChildrenNodes();
		
		for (FXmlNode* Layer : Layers)
		{		
			FString Type = Layer->GetAttribute(TEXT("type"));
			UE_LOG(LogTemp, Error, TEXT("Node here!"));
			if (Type.Equals(TEXT("PerlinNoise")))
			{
				UE_LOG(LogTemp, Error, TEXT("perlin found!"));
				PerlinNoise PerlinNoiseData;
				TArray<FXmlNode*> Values = Layer->GetChildrenNodes();

				for (FXmlNode* Param : Values)
				{
					if (Param->GetTag().Equals(TEXT("frequency_x")))
					{
						PerlinNoiseData.FreqX = FCString::Atof(*Param->GetContent());
					}

					if (Param->GetTag().Equals(TEXT("frequency_y")))
					{
						PerlinNoiseData.FreqY = FCString::Atof(*Param->GetContent());
					}

					if (Param->GetTag().Equals(TEXT("offset_x")))
					{
						PerlinNoiseData.OffsetX = FCString::Atof(*Param->GetContent());
					}

					if (Param->GetTag().Equals(TEXT("offset_y")))
					{
						UE_LOG(LogTemp, Error, TEXT("offset!"));
						PerlinNoiseData.OffsetY = FCString::Atof(*Param->GetContent());
					}

					if (Param->GetTag().Equals(TEXT("color")))
					{
						UE_LOG(LogTemp, Error, TEXT("Color!"));
						TArray<FXmlNode*> Color = Param->GetChildrenNodes();

						PerlinNoiseData.Color = { FCString::Atoi(*Color[0]->GetContent()),
												FCString::Atoi(*Color[1]->GetContent()),
												FCString::Atoi(*Color[2]->GetContent()),
												FCString::Atoi(*Color[3]->GetContent()) };
					}
				}

				PopulatePixelLayer(PerlinNoiseData, TextureSize, Pixels);

				UE_LOG(LogTemp, Error, TEXT("PerlinNose pixel %i"), Pixels[0].A);
			}
		}
	}
	else 
	{
		Warn->StatusForceUpdate(1, 1, FText::FromString(FString("File not in the correct format")));
		bOutOperationCanceled = true;
	}
	
	FCreateTexture2DParameters Param;

	return FImageUtils::CreateTexture2D(TextureSize, TextureSize, Pixels,InParent, InName.ToString(), Flags, Param);
}

void UCurveImporter::PopulatePixelLayer(const PerlinNoise InNoiseData, const int32 InTextureSize, TArray<FColor> &InPixels)
{
	for (size_t Index = 0; Index < InPixels.Num(); Index++)
	{
		int32 X = Index % InTextureSize;
		int32 Y = FMath::Floor(Index / InTextureSize);

		float PerlinNoise = FMath::Abs(FMath::PerlinNoise2D(FVector2D(InNoiseData.FreqX * (X)+InNoiseData.OffsetX, InNoiseData.FreqY * Y + InNoiseData.OffsetY)));
	
		InPixels[Index].R = InNoiseData.Color[0] * PerlinNoise;
		InPixels[Index].G = InNoiseData.Color[1] * PerlinNoise;
		InPixels[Index].B = InNoiseData.Color[2] * PerlinNoise;
		InPixels[Index].A = InNoiseData.Color[3];
	}
}

//UObject* UCurveImporter::FactoryCreateFile(UClass* InClass,
//	UObject* InParent,
//	FName InName,
//	EObjectFlags Flags,
//	const FString& Filename,
//	const TCHAR* Params,
//	FFeedbackContext* Warn,
//	bool& bOutOperationCanceled)
//{
//	FString Question = TEXT("Ecco la prova");
//	//Warn->YesNof(FText::FromString(Question));
//	TArray<FString> Lines;
//	FFileHelper::LoadFileToStringArray(Lines, *Filename);
//
//	float Time = 0.0f;
//	int Index = 0;
//	UCurveFloat* Curve = NewObject<UCurveFloat>(InParent, InName, Flags);
//	for (const FString& Line : Lines)
//	{
//		TArray<FString> TimeValue;
//		Line.ParseIntoArray(TimeValue, TEXT(" "));
//		if (TimeValue.Num() == 2)
//		{
//			Time = FCString::Atof(*TimeValue[0]);			
//			float NewKey = FCString::Atof(*TimeValue[1]);
//			if (NewKey == 0.0f)
//			{
//				if (Warn->YesNof(FText::FromString(Question)))
//				{
//					bOutOperationCanceled = true;
//					return nullptr;
//				}
//			}
//
//			Curve->FloatCurve.AddKey(Time, NewKey);			
//			Index++;
//			Warn->StatusForceUpdate(Index, Lines.Num(), FText::FromString(FString("Updating")));
//		}		
//	}
//
//	return Curve;
//}