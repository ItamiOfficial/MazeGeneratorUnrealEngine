// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeBuilder.h"
#include "MazeData.h"
#include "MazeGen.h"

// Sets default values
AMazeBuilder::AMazeBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup Wall
	Walls = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Walls"));
}

// Called when the game starts or when spawned
void AMazeBuilder::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMazeBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeBuilder::BuildMaze(UMazeGen* MazeGenerator)
{
	// Init ISMC
	Walls->SetStaticMesh(MazeVisualData->WallMesh);
	Walls->SetMaterial(0, MazeVisualData->WallMaterial);
	Walls->SetNumCustomDataFloats(1);

	// Copy Simple Data for readability
	const auto Maze = MazeGenerator->GenerateMaze();
	const int32 Dimension = MazeGenerator->GetDimension();

	UE_LOG(LogTemp, Warning, TEXT("Maze Neighbour Count: %d"), Maze->Graph.Num());

	// Declare A Set to save all the Walls already placed, so we have no duplicates
	TSet<FMazeNeighbours> PlacedWalls;

	// Declaring an Offset Array<Vector2Int>
	const TArray NeighbourOffsets = {
		FIntVector2(-1, 0),
		FIntVector2(1, 0),
		FIntVector2(0, -1),
		FIntVector2(0, 1)
	};

	const auto Center = FIntVector2(Dimension / 2, Dimension / 2);
	const TSet CenterCells = {
		Center - FIntVector2(1, 0),
		Center - FIntVector2(1, 1),
		Center - FIntVector2(0, 1),
		Center,
	};
	TArray<int32> CenterIndices;

	// iterate over each possible pair and perform a kernel convolution
	for (int x = 0; x < Dimension; x++)
	{
		for (int y = 0; y < Dimension; y++)
		{
			const auto CurrentCell = FIntVector2(x, y);

			// Inner Kernel Convolution
			for (auto Offset : NeighbourOffsets)
			{
				const auto NeighbourPosition= CurrentCell + Offset;
				const auto Temp1 = FMazeNeighbours {.A = CurrentCell, .B = NeighbourPosition};

				// if Edge is already found as a wall, we do not want to place there another
				if (PlacedWalls.Contains(Temp1))
					continue;
				
				// Check if Cell is BorderCell
				if (NeighbourPosition.GetMin() < 0 || NeighbourPosition.GetMax() > Dimension - 1)
				{
					auto _ = PlaceWall(CurrentCell, NeighbourPosition);
					PlacedWalls.Add(Temp1);
				}
				else if (!Maze->Graph.Contains(Temp1))
				{
					int32 ID = PlaceWall(CurrentCell, NeighbourPosition);
					if (CenterCells.Contains(CurrentCell) || CenterCells.Contains(NeighbourPosition))
						CenterIndices.Add(ID);
					PlacedWalls.Add(Temp1);
				}
			}
		}
	}

	// Adjust Center
	for (auto ID : CenterIndices)
	{
		Walls->SetCustomDataValue(ID, 0, 1.0f, true);
	}
}

int32 AMazeBuilder::PlaceWall(const FIntVector2 A, const FIntVector2 B) const
{
	const auto Location = FVector(
		(A.X * MazeVisualData->CellSize + B.X * MazeVisualData->CellSize) / 2.0,
		(A.Y * MazeVisualData->CellSize + B.Y * MazeVisualData->CellSize) / 2.0,
		0);

	
	const auto Scale = (A.X + B.X) % 2 == 0 ?
		FVector(MazeVisualData->CellSize / 100.0 - MazeVisualData->WallWidth, MazeVisualData->WallWidth, MazeVisualData->WallHeight):
		FVector(MazeVisualData->WallWidth, MazeVisualData->CellSize / 100.0 - MazeVisualData->WallWidth, MazeVisualData->WallHeight);
	

	// const auto Scale =FVector(MazeVisualData->CellSize / 100.0 - MazeVisualData->WallWidth, MazeVisualData->WallWidth, MazeVisualData->WallHeight);

	/*
	const auto Rotator = (A.X + B.X) % 2 == 0 ?
		FRotator(0,0,0):
		FRotator(0,90,0);
	*/
	
	FTransform T;
	// T.SetRotation(FQuat(Rotator));
	T.SetLocation(Location);
	T.SetScale3D(Scale);
	
	int32 InstanceID = Walls->AddInstance(T);
	Walls->SetCustomDataValue(InstanceID, 0, 0.0f, true);
	return InstanceID;
}
	
