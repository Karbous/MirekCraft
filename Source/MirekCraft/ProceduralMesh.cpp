// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMesh.h"
#include "DrawDebugHelpers.h"
#include "ProceduralMeshComponent.h"
#include "Chunk.h"

// Sets default values
AProceduralMesh::AProceduralMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProceduralMesh::BeginPlay()
{
	Super::BeginPlay();

	BuildChunk(10, 10, 10);
}

// Called every frame
void AProceduralMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralMesh::CreateQuad(ECubeSide Side, FVector& Position)
{
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<int32> Triangles = { 3,0,1,3,1,2 };
	TArray<FVector2D> UVs = SetUVsByBlockType(Side);

	switch (Side)
	{
		case ECubeSide::FRONT:
			Vertices = { P0 + Position, P1 + Position, P2 + Position, P3 + Position };
			Normals.Init(FVector(0.0f, -1.0f, 0.0f), 4);
			break;
		case ECubeSide::BACK:
			Vertices = { P5 + Position, P4 + Position, P7 + Position, P6 + Position };
			Normals.Init(FVector(0.0f, 1.0f, 0.0f), 4);
			break;
		case ECubeSide::LEFT:
			Vertices = { P4 + Position, P0 + Position, P3 + Position, P7 + Position };
			Normals.Init(FVector(-1.0f, 0.0f, 0.0f), 4);
			break;
		case ECubeSide::RIGHT:
			Vertices = { P1 + Position, P5 + Position, P6 + Position, P2 + Position };
			Normals.Init(FVector(1.0f, 0.0f, 0.0f), 4);
			break;
		case ECubeSide::TOP:
			Vertices = { P4 + Position, P5 + Position, P1 + Position, P0 + Position };
			Normals.Init(FVector(0.0f, 0.0f, 1.0f), 4);
			break;
		case ECubeSide::BOTTOM:
			Vertices = { P6 + Position, P7 + Position, P3 + Position, P2 + Position };
			Normals.Init(FVector(0.0f, 0.0f, -1.0f), 4);
			break;
	}

	for (int16 i = 0; i < Triangles.Num(); i++)
	{
		AllTriangles.Add(Triangles[i] + AllVertices.Num());
	}

	for (int16 i = 0; i < Vertices.Num(); i++)
	{
		AllVertices.Add(Vertices[i]);
		AllNormals.Add(Normals[i]);
		AllUVs.Add(UVs[i]);
	}
}

void AProceduralMesh::CreateCube(FVector& Position)
{
	CreateQuad(ECubeSide::TOP, Position);
	CreateQuad(ECubeSide::BOTTOM, Position);
	CreateQuad(ECubeSide::LEFT, Position);
	CreateQuad(ECubeSide::RIGHT, Position);
	CreateQuad(ECubeSide::FRONT, Position);
	CreateQuad(ECubeSide::BACK, Position);
}

TArray<FVector2D> AProceduralMesh::SetUVsByBlockType(ECubeSide Side)
{
	TArray<FVector2D> UVs;
	UVs.Init(FVector2D(), 4);

	switch (BlockType)
	{
		case EBlockType::GRASS:
			if (Side == ECubeSide::TOP)
			{
				UVs[2] = BlockUVs[0][0];
				UVs[3] = BlockUVs[0][1];
				UVs[1] = BlockUVs[0][2];
				UVs[0] = BlockUVs[0][3];
			}
			else if (Side == ECubeSide::BOTTOM)
			{
				UVs[2] = BlockUVs[2][0];
				UVs[3] = BlockUVs[2][1];
				UVs[1] = BlockUVs[2][2];
				UVs[0] = BlockUVs[2][3];
			}
			else
			{
				UVs[2] = BlockUVs[1][0];
				UVs[3] = BlockUVs[1][1];
				UVs[1] = BlockUVs[1][2];
				UVs[0] = BlockUVs[1][3];
			}
			break;
		case EBlockType::DIRT:
			UVs[2] = BlockUVs[2][0];
			UVs[3] = BlockUVs[2][1];
			UVs[1] = BlockUVs[2][2];
			UVs[0] = BlockUVs[2][3];
			break;
		case EBlockType::STONE:
			UVs[2] = BlockUVs[3][0];
			UVs[3] = BlockUVs[3][1];
			UVs[1] = BlockUVs[3][2];
			UVs[0] = BlockUVs[3][3];
			break;
		case EBlockType::UNBREAKABLE:
			UVs[2] = BlockUVs[4][0];
			UVs[3] = BlockUVs[4][1];
			UVs[1] = BlockUVs[4][2];
			UVs[0] = BlockUVs[4][3];
			break;
		default:
			// error texture if block type unknown
			UVs[2] = BlockUVs[0][0];
			UVs[3] = BlockUVs[0][0];
			UVs[1] = BlockUVs[0][0];
			UVs[0] = BlockUVs[0][0];
			break;
	}

	return UVs;
}

void AProceduralMesh::BuildChunk(int32 SizeX, int32 SizeY, int32 SizeZ)
{
	SpawnedChunk = GetWorld()->SpawnActor<AChunk>(GetActorLocation(), GetActorRotation());

	BlockType = EBlockType::GRASS; // This won't be hardcoded in future

	for (int32 z = 0; z < SizeZ; z++)
	{
		for (int32 y = 0; y < SizeY; y++)
		{
			for (int32 x = 0; x < SizeX; x++)
			{
				FVector BlockPosition(x, y, z);
				BlockPosition *= BlockSize;

				CreateCube(BlockPosition);
			}
		}
	}

	if (SpawnedChunk->Mesh)
	{
		SpawnedChunk->Mesh->CreateMeshSection(0, AllVertices, AllTriangles, AllNormals, AllUVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

		if (Material)
		{
			SpawnedChunk->Mesh->SetMaterial(0, Material);
		}
	}

	AllVertices.Empty();
	AllNormals.Empty();
	AllTriangles.Empty();
	AllUVs.Empty();
}