Para compilar el Hello World de FUSE hay que usar los siguientes parámetros:

gcc helloFuse.c -o helloFuse -lfuse -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=22


Una vez compilado, usamos el binario helloFuse para montar el sistema de archivos (hay que ser root):


mkdir helloFileSystem
sudo ./helloFuse helloFileSystem/


Aún así, vamos a necesitar permisos de root para navegar el FS, así que tenemos que hacer 'sudo ls' para listar los archivos, y demás.
