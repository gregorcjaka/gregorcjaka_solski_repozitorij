data = readmatrix("vozlisca_temperature_dn2.txt", "NumHeaderLines", 4);

x = data(:, 1);
y = data(:, 2);
temp = data(:, 3);
point_x = 0.403;
point_y = 0.503;

%naloga 1
tic;
scatinter = scatteredInterpolant(x, y, temp, 'linear', 'none'); 
T1 = scatinter(point_x, point_y);

time = toc;

%naloga 2
x_unique = unique(x);
y_unique = unique(y);
[X, Y] = ndgrid(x_unique, y_unique);
Tr = reshape(temp, length(x_unique), length(y_unique));

tic;
gridinter = griddedInterpolant(X, Y, Tr, "linear");
T2 = gridinter(point_x, point_y);

time2 = toc;

% naloga 3
tic;
% iscemo po principu: npr. najprej iscemo max vrednost x od x-ov, ki so se 
% manjsi...torej zato x_min. Tako poiscemo vse 4 koordinate najblizjih
% kotov
x_min = max(x_unique(x_unique <= point_x)); 
x_max = min(x_unique(x_unique >= point_x));
y_min = max(y_unique(y_unique <= point_y));
y_max = min(y_unique(y_unique >= point_y));

% Preberemo temperature v teh koordinatah
T11 = temp(x == x_min & y == y_min);
T21 = temp(x == x_max & y == y_min);
T12 = temp(x == x_min & y == y_max);
T22 = temp(x == x_max & y == y_max);

% Samo se uporabimo interpolacijske formule za bilinearno interpolacijo
K1 = (x_max - point_x) / (x_max - x_min) * T11 + (point_x - x_min) / (x_max - x_min) * T21;
K2 = (x_max - point_x) / (x_max - x_min) * T12 + (point_x - x_min) / (x_max - x_min) * T22;
T_manual = (y_max - point_y) / (y_max - y_min) * K1 + (point_y - y_min) / (y_max - y_min) * K2;
time3 = toc;

% Primerjamo case posameznih metod
fprintf('Cas, ki ga potrebuje scatteredInterpolant: %.6f sekund\n', time);
fprintf('Cas, ki ga potrebuje griddedInterpolant: %.6f sekund\n', time2);
fprintf('Cas, ki ga potrebuje metoda bilinearne interpolacije: %.6f sekund \n', time3);

% naloga 4
[max_temp, max_idx] = max(temp);
max_x = x(max_idx);
max_y = y(max_idx);

fprintf('Maksimalna temperatura, ki se pojavi je %.2f stopinj in to v vozliscu (%.3f, %.3f).\n', max_temp, max_x, max_y);