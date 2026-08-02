; colorbar.plt
;
header=0 : axlwt=0 : annlwt=0 : bgco=0 : fgco=0 : n=6 : a=1 : b=1
xmin=0 : ymin=0 : xmax=8 : ymax=8 : xllc=0 : yllc=0 : xlen=8 : ylen=8
plot

%define CMY "(1-($1-1)/(n-1)) (1-($2-1)/(n-1)) (1-($3-1)/(n-1))"
%repeat n
    %repeat n
        %repeat n
            %CMY $$0 $$$$0 $0
        %%%%
    %%%
%%
palette

%define box ; <xllc> <yllc> <color_index>
xofst=a+($1)*xgain : yofst=b-($2)*ygain : shdcol=$3 : pltype=r
0 0
1 1
plot
%%

xgain=(8.5/(2*n)) : ygain=(6*2/(n*n)) : shade=21
%repeat (n/2)
    %repeat 2
	a = 1.0 + xgain*(($$0-1)*n)
        b = 0.5 + ygain*((n*n/2-1) - ($0-1)*n)
        %repeat n
            %repeat n
                x=($$$$$$$$0-1)
                y=($$$$0-1)
                c=((($0-1)*2+($$0-1))*n+($$$$0-1))*n+($$$$$$$$0-1)
                %box x y c
            %%%%%
        %%%%
    %%%
%%

