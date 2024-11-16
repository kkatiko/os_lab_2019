CC=gcc                      # Компилятор gcc
CFLAGS=-I.                  # Флаги компилятора, указывающие на текущую директорию для поиска заголовочных файлов

# таргет all отвечает за сборку всех программ
all: sequential_min_max parallel_min_max ex5

ex5 : ex5.c
    $(CC) -o ex5 ex5.c $(CFLAGS)

# таргет для сборки последовательной программы
sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
    $(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

# таргет для сборки параллельной программы
parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
    $(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

# Правило для сборки объекта utils.o из utils.c
utils.o : utils.h
    $(CC) -o utils.o -c utils.c $(CFLAGS)

# Правило для сборки объекта find_min_max.o из find_min_max.c
find_min_max.o : utils.h find_min_max.h
    $(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

# Цель clean для удаления скомпилированных файлов и объектов
clean :
    rm utils.o find_min_max.o sequential_min_max parallel_min_max ex5
