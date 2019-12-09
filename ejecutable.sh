ALTER SESSION SET NLS_SORT=SPANISH_AI

sqlldr userid= f2/123 control=usuarios.ctl
read
sqlplus f2/123 @consulta.sql
read
sqlplus f2/123 @consulta2.sql
read

