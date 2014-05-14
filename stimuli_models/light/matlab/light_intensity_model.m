% Testing the light intensity model for the ASSISI simulator

% Light source parameters (distance part of the model)
I_max = 1;         % Max. luminous intensity of the source,
                    % in [cd] (candela)
%h = 0.02;           % Height of the light source, in [m]
%alpha = pi/3;       % Light source opening angle, in [rad]
%r = h * tan(alpha/2);   % Radius of area affected by the light
r = 0.05; % 5cm
%k = 5000;             % Sigmoid shaping coefficient
k = 100;

% Light source parameters (wavelength part of the model)
lambda_max = 435e-9;    % Blue light source wavelength, in [m] 
sigma = 10e-9;          % Shaping coefficient for wavelength dependence


% Input parameters
num_samples = 500;
d = linspace(0,2*r,num_samples);
lambda = linspace(250e-9,600e-9,100);
[D,LAMBDA] = meshgrid(d,lambda);

% Compute and plot the model

figure;
I = I_max * (1 - tanh(k*(D - r))) / 2 .* exp(-(LAMBDA-lambda_max).^2 / (2*sigma^2));
% mesh(D,LAMBDA,I);
% xlabel('distance [m]')
% ylabel('wavelength [m]')
% zlabel('Light intensity [cd]')
%surf(D,LAMBDA,I);
%figure;
%plot(D,I);

% Compute and plot the the simulated effect of a single light source
%figure;
axis equal;
hold on;
% The hue of the HSV colormap can be linearly mapped 
% to visible spectrum wavelengths (roughly)
% cmap = colormap;
% colormap(hsv(360));
num_samples = 101;
x_range = 2*r;
y_range = 2*r;
dx = 2*x_range/(num_samples-1); % x-resolution
dy = 2*y_range/(num_samples-1); % y-resolution
x = linspace(-x_range, x_range, num_samples);
y = linspace(-y_range, y_range, num_samples);
[X,Y] = meshgrid(x,y);
a = -2.5e6;   % Scaling factor for converting wavelength to hue
b = 1.75; % Offset factor for converting wavelngth to hue
            % a and b values obtained by assuming [400nm, 670nm] visible
            % spectrum and 
for i = 1:num_samples
    for j = 1:num_samples
        x = X(i,j);
        y = Y(i,j);
        d = sqrt( x^2 + y^2 );
        I_ij = I_max * (1 - tanh(k*(d - r)));
        h = -2.5e6 * lambda_max + 1.75;
        h_p = patch([x-dx/2,x+dx/2,x+dx/2,x-dx/2],...
                   [y-dy/2,y-dy/2,y+dy/2,y+dy/2],...
                   hsv2rgb([h,I_ij/2,1]));
        set(h_p,'EdgeColor','none');
    end
end
title(['k = ' mat2str(k)]);
xlabel('x / m');
ylabel('y / m');

