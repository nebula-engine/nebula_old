close all;clear all;clc

N = 3;
K = 7;

% 1 x1
% 2 y2
% 3 t1
% 4 t2
% 5 t3
% 6 c1
% 7 c2

% constats

constants = {'a1' 'b1' 'a2' 'b2' 'k'};

% body 1

xstr{1,1} = 'q1';

xstr{1,2} = 'q2';

tstr{1}   = 'q3';

% body 2

xstr{2,1} = 'q1 - a1*cos(q3) + b1*sin(q3) + q6*sin(q3)';

xstr{2,2} = 'q2 - a1*sin(q3) - b1*cos(q3) - q6*cos(q3)';

tstr{2}   = 'q3 + q4';

% body 3

xstr{3,1} = 'q1 + a2*cos(q3) + b2*sin(q3) + q7*sin(q3)';

xstr{3,2} = 'q2 + a2*sin(q3) - b2*cos(q3) - q7*cos(q3)';

tstr{3}   = 'q3 + q5';

% potential

Vstr = '1/2*k*q6^2 + 1/2*k*q7^2';

grav = true;

% compute

[f,L,Lag,Kin,V,Vx,xf,Q,dims,Vrel,xpp,Rf]=lagrange2D(N,K,xstr,tstr,Vstr,grav,constants);

save('Lag2D','f','xf','Rf','N','K','dims')