#include "find_min_max.h" 
#include <limits.h> 

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) { 
  struct MinMax min_max; 
  min_max.min = INT_MAX; 
  min_max.max = INT_MIN; 

  // Проходим по всем элементам массива в заданном промежутке
  for (unsigned int i = begin; i < end; i++) {
    // Обновляем минимальное значение, если текущий элемент меньше
    if (array[i] < min_max.min) {
      min_max.min = array[i];
    }

    // Обновляем максимальное значение, если текущий элемент больше
    if (array[i] > min_max.max) {
      min_max.max = array[i];
    }
  }

  return min_max; 
} 
