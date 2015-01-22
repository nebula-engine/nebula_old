function [f,L,Lag,Kin,V,x,xf,Q,dims,Vrel,xpp,Rf]=lagrange2D(N,K,xstr,tstr,Vstr,Cstr,grav,constants)
%LAGRANGE2D Lagrangian mechanics
%   lagrange2D(N,K,xstr,tstr,Vstr,Cstr,grav,constants)
%
%   xstr is an N by dims cell where each element is a string
%   representing a function for the cartesian position of the appropriate
%   body and coordinate.
%
%   tstr is an N by 1 cell where each element is a string
%   representing a function for the angular position of the appropriate
%   body.

dims = 2;

cstr = '';
for a=1:length(constants)
    syms(constants{a})
    cstr = [cstr ',' constants{a}]; %#ok<AGROW>
end

m       = sym('m',[N,1]);
I       = sym('I',[N,1]);
Vrelsym = sym('Vrel',[N,1]);

for a=1:K
    syms(['q'   num2str(a) '(T)'])
    syms(['qp'  num2str(a) '(T)'])
    syms(['qpp' num2str(a)])
end

x    = cell(N,dims);
xp   = cell(N,dims);
xpp  = cell(N,dims);
t    = cell(N,1);
tp   = cell(N,1);
tpp  = cell(N,1);
Kin  = cell(N,1);
R    = cell(N,dims);
Rf   = cell(N,dims);

syms T h g

L   = cell(K,1);
f   = cell(K,1);
qpp = cell(K,1);
xf  = cell(N,dims);

Vrel = cell(N,1);

n = sym('n',[N,dims]);
r = sym('r',[N,dims]);

%% convert strings
fprintf('converting strings\n')

for a=1:N
    for b=1:dims
        xstr{a,b} = regexprep(xstr{a,b},'(q\d)','$1(T)');
    end
    tstr{a} = regexprep(tstr{a},'(q\d)','$1(T)');
    
end

Vstr = regexprep(Vstr,'(q\d)','$1(T)');

for a=1:N
    for b=1:dims
        x{a,b}(T) = sym(xstr{a,b});
    end
    t{a}(T) = sym(tstr{a});
end

V(T) = sym(Vstr);

if grav
    for a=1:N
        V(T) = V(T) + m(a)*g*x{a,2};
    end
end

%% force
fprintf('calculating forces\n')

F=sym('F',[N dims]);

Q = cell(K,1);

for a=1:K
    Q{a} = 0;
    for b=1:N
        for c = 1:dims
            %Q{a} = Q{a} + F dot diff(r,q(a))
            Q{a} = Q{a} +...
                F(b,c) * sdiff(x{b,c},['q' num2str(a) '(T)']);
        end
    end
end

%% derivatives
fprintf('taking derivatives of position and velocity\n')

for a=1:N
    for b=1:dims
        xp{a,b}(T)  = subs1( diff(x{a,b}) , K );
        xpp{a,b}(T) = subs1( diff(xp{a,b}), K );
    end
    tp{a}(T)  = subs1( diff(t{a}) , K );
    tpp{a}(T) = subs1( diff(tp{a}), K );
end

% kinetic energy
fprintf('kinetic energy\n')

for a=1:N
    Kin{a}(T) = 1/2*m(a)*(xp{a,1}^2 + xp{a,2}^2) +...
        1/2*I(a)*tp{a}^2;
end

% Lagrangian
fprintf('lagrangian\n')

Lag = 0;
for a=1:N
    Lag = Lag + Kin{a};
end
Lag = Lag - V;

% equations of motion

for a=1:K
    L{a} =...
        diff(sdiff(Lag,['qp' num2str(a) '(T)'])) -...
        sdiff(Lag,['q' num2str(a) '(T)']) -...
        Q{a};
    L{a} = subs1( L{a},K );
    
    qpp{a} = solve(L{a},['qpp' num2str(a)]);
    
    f{a} = sym2func(qpp{a},cstr);
end

for a=1:N
    for b=1:dims
        xf{a,b}=sym2func(x{a,b},cstr);
    end
end

%% collision handling
fprintf('collision\n')

for a=1:N
    Vrel{a} = 0;
    for b=1:dims
        Vrel{a} = Vrel{a} +...
            ( xp{a,b} + h*xpp{a,b} ) * n(a,b);
    end
    Vrel{a} = Vrel{a} +...
        (n(a,1)*r(a,2) - n(a,2)*r(a,1)) *...
        ( tp{a} + h*tpp{a} );
    Vrel{a} = Vrel{a} - Vrelsym(a);
    
    for b=1:K
        Vrel{a} = subs(Vrel{a},['qpp' num2str(b)],['temp' num2str(b)]);
    end
    for b=1:K
        Vrel{a} = subs(Vrel{a},['temp' num2str(b)],qpp{b});
    end
    
    for b=1:dims
        R{a,b}  = solve(Vrel{a},['F' num2str(a) '_' num2str(b)]);
        Rf{a,b} = sym2func(R{a,b},cstr);
    end
end



end





















function x=subs1(x,K)

str = char(x);

for a=1:K
    str = regexprep(str,...
        ['D\(q'  num2str(a) '\)\(T\)'],...
        ['qp' num2str(a) '(T)']);
    str = regexprep(str,...
        ['D\(qp' num2str(a) '\)\(T\)'],...
        ['qpp' num2str(a)]);
end

x=sym(str);

end

function y=sdiff(y,v)
syms temp

y = subs(y,v,temp);
y = diff(y,'temp');
y = subs(y,'temp',v);

end

function f=sym2func(x,cstr)

str = char(x);

str = regexprep(str,'q(\d)\(T\)' ,'q($1)');
str = regexprep(str,'qp(\d)\(T\)','qp($1)');
str = regexprep(str,'qpp(\d)'    ,'qpp($1)');

str = regexprep(str,'m(\d)','m($1)');
str = regexprep(str,'I(\d)','I($1)');

str = regexprep(str,'Vrel(\d)','Vrel($1)');

str = regexprep(str,'F(\d)_(\d)','F($1,$2)');

str = regexprep(str,'n(\d)_(\d)','n($1,$2)');
str = regexprep(str,'r(\d)_(\d)','r($1,$2)');


str = ['@(q,qp,qpp' cstr ',F,n,r,m,I,g,h,Vrel) ' str];

f=str2func(str);

end