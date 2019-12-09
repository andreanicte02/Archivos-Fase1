INSERT INTO usuario(carne,nombre,clave)
SELECT DISTINCT tus.carne, tus.nombre, tus.clave
FROM t_us tus
ORDER BY carne;

exit


