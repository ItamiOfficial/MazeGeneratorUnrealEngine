# MazeGeneratorUnrealEngine
Small Maze Generator with lot's of space for improval. Created for an University Course at Hochschule Fulda.

In Order to run it you only have to Make Sure that you have a Valid MazeData Data Asset Setup in a AMazeBuilder Actor.
If its set, you can create a MazeGen Object (maybe with NewObject<UMazeGen>(...)) and run Build Maze from the AMazeBuilder with the MazeGen as reference.

MazeGen Properties:
  Dimension: The Dimension of the Square, for example, you'll get a 8x8 Maze with a Dimension of 8
  Seed: The Random Seed, ensures that the Maze will always be the same if the Seed is the Same
  Dissolvement Ratio: (Range(0.0 - 1.0)). 0.0 would mean all walls stay, 1.0 would mean no wall would exist, except center. After Creating the Maze, we randomly dissolve Walls so we have more options exploring the Maze.
                      This is Basically the only thing i really want to change in the Future, exploring more options,
                      in order to create more intersting Mazes.

MazeData Properties:
  Wall Mesh: Just choos any Square with 100 x 100 x 100 Dimenions
  Wall Material: If you really want something funny, use some sort of Projection. Maybe Mesh gets removed Later and is getting build manually, so a procedural Material will be required anyway
  Wall Width: Width in Meters
  Wall Height: Height in Meters
  CellSize: Size of Cell in Centimeters

How to Use:
  1. Create an AMazeBuilder in the Scene (I'd create a BP Child)
  2. Create AMazeData with some 100x100x100 Cube, some White Material, CellSize: 100, WallHeight: 0.7, WallWidth: 0.12;
  3. Set MazeData Variable in AMazeBuilder Editor
  4. Create MazeGen with NewObject (Setup Parameters how you like, Dimension should be an even number)
  5. Call BuildMaze from NewObject
