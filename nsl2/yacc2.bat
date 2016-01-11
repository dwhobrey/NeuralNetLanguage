rem use this if you have the non recursive version of yacc
yacc -dv nslyacc2.y
rmlines ytab.c
del ytab.c
del ytab2.*
yactorec <ytab.0 >ytab2.c
del ytab.0
ren ytab.h ytab2.h