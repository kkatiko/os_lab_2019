#!/bin/bash

# Создаем файл numbers.txt с 150 случайными числами
# Используем od для извлечения случайных чисел из /dev/urandom

# Генерируем 150 случайных чисел, выводим в numbers.txt
#od -A n -N2 -i /dev/urandom | head -n 150 > numbers.txt
od -N2 -i /dev/urandom | head -n 150 > numbers.txt
