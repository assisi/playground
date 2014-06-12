clear

#input parameters
amplitude = 0.2;

num_samples = 50;

arena_width = 0.5;
arena_height = 0.5;

number_casus = 4;

decay_constant = 5;

#coordinates of the casus
x_coordinates = [0.225, 0.425, 0.225, 0.425];
y_coordinates = [0.225, 0.425, 0.425, 0.225];

# compute and plot
x = linspace(0, arena_width, num_samples);
y = linspace(0, arena_height, num_samples);

[X,Y] = meshgrid(x,y);

figure;
axis equal;
hold on;

dx = arena_width/(num_samples-1);
dy = arena_height/(num_samples-1);

for i = 1:num_samples
    for j = 1:num_samples
        x = X(i,j);
        y = Y(i,j);
        position_amplitude = 0;
        for casu = 1:number_casus
            casu_x = x_coordinates(casu);
            casu_y = y_coordinates(casu);
            squared_distance = (casu_x - x)^2 + (casu_y - y)^2;
            measured_amplitude = amplitude - squared_distance * decay_constant;
            if measured_amplitude >= 0
                position_amplitude += measured_amplitude;
            end
        end
        
        position_patch = patch([x-dx/2,x+dx/2,x+dx/2,x-dx/2],...
                   [y-dy/2,y-dy/2,y+dy/2,y+dy/2],...
                   [1 - position_amplitude, 1 - position_amplitude, 1 - position_amplitude]);
        set(position_patch, 'EdgeColor', 'none');
     end
end