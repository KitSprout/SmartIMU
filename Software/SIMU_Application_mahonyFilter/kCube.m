classdef kCube < handle

properties (SetAccess = public)
    origin;
    scale;
    color;
    rotate;
    qText;
end

properties (SetAccess = private)
    fig;
    coordinate;
    vertices;
    face;
    surface;
    window;
end

methods

    function cube = kCube( origin, scale, window )
        cube.origin = [ origin(1), origin(2), origin(3) ];
        cube.scale  = [ scale(1), scale(2), scale(3) ];
        cube.color  = [ 'r', 'g', 'b', 'y', 'm', 'c'];
        cube.rotate = [ 1, 0, 0; 0, 1, 0; 0, 0, 1 ];

        cube.coordinate = [  1,  1, -1; -1,  1, -1; -1,  1,  1;  1,  1,  1;
                            -1, -1,  1;  1, -1,  1;  1, -1, -1; -1, -1, -1 ];
        cube.coordinate = 0.5 * cube.coordinate;
        cube.vertices = [ cube.coordinate(:, 1) * cube.scale(1), cube.coordinate(:, 2) * cube.scale(2), cube.coordinate(:, 3) * cube.scale(3) ];
        cube.face = [ 1, 2, 3, 4; 4, 3, 5, 6; 6, 7, 8, 5;
                      1, 2, 8, 7; 6, 7, 1, 4; 2, 3, 5, 8 ];
        cube.window = [-window(1), window(1), -window(2), window(2), -window(3), window(3)];
    end

	function initCube( cube, fig, v )
        cube.fig = fig;
        grid(cube.fig, 'on');
        hold(cube.fig, 'on');
        axis(cube.fig, 'equal');
        axis(cube.fig, cube.window);
        xlabel(cube.fig, 'x-axis');
        ylabel(cube.fig, 'y-axis');
        zlabel(cube.fig, 'z-axis');
        cube.surface(1) = patch(cube.fig, 'Faces', cube.face(5, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(1));     % + x
        cube.surface(2) = patch(cube.fig, 'Faces', cube.face(1, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(2));     % + y
        cube.surface(3) = patch(cube.fig, 'Faces', cube.face(2, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(3));     % + z
        cube.surface(4) = patch(cube.fig, 'Faces', cube.face(6, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(4));     % - x
        cube.surface(5) = patch(cube.fig, 'Faces', cube.face(3, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(5));     % - y
        cube.surface(6) = patch(cube.fig, 'Faces', cube.face(4, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(6));     % - z
        
        cube.qText = text(cube.fig, cube.window(4) + 1, cube.window(1) - 1, 0, '', 'HorizontalAlignment', 'left');
        view(v);
    end

    function plotCube( cube, p, q )
        cube.origin = p;
        cube.vertices = [ cube.coordinate(:, 1) * cube.scale(1), cube.coordinate(:, 2) * cube.scale(2), cube.coordinate(:, 3) * cube.scale(3) ];
        cube.rotate(1) = 2.0 * (q(1) * q(1) + q(2) * q(2) - 0.5);
        cube.rotate(2) = 2.0 * (q(2) * q(3) + q(1) * q(4));
        cube.rotate(3) = 2.0 * (q(2) * q(4) - q(1) * q(3));
        cube.rotate(4) = 2.0 * (q(2) * q(3) - q(1) * q(4));
        cube.rotate(5) = 2.0 * (q(1) * q(1) + q(3) * q(3) - 0.5);
        cube.rotate(6) = 2.0 * (q(3) * q(4) + q(1) * q(2));
        cube.rotate(7) = 2.0 * (q(2) * q(4) + q(1) * q(3));
        cube.rotate(8) = 2.0 * (q(3) * q(4) - q(1) * q(2));
        cube.rotate(9) = 2.0 * (q(1) * q(1) + q(4) * q(4) - 0.5);
        cube.vertices = cube.rotate * cube.vertices';
        cube.vertices = cube.vertices';
        cube.vertices = [ cube.vertices(:, 1) + cube.origin(1), cube.vertices(:, 2) + cube.origin(2), cube.vertices(:, 3) + cube.origin(3) ];

        delete(cube.surface);
        cube.surface(1) = patch(cube.fig, 'Faces', cube.face(5, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(1));     % + x
        cube.surface(2) = patch(cube.fig, 'Faces', cube.face(1, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(2));     % + y
        cube.surface(3) = patch(cube.fig, 'Faces', cube.face(2, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(3));     % + z
        cube.surface(4) = patch(cube.fig, 'Faces', cube.face(6, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(4));     % - x
        cube.surface(5) = patch(cube.fig, 'Faces', cube.face(3, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(5));     % - y
        cube.surface(6) = patch(cube.fig, 'Faces', cube.face(4, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(6));     % - z
        drawnow
    end

end

end
