#include <stdio.h>
#include <stdlib.h>
#include "find_min_max.h" // Подключаем заголовочный файл с объявлениями функций для поиска минимального и максимального значений
#include "utils.h"        // Подключаем заголовочный файл с утилитами для генерации массива

int main(int argc, char **argv) {
    // Проверяем количество аргументов командной строки
    if (argc != 3) {
        printf("Usage: %s seed arraysize\n", argv[0]); // Выводим сообщение о правильном использовании программы
        return 1;
    }

    // Преобразуем первый аргумент в целое число
    int seed = atoi(argv[1]);
    if (seed <= 0) {
        printf("seed is a positive number\n"); // Проверяем, что seed положительное число
        return 1;
    }

    int array_size = atoi(argv[2]);
    if (array_size <= 0) {
        printf("array_size is a positive number\n"); 
        return 1;
    }

    // Выделяем память для массива целых чисел размером array_size
    int *array = malloc(array_size * sizeof(int));
    if (array == NULL) { // Проверяем, успешно ли выделена память
        printf("Memory allocation failed\n"); // Если нет, выводим сообщение об ошибке
        return 1;
    }

    // Генерируем массив целых чисел
    GenerateArray(array, array_size, seed);

    // Находим минимальные и максимальные значения в массиве
    struct MinMax min_max = GetMinMax(array, 0, array_size);

    free(array);

    // Выводим найденные минимальное и максимальное значения на экран
    printf("min: %d\n", min_max.min);
    printf("max: %d\n", min_max.max);

    return 0;
}
