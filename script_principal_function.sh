#Opcion 1: Leer los procesos
valgrind ./crms memfilled.bin 1
#
#Opcion 4: Crear un proceso con un id
valgrind ./crms memfilled.bin 4 136 TarjetaBIP
#Opcion 1: Leer los procesos
valgrind ./crms memfilled.bin 1
#
#Opcion 5: Finaliza un proceso con un id
valgrind ./crms memfilled.bin 5 136
#Opcion 1: Leer los procesos
valgrind ./crms memfilled.bin 1
#
#Opcion 3: Listar los archivos del proceso 9
valgrind ./crms memfilled.bin 3 9
#
#Opcion 2: Indica si un archivo esta en un proceso
valgrind ./crms memfilled.bin 2 9 descarga.png
#
#Opcion 6: Leer el archivo de un proceso
valgrind ./crms memfilled.bin 6 9 theme.wav 34000000
#
#Opcion 8: Borrar el archivo de un proceso
valgrind ./crms memfilled.bin 8 9 theme.wav
#Opcion 2: Indica si un archivo esta en un proceso
valgrind ./crms memfilled.bin 2 9 theme.wav
#
#Opcion 3: Listar los archivos del proceso 0
valgrind ./crms memfilled.bin 3 0
#Opcion 7: Escribir un archivo en un proceso
valgrind ./crms memfilled.bin 7 0 fltest1.mp3 34000000
#Opcion 7: Escribir un archivo en un proceso
valgrind ./crms memfilled.bin 7 0 fltest2.png 70000
#Opcion 3: Listar los archivos del proceso 9
valgrind ./crms memfilled.bin 3 0
#Opcion 6: Leer el archivo de un proceso
valgrind ./crms memfilled.bin 6 0 fltest1.mp3 34000000
#Opcion 6: Leer el archivo de un proceso
valgrind ./crms memfilled.bin 6 0 fltest2.png 34000000
#
#Opcion 4: Crear un proceso con un id
valgrind ./crms memfilled.bin 4 137 TarjetaVIP
#Opcion 1: Leer los procesos
valgrind ./crms memfilled.bin 1
#Opcion 7: Escribir un archivo en un proceso
valgrind ./crms memfilled.bin 7 137 fltest1.mp3 34000000
#Opcion 7: Escribir un archivo en un proceso
valgrind ./crms memfilled.bin 7 137 fltest2.png 34000000
#Opcion 7: Escribir un archivo en un proceso
valgrind ./crms memfilled.bin 7 137 fltest3.jpg 34000000
#Opcion 3: Listar los archivos del proceso 137
valgrind ./crms memfilled.bin 3 137
#Opcion 8: Borrar el archivo de un proceso
valgrind ./crms memfilled.bin 8 137 fltest2.png
#Opcion 3: Listar los archivos del proceso 137
valgrind ./crms memfilled.bin 3 137
#Opcion 7: Escribir un archivo en un proceso
valgrind ./crms memfilled.bin 7 137 fltest5.png 34000000
#Opcion 3: Listar los archivos del proceso 137
valgrind ./crms memfilled.bin 3 137
#Opcion 6: Leer el archivo de un proceso
valgrind ./crms memfilled.bin 6 137 fltest1.mp3 34000000
#Opcion 6: Leer el archivo de un proceso
valgrind ./crms memfilled.bin 6 137 fltest3.jpg 34000000
#Opcion 6: Leer el archivo de un proceso
valgrind ./crms memfilled.bin 6 137 fltest5.png 34000000
#Opcion 5: Finaliza un proceso con un id
valgrind ./crms memfilled.bin 5 137
#Opcion 1: Lista los procesos
valgrind ./crms memfilled.bin 1
#
#ERRORES
./crms memfilled.bin 4 137 SaleMal
#ERROR 1.1: Operation not permitted, se inicia un proceso ya iniciado
./crms memfilled.bin 1
./crms memfilled.bin 4 120 anyth
./crms memfilled.bin 1
./crms memfilled.bin 4 120 anyth
#ERROR 1.2: Operation not permitted, el archivo ya esta abierto
./crms memfilled.bin 9 137 any.gif
#ERROR 1.3: Operation not permitted
#F
#ERROR 2: No such file or directory
./crms memfilled.bin 6 0 idontexist.f 34000000
#ERROR 3: No such process, el proceso no existe
./crms memfilled.bin 3 95
#ERROR 4: No space left on device
./crms memfilled.bin 4 1 anyth1
./crms memfilled.bin 4 2 anyth2
./crms memfilled.bin 4 3 anyth3
./crms memfilled.bin 4 4 anyth4
./crms memfilled.bin 4 5 anyth5
./crms memfilled.bin 4 6 anyth6
./crms memfilled.bin 4 7 anyth7
./crms memfilled.bin 4 8 anyth8
./crms memfilled.bin 4 9 anyth9
./crms memfilled.bin 4 10 anyth10
./crms memfilled.bin 4 11 anyth11
./crms memfilled.bin 4 12 anyth12
./crms memfilled.bin 4 13 anyth13
./crms memfilled.bin 4 14 anyth14
./crms memfilled.bin 4 15 anyth15
./crms memfilled.bin 4 16 anyth16
./crms memfilled.bin 4 17 anyth17
#ERROR 5: Numerical result out of range
./crms memfilled.bin 4 256 outofrange
#ERROR 6: Illegal byte sequence
./crms memfilled.bin 7 137 fltest5.png -1