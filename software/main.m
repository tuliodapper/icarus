% Clear the command window
clc

%Size
size = 4;

NORTH = 1;
EAST = 2;
SOUTH = 3;
WEST = 4;

% Array contains all the number of cells needed to get to the center on each position.
maze_array = zeros(size*size);

% Array contains wall info north south east west of each cell
maze_walls = zeros(size*size,4);

% Variable initialisation
current_x = 1;
current_y = size;

% To stop execution of a MATLAB® command, press Ctrl+C or Ctrl+Break.
% On Apple Macintosh platforms, you also can use Command+. (the Command key
% and the period key).

%s4=serial('COM4', 'BaudRate', 9600,'DataBits', 8, 'StopBits', 1, 'Parity', 'none');
%s5=serial('COM5', 'BaudRate', 9600,'DataBits',8, 'StopBits', 1, 'Parity', 'none');

filename = 'C:\Icarus\putty.txt';
x = 0;

%fopen(s4);
%fopen(s5);

while(1)

    %% Plot maze
    draw_maze(size,1,current_x,current_y,maze_array,maze_walls);
    

    while (exist(filename, 'file') == 0)
        pause(0.5);
    end
    
    value = dlmread(filename,',',1+x,0);
    
    delete(filename);
 
    x = x + length(value(:,1));
    
    for i = 1:length(value(:,1))
        x = value(i,1);
        y = value(i,2);
        walls = value(i,3);
        distance = value(i,4);
        y = size - y;
        x = x + 1;
        if(walls~=99)
            index = maze_index_from_XY(size, x, y);
            maze_walls(index,WEST) = bitget(walls, 1);
            maze_walls(index,NORTH) = bitget(walls, 2);
            maze_walls(index,EAST) = bitget(walls, 3);
            maze_walls(index,SOUTH) = bitget(walls, 4);
            maze_array(index) = distance;        
        else
            current_y = size - y;
            current_x = x + 1;
        end
    end
    
    pause(0.1);
end