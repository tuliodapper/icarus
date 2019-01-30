% index = maze_index_from_XY(maze,X,Y)
% By Jeremy Kubica, Copyright 2003
%
% Gets the index from the X and Y coordinates.
% Does NO bounds checking.
function index = maze_index_from_XY(size,X,Y)

index = ((X-1) * size) + Y;