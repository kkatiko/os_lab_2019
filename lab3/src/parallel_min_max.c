#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>

#include "find_min_max.h" 
#include "utils.h"        

int main(int argc, char **argv) {
    int seed = -1;          // Переменная для генерации случайных чисел
    int array_size = -1;   // Переменная для размера массива
    int pnum = -1;         // Переменная для количества дочерних процессов
    bool with_files = false; // Флаг для использования файлов

    // считывание с командной строки
    while (true) {
        int current_optind = optind ? optind : 1; // текущий индекс опции

        static struct option options[] = {
            {"seed", required_argument, 0, 0},          
            {"array_size", required_argument, 0, 0},   
            {"pnum", required_argument, 0, 0},         
            {"by_files", no_argument, 0, 'f'},   
            {0, 0, 0, 0}
        };

        int option_index = 0; // текущее значение
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break; // Выход из цикла при отсутствии значений

        switch (c) {
            case 0: // установка аргументов
                switch (option_index) {
                    case 0: seed = atoi(optarg); break;      
                    case 1: array_size = atoi(optarg); break; 
                    case 2: pnum = atoi(optarg); break;   
                    case 3: with_files = true; break; 
                    default: printf("Index %d is out of options\n", option_index);
                }
                break;
            case 'f': 
                with_files = true; // Если флаг поднят, with_files = true
                break;
            case '?':
                break; // Игнорируем неизвестные аргументы
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }

    // Проверка на наличие лишних аргументов после обработки опций
    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    // Проверка на наличие обязательных аргументов
    if (seed == -1 || array_size == -1 || pnum == -1) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n", argv[0]);
        return 1;
    }

    // Выделение памяти под массив
    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed); // Генерация массива

    int active_child_processes = 0; // Счетчик активных дочерних процессов

    struct timeval start_time;
    gettimeofday(&start_time, NULL); // время начала


    // Дескриптор pipe (0 чтение, 1 запись)
    int pipe_fd[2];
    // Создание pipe для межпроцессного взаимодействия
    if (pipe(pipe_fd) == -1) {
        perror("pipe"); // Выводим ошибку в случае неудачи
        exit(EXIT_FAILURE);
    }

    // Цикл для создания дочерних процессов
for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork(); // Создание нового процесса
    if (child_pid >= 0) {     // Успешное создание процесса
        active_child_processes += 1; // Увеличиваем счетчик активных процессов
        
        if (child_pid == 0) { // Код выполняется в дочернем процессе
            // Если используется pipe
            if (!with_files) {
                close(pipe_fd[0]); // Закрываем конец чтения в дочернем процессе

                // Определяем диапазон индексов для обработки в этом процессе
                int start_index = (array_size / pnum) * i;
                int end_index = (i == pnum - 1) ? array_size : start_index + (array_size / pnum);
                
                int local_min = INT_MAX;  
                int local_max = INT_MIN; 
                
                for (int j = start_index; j < end_index; j++) {
                    if (array[j] < local_min) local_min = array[j];
                    if (array[j] > local_max) local_max = array[j];
                }

                // Записываем результаты в pipe
                write(pipe_fd[1], &local_min, sizeof(int)); 
                write(pipe_fd[1], &local_max, sizeof(int)); 
                
                close(pipe_fd[1]); // Закрываем конец записи после записи данных
            } else {
                // Если используется файл
                char filename[20];
                sprintf(filename, "results_%d.txt", i); // Формируем имя файла для каждого процесса
                FILE *file = fopen(filename, "w"); // Открываем файл на запись

                // Определяем диапазон индексов для обработки в этом процессе
                int start_index = (array_size / pnum) * i;
                int end_index = (i == pnum - 1) ? array_size : start_index + (array_size / pnum);
                
                int local_min = INT_MAX;  
                int local_max = INT_MIN; 
                
                for (int j = start_index; j < end_index; j++) {
                    if (array[j] < local_min) local_min = array[j];
                    if (array[j] > local_max) local_max = array[j];
                }

                // Записываем результаты в файл
                fprintf(file, "%d %d\n", local_min, local_max);
                
                fclose(file); // Закрываем файл после записи
            }
            return 0; 
        }
    } else {
        printf("Fork failed!\n"); 
        return 1;
    }
}

// Ожидание завершения всех дочерних процессов
while (active_child_processes > 0) {
    wait(NULL); 
    active_child_processes -= 1; 
}

struct MinMax min_max;
min_max.min = INT_MAX; 
min_max.max = INT_MIN;

if (!with_files) {
    for (int i = 0; i < pnum; i++) {
        int min_from_pipe, max_from_pipe;

        // Читаем результаты из pipe
        read(pipe_fd[0], &min_from_pipe, sizeof(int)); 
        read(pipe_fd[0], &max_from_pipe, sizeof(int)); 

        // Находим глобальный минимум и максимум
        if (min_from_pipe < min_max.min) min_max.min = min_from_pipe; 
        if (max_from_pipe > min_max.max) min_max.max = max_from_pipe;
    }

    close(pipe_fd[0]); // Закрываем конец чтения после использования
} else {
    for (int i = 0; i < pnum; i++) {
        char filename[20];
        sprintf(filename, "results_%d.txt", i); // Формируем имя файла для чтения результатов
        FILE *file = fopen(filename, "r"); // Открываем файл на чтение

        int min_from_file, max_from_file;
        fscanf(file, "%d %d", &min_from_file, &max_from_file); // Читаем минимумы и максимумы из файла

        // Находим глобальный минимум и максимум
        if (min_from_file < min_max.min) min_max.min = min_from_file; 
        if (max_from_file > min_max.max) min_max.max = max_from_file;

        fclose(file);
    }
}

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL); 
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    free(array); 

    printf("Min: %d\n", min_max.min);   
    printf("Max: %d\n", min_max.max); 
    printf("Elapsed time: %fms\n", elapsed_time); 
    
    return 0;
}
