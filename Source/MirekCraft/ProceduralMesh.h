// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMesh.generated.h"

UENUM()
enum class EBlockType : uint8
{
	GRASS, DIRT, STONE, UNBREAKABLE, AIR
};
UENUM()
enum class ECubeSide : uint8
{
	BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK
};

UCLASS()
class MIREKCRAFT_API AProceduralMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	const float BlockSize = 100.0f;

	//all possible vertices
	const FVector P0{ -BlockSize/2, -BlockSize/2, BlockSize/2 };
	const FVector P1{ BlockSize/2, -BlockSize/2, BlockSize/2 };
	const FVector P2{ BlockSize/2, -BlockSize/2, -BlockSize/2 };
	const FVector P3{ -BlockSize/2, -BlockSize/2, -BlockSize/2 };
	const FVector P4{ -BlockSize/2, BlockSize/2, BlockSize/2 };
	const FVector P5{ BlockSize/2, BlockSize/2, BlockSize/2 };
	const FVector P6{ BlockSize/2, BlockSize/2, -BlockSize/2 };
	const FVector P7{ -BlockSize/2, BlockSize/2, -BlockSize/2 };

	const TArray<TArray<FVector2D>> BlockUVs =
	{
		/*GRASS TOP*/		{FVector2D{0.125f, 0.375f}, FVector2D{0.1875f, 0.375f}, FVector2D{0.125f, 0.4375f},FVector2D{0.1875f, 0.4375f}},
		/*GRASS SIDE*/		{FVector2D{0.1875f, 0.9375f}, FVector2D{0.25f, 0.9375f}, FVector2D{0.1875f, 1.0f},FVector2D{0.25f, 1.0f}},
		/*DIRT*/			{FVector2D{0.125f, 0.9375f}, FVector2D{0.1875f, 0.9375f}, FVector2D{0.125f, 1.0f},FVector2D{0.1875f, 1.0f}},
		/*STONE*/			{FVector2D{0.0f, 0.875f}, FVector2D{0.0625f, 0.875f}, FVector2D{0.0f, 0.9375f},FVector2D{0.0625f, 0.9375f}},
		/*UNBREAKABLE*/	    {FVector2D{0.0625f, 0.5f}, FVector2D{0.125f, 0.5f}, FVector2D{0.0625f, 0.5625f},FVector2D{0.125f, 0.5625f}},

		/*NOCRACK*/         {FVector2D{0.6875f, 0.0f}, FVector2D{0.75f, 0.0f}, FVector2D{0.6875f, 0.0625f},FVector2D{0.75f, 0.0625f}},
		/*CRACK1*/          {FVector2D{0.0625f, 0.0f}, FVector2D{0.125f, 0.0f}, FVector2D{0.0625f, 0.0625f},FVector2D{0.125f, 0.0625f}},
		/*CRACK2*/          {FVector2D{0.125f, 0.0f}, FVector2D{0.1875f, 0.0f}, FVector2D{0.125f, 0.0625f},FVector2D{0.1875f, 0.0625f}},
		/*CRACK3*/          {FVector2D{0.1875f, 0.0f}, FVector2D{0.25f, 0.0f}, FVector2D{0.1875f, 0.0625f},FVector2D{0.25f, 0.0625f}}
	};

	EBlockType BlockType = EBlockType::AIR;

	class AChunk* SpawnedChunk = nullptr;

	TArray<FVector> AllVertices;
	TArray<FVector> AllNormals;
	TArray<int32> AllTriangles;
	TArray<FVector2D> AllUVs;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* Material = nullptr;

	void CreateQuad(ECubeSide Side, FVector& Position);
	void CreateCube(FVector& Position);
	TArray<FVector2D> SetUVsByBlockType(ECubeSide Side);
	void BuildChunk(int32 SizeX, int32 SizeY, int32 SizeZ);
};
