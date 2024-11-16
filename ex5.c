#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    pid_t pid = fork(); // Создаем новый процесс

    if (pid == 0) {
        // Код выполняется в дочернем процессе
        execl("./sequential_min_max", "sequential_min_max", "--seed", "10", "--array_size", "10", NULL); // Запускаем программу
        perror("execl failed"); // Если execl возвращает, значит произошла ошибка
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Ошибка при создании процесса
        perror("Fork failed");
        return 1;
    }
    
    printf("finished");
    return 0;
}
