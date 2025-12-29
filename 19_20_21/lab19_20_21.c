#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "functions.h"
#define MAX_SIZE 100
#define FILENAME "employees.txt"
#define WINTER_FILE "winter.txt"
#define WINTER_NEW_FILE "winter_new.txt"
#define MAX_LINE_LENGTH 256



int main() {
    system("chcp 1251");
    setlocale(LC_CTYPE, "RUS");
    employee_t employees[MAX_SIZE];
    int size = 0;
    int choice;

    do {
        printf("\n=== МЕНЮ ПРОГРАММЫ ===\n");
        printf("1. Заполнить массив случайными значениями\n");
        printf("2. Найти сотрудника\n");
        printf("3. Отсортировать по зарплате\n");
        printf("4. Вывести сотрудников, родившихся в мае\n");
        printf("5. Сохранить в файл\n");
        printf("6. Загрузить из файла\n");
        printf("7. Обработать файл winter.txt (Лаб 20)\n");
        printf("8. Выход их программы\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            printf("Введите количество записей (макс. %d): ", MAX_SIZE);
            int new_size;
            scanf("%d", &new_size);
            if (new_size > 0 && new_size <= MAX_SIZE) {
                if (fill_arr_random(employees, new_size)) {
                    size = new_size;
                    printf("\n==========================================================================================\n");
                    printf("|  Фамилия   |    Имя     |  Отчество   |    Должность    | Зарплата | Дата рождения |\n");
                    printf("==========================================================================================\n");
                    print_array(employees, size);
                    printf("==========================================================================================\n");
                }
                else {
                    printf("Ошибка при заполнении массива.\n");
                }
            }
            else {
                printf("Неверное количество записей.\n");
            }
            break;
        }

        case 2: {
            if (size == 0) {
                printf("Массив пуст. Нет записей для поиска.\n");
                break;
            }

            int search_type;
            printf("\nВыберите поле для поиска:\n");
            printf("1. Фамилия\n");
            printf("2. Должность\n");
            printf("3. Зарплата\n");
            printf("Ваш выбор: ");
            scanf("%d", &search_type);

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            int found_indices[MAX_SIZE];
            int found_count = 0;
            int search_success = 0;
            char search_str[30];
            int search_salary = 0;

            switch (search_type) {
            case 1: {
                printf("Введите фамилию для поиска: ");
                scanf("%19s", search_str);
                search_success = search_by_lastname(employees, size, search_str, found_indices, &found_count);
                if (search_success) {
                    printf("\nРезультаты поиска по фамилии '%s':\n", search_str);
                }
                break;
            }
            case 2: {
                printf("Введите должность для поиска: ");
                scanf("%29s", search_str);
                search_success = search_by_position(employees, size, search_str, found_indices, &found_count);
                if (search_success) {
                    printf("\nРезультаты поиска по должности '%s':\n", search_str);
                }
                break;
            }
            case 3: {
                printf("Введите зарплату для поиска: ");
                scanf("%d", &search_salary);
                search_success = search_by_salary(employees, size, search_salary, found_indices, &found_count);
                if (search_success) {
                    printf("\nРезультаты поиска по зарплате %d:\n", search_salary);
                }
                break;
            }
            default: {
                printf("Неверный выбор.\n");
                break;
            }
            }

            if (search_type >= 1 && search_type <= 3 && search_success) {
                if (found_count > 0) {
                    printf("==========================================================================================\n");
                    printf("|  Фамилия   |    Имя     |  Отчество   |    Должность    | Зарплата | Дата рождения |\n");
                    printf("==========================================================================================\n");
                    for (int i = 0; i < found_count; i++) {
                        int idx = found_indices[i];
                        printf("| %-10s | %-10s | %-12s | %-15s | %8d | %11s |\n",
                            employees[idx].lastName, employees[idx].firstName, employees[idx].patronymic,
                            employees[idx].position, employees[idx].salary, employees[idx].birthDate);
                    }
                    printf("==========================================================================================\n");
                    printf("Найдено записей: %d\n", found_count);
                }
                else {
                    printf("Записи не найдены.\n");
                }
            }
            else if (search_type >= 1 && search_type <= 3 && !search_success) {
                printf("Ошибка при выполнении поиска.\n");
            }
            break;
        }

        case 3: {
            if (size == 0) {
                printf("Массив пуст. Нечего сортировать.\n");
                break;
            }
            if (sort_array(employees, size)) {
                printf("Сортировка по зарплате выполнена.\n");
                print_array(employees, size);
            }
            else {
                printf("Ошибка при сортировке.\n");
            }
            break;
        }

        case 4: {
            if (size == 0) {
                printf("Массив пуст.\n");
                break;
            }

            int found_indices[MAX_SIZE];
            int found_count = 0;

            if (find_born_in_may(employees, size, found_indices, &found_count)) {
                printf("\n=== СОТРУДНИКИ, РОДИВШИЕСЯ В МАЕ ===\n");
                if (found_count > 0) {
                    printf("==========================================================================================\n");
                    printf("|  Фамилия   |    Имя     |  Отчество   |    Должность    | Зарплата | Дата рождения |\n");
                    printf("==========================================================================================\n");
                    for (int i = 0; i < found_count; i++) {
                        int idx = found_indices[i];
                        printf("| %-10s | %-10s | %-12s | %-15s | %8d | %11s |\n",
                            employees[idx].lastName, employees[idx].firstName, employees[idx].patronymic,
                            employees[idx].position, employees[idx].salary, employees[idx].birthDate);
                    }
                    printf("==========================================================================================\n");
                    printf("Найдено сотрудников, родившихся в мае: %d\n", found_count);
                }
                else {
                    printf("Нет сотрудников, родившихся в мае.\n");
                }
            }
            else {
                printf("Ошибка при поиске сотрудников.\n");
            }
            break;
        }

        case 5: {
            if (size > 0) {
                if (output_file(FILENAME, employees, size)) {
                    printf("Данные успешно записаны в файл '%s' (%d записей).\n", FILENAME, size);
                }
                else {
                    printf("Ошибка при записи в файл.\n");
                }
            }
            else {
                printf("Массив пуст. Нет данных для сохранения.\n");
            }
            break;
        }

        case 6: {
            int loaded_size = 0;
            if (input_file(FILENAME, employees, MAX_SIZE, &loaded_size)) {
                size = loaded_size;
                if (size > 0) {
                    printf("Загружено %d записей из файла '%s'.\n", size, FILENAME);
                    print_array(employees, size);
                }
                else {
                    printf("Файл пуст или содержит неверные данные.\n");
                }
            }
            else {
                printf("Ошибка при чтении файла или файл не найден.\n");
            }
            break;
        }

        case 7: {
            printf("\n=== ЛАБОРАТОРНАЯ РАБОТА 20 - ОБРАБОТКА ФАЙЛА winter.txt ===\n");
            printf("Содержимое файла winter.txt:\n");
            printf("==============================\n");
            if (winter_file()) {
                printf("\n==============================\n");
                printf("Результат записан в файл '%s'\n", WINTER_NEW_FILE);
            }
            else {
                printf("Ошибка при обработке файла.\n");
            }
            break;
        }

        case 8:
            printf("Выход из программы.\n");
            break;

        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 8);

    return 0;
}

