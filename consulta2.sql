INSERT INTO ASINGAR_ROL (USUARIO_CARNE,rol_rol)
SELECT DISTINCT tus.carne, tus.tipo
FROM t_us tus
ORDER BY tus.carne;

INSERT INTO ASINGAR_ROL (USUARIO_CARNE,rol_rol)
SELECT DISTINCT tus.carne, 'administrador' 
FROM t_us tus  WHERE tus.tipo = 'root'
ORDER BY carne;

exit
