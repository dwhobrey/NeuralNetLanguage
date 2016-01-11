rem if you have the recursive version of yacc
set YACCPAR=C:\usr\bin
yacc -dv nslyacc2.y
rmlines ytab.c
del ytab.c
del ytab2.*
del youtput
ren ytab.0 ytab2.c
ren ytab.h ytab2.h