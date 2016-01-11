rem generates user function file 'nslusrfn.c' to compile & link with nsl2
rem NeSeL v1.0 April 1989  Neural Network Specification Language
cpp -P2 -Wno-bad-chars -ansi -IC:\msdev\include -I..\include -I..\nsllib -DNSL1 -DNSL2 -D_WIN32 -DWIN32 -D_CONSOLE nslusrfn.n >nslusrfn.nsp
nsl1 -f nslusrfn.nsp
rmlines nslusrfn.c b
del nslusrfn.c
ren nslusrfn.0 nslusrfn.c
del nslusrfn.ns?
