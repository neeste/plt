; twinkle.plt
;
symbol=18 : symsiz=5 : filcol=14
5 5
plot
;
; note frequencies (Hz)
a3= 440
as3=a3*2^(1/12)
b3= a3*2^(2/12)
c3= a3*2^(3/12)
cs3=a3*2^(4/12)
d3= a3*2^(5/12)
ds3=a3*2^(6/12)
e3= a3*2^(7/12)
f3= a3*2^(8/12)
fs3=a3*2^(9/12)
g3= a3*2^(10/12)
gs3=a3*2^(11/12)
a4=a3*2
;
; note duration (s)
t=0.2
;
; song notes
(c3) t
(c3) t
(g3) t
(g3) t
(a4) t
(a4) t
(g3) t*1.5
  0  t*0.5
(f3) t
(f3) t
(e3) t
(e3) t
(d3) t
(d3) t
(c3) t*1.5
  0  t*0.5
(g3) t
(g3) t
(f3) t
(f3) t
(e3) t
(e3) t
(d3) t*1.5
  0  t*0.5
(g3) t
(g3) t
(f3) t
(f3) t
(e3) t
(e3) t
(d3) t*1.5
  0  t*0.5
(c3) t
(c3) t
(g3) t
(g3) t
(a4) t
(a4) t
(g3) t*1.5
  0  t*0.5
(f3) t
(f3) t
(e3) t
(e3) t
(d3) t
(d3) t
(c3) t*1.5
play
