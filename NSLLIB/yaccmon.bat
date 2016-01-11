yacc -dv nslmsmon.y
rmlines ytab.c
del ytab.c
yactorec <ytab.0 >ytab.c
del ytab.0
