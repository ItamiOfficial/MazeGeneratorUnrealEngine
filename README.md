# MazeGeneratorUnrealEngine
Small Maze Generator with lot's of space for improval. Created for an University Course at Hochschule Fulda.

In Order to run it you only have to Make Sure that you have a Valid MazeData Data Asset Setup in a AMazeBuilder Actor.
If its set, you can create a MazeGen Object (maybe with NewObject<UMazeGen>(...)) and run Build Maze from the AMazeBuilder with the MazeGen as reference.

MazeGen Properties:
  Dimension: The Dimension of the Square, for example, you'll get a 8x8 Maze with a Dimension of 8
  Seed: The Random Seed, ensures that the Maze will always be the same if the Seed is the Same
  Dissolvement Ratio: After Creating the Maze, we randomly dissolve Walls so we have more options exploring the Maze.
                      This is Basically the only thing i really want to change in the Future, exploring more options,
                      in order to create more intersting Mazes.

MazeData Properties:
  Wall Mesh: Just choos any Square with 100 x 100 x 100 Dimenions
  Wall Material: If you really want something funny, use some sort of Projection. Maybe Mesh gets removed Later and is getting build manually, so a procedural Material will be required anyway
  Wall Width: Width in Meters
  Wall Height: Height in Meters
  CellSize: Size of Cell in Centimeters
