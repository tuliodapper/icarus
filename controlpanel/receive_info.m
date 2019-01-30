function [x, y, walls] = receive_info(bt)
      A = fscanf(bt, '%2d%2d%2d')
      x = A(1)
      y = A(2)
      walls = A(3)
end