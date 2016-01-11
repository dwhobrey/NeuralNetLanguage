set YACCPAR=C:\usr\bin
yacc -dv nslmsmon.y
rmlines ytab.c
del youtput
del ytab.c
ren ytab.0 ytab.c
