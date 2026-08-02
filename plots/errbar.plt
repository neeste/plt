; dblerr.plt - demostrate macro that plots X and Y error bars
;   
;============================ macro definition ===================================
% define dblerr ; X, Y, YSD, XSD
p=pltype : l=lintype
; This part draws the line between + and - one SD along the X axis
pltype=2 : lintype=0
$1-$4  $2
$1+$4  $2
plot
; This part draws the vertical bars on the error bars for the X axis variable
D = SYMSIZ*[EBWID * (XLEN + YLEN) * (YMAX - YMIN) / (2 * YLEN * YPER)]
$1-$4  $2+D
$1-$4  $2-D
plot
$1+$4  $2+D
$1+$4  $2-D
plot
; This part draws the symbol at the correct location, and the Y axis error bar
pltype=1
$1 $2 $3
plot
lintype=(l) : pltype=(p)
%%
;==================================================================================

%dblerr 2 3 0.2 0.8
%dblerr 3 4 0.3 0.2
%dblerr 4 5 0.2 0.3
