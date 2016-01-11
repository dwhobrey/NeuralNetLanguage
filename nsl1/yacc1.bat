set YACCPAR=C:\usr\bin
yacc -dv nslyacc.y
rmlines ytab.c
del ytab.c
del youtput
ren ytab.0 ytab.c
