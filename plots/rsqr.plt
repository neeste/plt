pltype=symb
1 1.1
2 2
3 3.3
4 4
5 5.5
6 6
7 7.7
8 8
stat
plot
%msg 0.5 5.4 "y = %.2f + %.2f x" $lr_a $lr_b 
%msg 0.5 5.0 "R{2} = %.3f" $corr_xy^2
pltype=line
($x_min) ($lr_a+$lr_b*$x)
($x_max) ($lr_a+$lr_b*$x)
plot
