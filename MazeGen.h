// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeBuilder.h"
#include "UObject/Object.h"
#include "MazeGen.generated.h"

UENUM(BlueprintType)
enum class Direction : uint8
{
	Vertical,
	Horizontal,
};

USTRUCT(BlueprintType)
struct FMazeNeighbours
{
	GENERATED_BODY()

	UPROPERTY()
	FIntVector2 A;

	UPROPERTY()
	FIntVector2 B;

	bool operator==(const FMazeNeighbours& Other) const
	{
		return Equals(Other);
	}
	
	bool Equals(const FMazeNeighbours& Other) const
	{
		return (A == Other.A && B == Other.B) || (A == Other.B && B == Other.A);
	}

	bool EqualsExactly(const FMazeNeighbours& Other) const
	{
		return A == Other.A && B == Other.B;
	}
};

FORCEINLINE uint32 GetTypeHash(const FMazeNeighbours& Self)
{
	auto HashVec = [](const FIntVector2& V) -> uint32 {
		return (uint32(V.X) * 73856093u) ^ (uint32(V.Y) * 19349663u);
	};
	uint32 Ha = HashVec(Self.A);
	uint32 Hb = HashVec(Self.B);
	
	return Ha + Hb;
}

UCLASS(BlueprintType)
class UMaze : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Maze")
	TSet<FMazeNeighbours> Graph;
};


UCLASS(BlueprintType, Blueprintable)
class MAZEGENERATOR_API UMazeGen : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn), Category="Maze|Generation")
	uint8 Dimension = 16;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn), Category="Maze|Generation")
	double DissolveRatio = 0.05;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn), Category="Maze|Generation")
	int32 RandSeed = 42;
	
public:
	UFUNCTION(BlueprintNativeEvent, Category="Maze|Generation")
	UMaze* GenerateMaze();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Maze|Generation")
	int32 GetDimension() const { return Dimension; }

protected:
	UFUNCTION(Category="Maze|Generation|Utility")
	TArray<FIntVector2> GetValidNeighbours(const FIntVector2& Origin) const;

private:
	void InitUnvisited(TArray<FIntVector2>& Horst) const;
	void FixCenter(UMaze* Maze, const TArray<FIntVector2>& CenterCells);
	void KillAndHunt(UMaze* Maze, const TArray<FIntVector2>& IgnoredTiles) const;
};
