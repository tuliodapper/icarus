% draw_maze(maze)
% By Jeremy Kubica, Copyright 2003
%
% Displays the maze.
%   label_cells - A Boolean variable indicating whether to label
%                 the cells.  0 = Don't Label, 1 = Label (Default = 0)
% Type 'help maze' for more information
function h1 = draw_maze(sizeX, sizeY, X, Y, wall_array)

h1 = figure(1);

set(h1,'DoubleBuffer','on')

clf;
hold on;

% determine the size of the maze and set the figure accordingly
C = sizeX;
R = sizeY;

axis([0 sizeX 2 sizeY+2]);

% draw the grid
text(X-0.5,(R-Y+2.5),'#');

WALL_COLOR = 'r';
WALL_WIDTH = 1.5;

for i = 1:C
   for j = 1:R
            
      % Draw the northern border
      HN = line([(i-1) (i)],[(R-j+3) (R-j+3)]);
      if(wall_array(i, j, 1) == 1)
         set(HN,'Color', WALL_COLOR);
         set(HN,'LineStyle','-');
         set(HN,'LineWidth', WALL_WIDTH);
      end
      
      % Draw the southern border
      HS = line([(i-1) (i)],[(R-j+2) (R-j+2)]);
      if(wall_array(i, j, 3) == 1)
         set(HS,'Color', WALL_COLOR);
         set(HS,'LineStyle','-');
         set(HS,'LineWidth', WALL_WIDTH);
      end
      
      % Draw the eastern border
      HE = line([(i) (i)],[(R-j+3) (R-j+2)]);
      if(wall_array(i, j, 2) == 1)
         set(HE,'Color', WALL_COLOR);
         set(HE,'LineStyle','-');
         set(HE,'LineWidth', WALL_WIDTH);
      end
      
      % Draw the western border
      HW = line([(i-1) (i-1)],[(R-j+3) (R-j+2)]);
      if(wall_array(i, j, 4) == 1)
         set(HW,'Color', WALL_COLOR);
         set(HW,'LineStyle','-');
         set(HW,'LineWidth', WALL_WIDTH);
      end
   end
   drawnow;
end
