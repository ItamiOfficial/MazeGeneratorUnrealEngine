// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MazeData.generated.h"



UCLASS(BlueprintType)
class MAZEGENERATOR_API UMazeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Maze|Data")
	UStaticMesh* WallMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Maze|Data")
	UMaterial* WallMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Maze|Data")
	double WallWidth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Maze|Data")
	double WallHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Maze|Data")
	double CellSize;
};
