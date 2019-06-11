COMP = gcc
OBJS =  $(patsubst %.c,%.o,$(wildcard *.c)) # Todos los .o en carpeta
HEADERS = $(patsubst %.c,%.h,$(wildcard *.c)) # Todos los .h en carpeta
CFLAGS = -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion
EXE = pruebas

# Si escribo make se ejecuta las reglas que indique a continuacion
all: compilar valgrind

# Correr el EXE
run: $(EXE)
	./$(EXE)

# Compila el programa principal
compilar: $(OBJS)
	$(COMP) $(CFLAGS) -o $(EXE) $(OBJS)

# Genera los archivos.o individuales
%.o: %.c $(HEADERS)

# Elimina los archivos.o
clean: 
	rm -f $(OBJS)

# Elimina el archivo EXE
clean_exe:
	rm -f $(EXE)

# Elimina los arhivos o y el EXE
clean_all: clean clean_exe

# Correr pruebas con valgrind
valgrind: $(EXE)
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./$(EXE) tweets_head.txt

# Corre pruebas con valgreen (valgrind de la catedra)
valgreen: $(EXE)
	valgreen ./$(EXE)

#######################################################################
##						 COMO UTILIZARLO                             ##
#######################################################################
##																	 ##
## "make": compila y corre valgrind.                                 ##
## "make compilar": compila.                                         ##
## "make run": corre el programa.                                    ##
## "make valgrind":  corre valgrind.                                 ##
## "make clean": elimina los .o de la carpeta                        ##
## "make clean_exe": elimina el EXE de la carpeta                    ##
## "make clean_all": elimina el .o y EXE de la carpeta               ##
##																	 ##
#######################################################################

#### Apuntes varios:
# $< (refiere a la primer dependencia)
# $? (señala al todos los pre requisitos)
# $@ (se cambia por el nombre de la regla)
## Explicacion de linea magica " $(patsubst %.c,%.h,$(wildcard *.c)) "
## https://stackoverflow.com/questions/34219186/what-is-the-difference-between-and-in-a-makefile