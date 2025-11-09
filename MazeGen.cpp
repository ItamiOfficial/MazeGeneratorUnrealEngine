// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGen.h"
#include "Algo/RandomShuffle.h"



UMaze* UMazeGen::GenerateMaze_Implementation()
{
    // Init Objects/Properties
    auto Maze = NewObject<UMaze>();
    FMath::RandInit( RandSeed );

    // Center
    const FIntVector2 Center = FIntVector2(Dimension / 2, Dimension / 2);
    const TArray CenterCells = {
        Center,
        Center - FIntVector2(0, 1),
        Center - FIntVector2(1, 1),
        Center - FIntVector2(1, 0),
    };

    // Step 1. Perform Kill and Hunt 
    KillAndHunt(Maze, CenterCells);

    // Step 2. Dissolve Random Walls -> Add Edges
    for (int j = 0; j < (Dimension * Dimension) * DissolveRatio; j++)
    {
        int xOffset = FMath::RandRange(0, Dimension * Dimension - 1);
        int yOffset = FMath::RandRange(0, Dimension * Dimension - 1);
        for (int i = 0; i < Dimension * Dimension; i++)
        {
            bool bCanAbort = false;
            FIntVector2 Cell = FIntVector2((i + xOffset) / Dimension, (i + yOffset) % Dimension);

            for (auto Neighbour : GetValidNeighbours(Cell))
            {
                if (!Maze->Graph.Contains( {Cell, Neighbour} ))
                {
                    Maze->Graph.Add( {Cell, Neighbour});
                    bCanAbort = true;
                    break;
                }
            }

            if (bCanAbort)
                break;
        }
    }
    
    // Step 3. Clear Center
    FixCenter(Maze, CenterCells);

    // Return Maze
    return Maze;
}

TArray<FIntVector2> UMazeGen::GetValidNeighbours(const FIntVector2& Origin) const
{
    // Declare Constants
    const TArray Neighbours =
    {
        FIntVector2(1, 0),
        FIntVector2(0, 1),
        FIntVector2(-1, 0),
        FIntVector2(0, -1),
    };

    TArray<FIntVector2> Ret;

    for (auto N : Neighbours)
    {
        const auto NP = Origin + N;
        if (NP.GetMax() < Dimension && NP.GetMin() > -1)
        {
            Ret.Add(NP);
        }
    }

    return Ret;
}

void UMazeGen::InitUnvisited(TArray<FIntVector2>& Horst) const
{
    for ( int i = 0; i < Dimension * Dimension; i++ )
    {
        Horst.Add( FIntVector2(i / Dimension, i % Dimension) );
    }

    
}

void UMazeGen::KillAndHunt(UMaze* Maze, const TArray<FIntVector2>& IgnoredTiles) const
{
    // Declare Some Variables
    TArray<FIntVector2> Unvisited;

    // Make Horst Complete
    InitUnvisited(Unvisited);
    for (auto V : IgnoredTiles)
    {
        Unvisited.Remove(V);
    }

    FIntVector2 Current = FIntVector2::ZeroValue;
    Unvisited.Remove(Current);
    
    while ( !Unvisited.IsEmpty() )
    {
        // Kill Phase
        auto Neighbour = FIntVector2(-1, -1);
        auto ShuffledNeighbours = GetValidNeighbours(Current);
        Algo::RandomShuffle(ShuffledNeighbours);
        for (auto NewNeighbour : ShuffledNeighbours)
        {
            if ( Unvisited.Contains(NewNeighbour) )
            {
                Maze->Graph.Add( FMazeNeighbours {Current, NewNeighbour } );
                Neighbour = NewNeighbour;
                Current = Neighbour;
                Unvisited.Remove(Current);
                break;
            } 
        }

        // Hunt Phase
        if ( Neighbour == FIntVector2(-1, -1) )
        {
            bool bFoundUnvisitedCellWithNeighbour = false;
            for (auto Cell : Unvisited)
            {
                for (auto NewNeighbour : GetValidNeighbours(Cell))
                {
                    if (!Unvisited.Contains(NewNeighbour))
                    {
                        Maze->Graph.Add( FMazeNeighbours {Cell, NewNeighbour } );
                        Current = Cell;
                        Unvisited.Remove(Cell);
                        bFoundUnvisitedCellWithNeighbour = true;
                        break;
                    } 
                }

                if (bFoundUnvisitedCellWithNeighbour)
                    break;
            }
        }
    }
}

void UMazeGen::FixCenter(UMaze* Maze, const TArray<FIntVector2>& CenterCells)
{
    // Remove Outer Walls
    Maze->Graph.Remove( FMazeNeighbours {CenterCells[0], CenterCells[0] + FIntVector2(0, 1)} );
    Maze->Graph.Remove( FMazeNeighbours {CenterCells[0], CenterCells[0] + FIntVector2(1, 0)} );
    Maze->Graph.Remove( FMazeNeighbours {CenterCells[1], CenterCells[1] + FIntVector2(0, -1)} );
    Maze->Graph.Remove( FMazeNeighbours {CenterCells[1], CenterCells[1] + FIntVector2(1, 0)} );
    Maze->Graph.Remove( FMazeNeighbours {CenterCells[2], CenterCells[2] + FIntVector2(0, -1)} );
    Maze->Graph.Remove( FMazeNeighbours {CenterCells[2], CenterCells[2] + FIntVector2(-1, 0)} );
    Maze->Graph.Remove( FMazeNeighbours {CenterCells[3], CenterCells[3] + FIntVector2(0, 1)} );
    Maze->Graph.Remove( FMazeNeighbours {CenterCells[3], CenterCells[3] + FIntVector2(-1, 0)} );

    // Add Back One Entry, does not matter where, the maze is Perfect!
    switch (const uint8 RandomOpenWall = FMath::RandRange(0, 7))
    {
    case 0: Maze->Graph.Add( FMazeNeighbours {CenterCells[0], CenterCells[0] + FIntVector2(0, 1)} ); break;
    case 1: Maze->Graph.Add( FMazeNeighbours {CenterCells[0], CenterCells[0] + FIntVector2(1, 0)} ); break;
    case 2: Maze->Graph.Add( FMazeNeighbours {CenterCells[1], CenterCells[1] + FIntVector2(0, -1)} ); break;
    case 3: Maze->Graph.Add( FMazeNeighbours {CenterCells[1], CenterCells[1] + FIntVector2(1, 0)} ); break;
    case 4: Maze->Graph.Add( FMazeNeighbours {CenterCells[2], CenterCells[2] + FIntVector2(0, -1)} ); break;
    case 5: Maze->Graph.Add( FMazeNeighbours {CenterCells[2], CenterCells[2] + FIntVector2(-1, 0)} ); break;
    case 6: Maze->Graph.Add( FMazeNeighbours {CenterCells[3], CenterCells[3] + FIntVector2(0, 1)} ); break;
    case 7: Maze->Graph.Add( FMazeNeighbours {CenterCells[3], CenterCells[3] + FIntVector2(-1, 0)} ); break;
    default: break;
    }
    
    // Add Inner Square
    for (int i = 0; i < CenterCells.Num(); i++)
    {
        Maze->Graph.FindOrAdd(FMazeNeighbours { CenterCells[i], CenterCells[(i + 1) % CenterCells.Num()]} );
    }
}
