rem generates user function file 'nslmonfn.c' to compile & link with nslms
rem NeSeL v1.0 April 1989  Neural Network Specification Language
cpp -P2 -Wno-bad-chars -Ic:\msdev\include -I..\include -I..\nsllib -DNSL1 -D_WIN32 -DWIN32 -D_CONSOLE nslmonfn.n >nslmonfn.nsp
nsl1 -xmvasp nslmonfn.nsp
rmlines nslmonfn.c b
del nslmonfn.c
ren nslmonfn.0 nslmonfn.c
del nslmonfn.ns?
