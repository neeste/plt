; c256.plt
;
header=0 : axlwt=0 : annlwt=0
xmin=0 : ymin=0 : xmax=8 : ymax=8 : xllc=0 : yllc=0 : xlen=8 : ylen=8
plot

n=4 : b=(1/((n-1)*(n+1))) : a=(b*n)
%define RGBW "(($1-1)*a+($4-1)*b) (($2-1)*a+($4-1)*b) (($3-1)*a+($4-1)*b)"
data
%repeat n
    %repeat n
        %repeat n
            %repeat n
                %RGBW $0 $$0 $$$$0 $$$$$$$$0
            %%%%%
        %%%%
    %%%
%%
palette

%define box ; <xllc> <yllc> <color_index>
xofst=($1)*xgain : yofst=($2)*ygain : pltype=6 : shade=21 : shdcol=$3
0 0
0 1
1 1
1 0
plot
%%

xgain=(10.5/(n*n)) : ygain=(8/(n*n)) : c=0
%repeat n
    %repeat n
        %repeat n
            %repeat n
                x=($$0-1)*n + ($$$$$$$$0-1)
                y=($0-1)*n + ($$$$0-1)
                c=((($0-1)*n+($$0-1))*n+($$$$0-1))*n+($$$$$$$$0-1)
                %box x ((n+1)*(n-1)-y) c
            %%%%%
        %%%%
    %%%
%%

