// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "MazeBuilder.generated.h"

class UMazeData;
class UMazeGen;

UCLASS(Blueprintable, BlueprintType)
class MICROMOUSE_API AMazeBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeBuilder();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Maze|Visualisation")
	UInstancedStaticMeshComponent* Walls;
	
	UPROPERTY(EditAnywhere, Category="Maze|Visualisation")
	UMazeData* MazeVisualData;

	UFUNCTION(BlueprintCallable, Category="Maze|Generation")
	void BuildMaze(UMazeGen* MazeGenerator);

private:
	int32 PlaceWall(FIntVector2 A, FIntVector2 B) const;
};
