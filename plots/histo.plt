pltype=hist
openax=y
yhor=y
ylen=5

%define HISTSETUP     ; <number of bins> <percentage for each bin> <1st datum>
b=(100/$2)
r=(b-1)
xint=$1+((b-1)*($1-1))
xmax=$1+((b-1)*($1-1))
xtick=0
xpercent=80
$3
%%

%define HISTDAT     ; <data value>
%repeat r
0
%%%
$1
%%

xannot=.5,A 1.5,B 2.5,C 3.5,D 4.5,E 5.5,F 6.5,G 7.5,H 8.5,I 9.5,J
;xannot=.5,A 2.5,B 4.5,C 6.5,D 8.5,E 10.5,F 12.5,G 14.5,H 16.5,I 18.5,J
;xannot=.5,A 3.5,B 6.5,C 9.5,D 12.5,E 15.5,F 18.5,G 21.5,H 24.5,I 27.5,J
;xannot=.5,A 4.5,B 8.5,C 12.5,D 16.5,E 20.5,F 24.5,G 28.5,H 32.5,I 36.5,J
;xannot=.5,A 10.5,B 20.5,C 30.5,D 40.5,E 50.5,F 60.5,G 70.5,H 80.5,I 90.5,J
;xannot=.5,A 20.5,B 40.5,C 60.5,D 80.5,E 100.5,F 120.5,G 140.5,H 160.5,I 180.5,J
%HISTSETUP 10 100 3
%HISTDAT 7
%HISTDAT 6
%HISTDAT 4
%HISTDAT 3
%HISTDAT 2
%HISTDAT 1
%HISTDAT 3
%HISTDAT 2
%HISTDAT 5
;varlist
;prmlist
;datlist
plot
