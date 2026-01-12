#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "functions.h"
#define MAX_SIZE 50
#define FILENAME "employees.txt"
#define WINTER_FILE "winter.txt"
#define WINTER_NEW_FILE "winter_new.txt"
#define MAX_LINE_LENGTH 256

int getRandomNumber(int min, int max) {
    if (min > max) return 0;
    return rand() % (max - min + 1) + min;
}

int getRandomDate(char* date) {
    if (date == NULL) return 0;

    int day = getRandomNumber(1, 28);
    int month = getRandomNumber(1, 12);
    int year = getRandomNumber(1960, 2000);

    if (sprintf(date, "%02d.%02d.%04d", day, month, year) != 10) {
        return 0;
    }
    return 1;
}

int fill_arr_random(employee_t* employees, int size) {
    if (employees == NULL || size <= 0 || size > MAX_SIZE) return 0;

    srand(time(NULL));

    const char* firstNames[] = { "Артём", "Михаил", "Дмитрий", "Роман", "Василий",
                               "Мария", "Полина", "Элизавета", "Ольга", "Екатерина" };
    const char* lastNames[] = { "Когтин", "Васин", "Молчунова", "Борисова", "Душков",
                              "Куратов", "Арбузов", "Поздняков", "Шевченко", "Малых" };
    const char* patronymics[] = { "Олегович", "Викторович", "Владимировна", "Игоревна", "Никитич",
                                "Максимович", "Дмитриевич", "Васильевна", "Эдуардович" };
    const char* positions[] = { "Менеджер", "Программист", "Бухгалтер", "Дизайнер",
                              "Аналитик", "Тестировщик", "Администратор", "Директор",
                              "Маркетолог", "Консультант" };

    int firstNamesCount = sizeof(firstNames) / sizeof(firstNames[0]);
    int lastNamesCount = sizeof(lastNames) / sizeof(lastNames[0]);
    int patronymicsCount = sizeof(patronymics) / sizeof(patronymics[0]);
    int positionsCount = sizeof(positions) / sizeof(positions[0]);

    for (int i = 0; i < size; i++) {
        strcpy(employees[i].firstName, firstNames[rand() % firstNamesCount]);
        strcpy(employees[i].lastName, lastNames[rand() % lastNamesCount]);
        strcpy(employees[i].patronymic, patronymics[rand() % patronymicsCount]);
        strcpy(employees[i].position, positions[rand() % positionsCount]);
        employees[i].salary = getRandomNumber(30000, 150000);

        if (getRandomDate(employees[i].birthDate) == 0) {
            return 0;
        }
    }
    return 1;
}

void print_array(employee_t* employees, int size) {
    if (size == 0) {
        printf("Массив пуст.\n");
        return;
    }
    for (int i = 0; i < size; i++) {
        printf("| %-10s | %-10s | %-12s | %-15s | %8d | %11s |\n",
            employees[i].lastName, employees[i].firstName, employees[i].patronymic,
            employees[i].position, employees[i].salary, employees[i].birthDate);
    }
}

int search_by_lastname(employee_t* employees, int size, char* lastName, int* found_indices, int* found_count) {
    if (employees == NULL || size <= 0 || lastName == NULL || found_indices == NULL || found_count == NULL)
        return 0;

    int count = 0;
    for (int i = 0; i < size && count < MAX_SIZE; i++) {
        if (strcmp(employees[i].lastName, lastName) == 0) {
            found_indices[count] = i;
            count++;
        }
    }
    *found_count = count;
    return 1;
}

int search_by_position(employee_t* employees, int size, char* position, int* found_indices, int* found_count) {
    if (employees == NULL || size <= 0 || position == NULL || found_indices == NULL || found_count == NULL)
        return 0;

    int count = 0;
    for (int i = 0; i < size && count < MAX_SIZE; i++) {
        if (strcmp(employees[i].position, position) == 0) {
            found_indices[count] = i;
            count++;
        }
    }
    *found_count = count;
    return 1;
}

int search_by_salary(employee_t* employees, int size, int salary, int* found_indices, int* found_count) {
    if (employees == NULL || size <= 0 || found_indices == NULL || found_count == NULL)
        return 0;

    int count = 0;
    for (int i = 0; i < size && count < MAX_SIZE; i++) {
        if (employees[i].salary == salary) {
            found_indices[count] = i;
            count++;
        }
    }
    *found_count = count;
    return 1;
}

int compare_salaries(const void* a, const void* b) {
    const employee_t* emp1 = (const employee_t*)a;
    const employee_t* emp2 = (const employee_t*)b;

    if (emp1->salary < emp2->salary) return -1;
    if (emp1->salary > emp2->salary) return 1;
    return 0;
}

int sort_array(employee_t* employees, int size) {
    if (employees == NULL || size <= 0) return 0;

    // Используем стандартную функцию qsort с нашей функцией сравнения
    qsort(employees, size, sizeof(employee_t), compare_salaries);

    return 1;
}

int output_file(char* filename, employee_t* arr, int n) {
    if (filename == NULL || arr == NULL || n <= 0) return 0;

    FILE* file = fopen(filename, "w");
    if (file == NULL) return 0;

    for (int i = 0; i < n; i++) {
        if (fprintf(file, "%s %s %s %s %d %s\n",
            arr[i].lastName,
            arr[i].firstName,
            arr[i].patronymic,
            arr[i].position,
            arr[i].salary,
            arr[i].birthDate) < 0) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

int input_file(char* filename, employee_t* arr, int max_size, int* count) {
    if (filename == NULL || arr == NULL || max_size <= 0 || count == NULL) return 0;

    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;

    int cnt = 0;
    while (cnt < max_size &&
        fscanf(file, "%19s %19s %19s %29s %d %10s",
            arr[cnt].lastName,
            arr[cnt].firstName,
            arr[cnt].patronymic,
            arr[cnt].position,
            &arr[cnt].salary,
            arr[cnt].birthDate) == 6) {
        cnt++;
    }

    fclose(file);
    *count = cnt;
    return 1;
}

int find_born_in_may(employee_t* employees, int size, int* found_indices, int* found_count) {
    if (employees == NULL || size <= 0 || found_indices == NULL || found_count == NULL) return 0;

    int count = 0;
    for (int i = 0; i < size && count < MAX_SIZE; i++) {
        char monthStr[3];
        monthStr[0] = employees[i].birthDate[3];
        monthStr[1] = employees[i].birthDate[4];
        monthStr[2] = '\0';

        int month = atoi(monthStr);

        if (month == 5) {
            found_indices[count] = i;
            count++;
        }
    }

    *found_count = count;
    return 1;
}

int winter_file() {
    FILE* input_file = fopen(WINTER_FILE, "r");
    if (input_file == NULL) {
        printf("Не удалось открыть файл '%s' для чтения.\n", WINTER_FILE);
        return 0;
    }

    FILE* output_file = fopen(WINTER_NEW_FILE, "w");
    if (output_file == NULL) {
        printf("Не удалось открыть файл '%s' для записи.\n", WINTER_NEW_FILE);
        fclose(input_file);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    printf("Исходное содержимое:\n");

    while (fgets(line, sizeof(line), input_file) != NULL) {
        line_number++;

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        printf("%s\n", line);

        fprintf(output_file, "!%s\n", line);
    }

    fclose(input_file);
    fclose(output_file);

    printf("\nОбработанное содержимое (из файла '%s'):\n", WINTER_NEW_FILE);

    output_file = fopen(WINTER_NEW_FILE, "r");
    if (output_file != NULL) {
        while (fgets(line, sizeof(line), output_file) != NULL) {
            printf("%s", line);
        }
        fclose(output_file);
    }

    return 1;
}
