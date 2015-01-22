close all;clear all;clc

load('Lag2D')

h = 0.01;
Nt = 1000;

a1= 1;
b1= 1;
a2= 1;
b2= 1;
k = 200;

rad = [0.5 0.5 0.5];

m = [5 1 1];
I = [1 1 1];

g = 10;

F = zeros(N,dims);
% F(1,1) = 0.1;

% collision
yfloor = 0;
n = [0  1;0  1;0  1];
r = [0 -1;0 -1;0 -1];

Vrel = zeros(N,1);

collide = zeros(N,1);

%% initial conditions
q   = zeros(K,Nt);
qp  = zeros(K,Nt);
qpp = zeros(K,Nt);

q(:,1)   = [0 2 0 0 0 0 0]';

qp(:,1)  = [0 0 0 0 0 0 0]';

figure()

%% step
for nt=2:Nt
    F = zeros(N,dims);
    %% solve
    for a=1:3
        if (xf{a,2}(q(:,nt-1),qp(:,nt-1),qpp(:,nt-1),a1,b1,a2,b2,k,F,n,r,m,I,g,h,Vrel)-rad(a))<yfloor
            collide(a)=true;
        else
            collide(a)=false;
        end 
    end
    
    
    for a=1:N
        if collide(a)
            for b=2
                F(a,b) = Rf{a,b}(q(:,nt-1),qp(:,nt-1),qpp(:,nt-1),a1,b1,a2,b2,k,F,n,r,m,I,g,h,Vrel);
            end
%             fprintf('collision\n')
        end
    end
    
    while true
        qpp0 = qpp(:,nt);
        
        
        
        for a=1:K
            qpp(a,nt) = f{a}(q(:,nt-1),qp(:,nt-1),qpp(:,nt),a1,b1,a2,b2,k,F,n,r,m,I,g,h,Vrel);
        end
        
        if max(abs((qpp0-qpp(:,nt))./qpp(:,nt))) < 0.0001
            break
        end
        
    end
    
    %% advance
    
    qp(:,nt) = qp(:,nt-1) + qpp(:,nt) * h;
    q(:,nt)  = q(:,nt-1)  + qp(:,nt)  * h;
    
    %% plot
    hold off
    title(sprintf('%i',nt))
    for a=1:3
        circle(...
            xf{a,1}(q(:,nt),qp(:,nt),qpp(:,nt),a1,b1,a2,b2,k,F,n,r,m,I,g,h,Vrel),...
            xf{a,2}(q(:,nt),qp(:,nt),qpp(:,nt),a1,b1,a2,b2,k,F,n,r,m,I,g,h,Vrel),...
            rad(a));hold on
    end

    axis equal
    axis([-5 5 -5 5])
    pause(0.001)
end