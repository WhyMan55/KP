#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <locale.h>

#define MAX_VALUES 9999
#define FILENAME "output.csv"

double F1(double x) {
    return log(fabs(sin(x)) + pow(x, 2.0 / 5.0)) / log(5);
}

double F2(double x) {
    if (x <= 1) {
        return (pow(x, 2) - 7 * x + 3) / exp(x);
    }
    else {
        return log(pow(x, 2));
    }
}

void output_to_file(double values[], int count) {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d, %lf\n", i + 1, values[i]);
    }
    fclose(file);
    printf("Данные записаны в файл %s\n", FILENAME);
}

void find_min_max(double values[], int count, double* min, double* max) {
    *min = *max = values[0];
    for (int i = 1; i < count; i++) {
        if (values[i] < *min) *min = values[i];
        else *max = values[i];
    }
}

int linear_search(double values[], int count, double target, double precision) {
    for (int i = 0; i < count; i++) {
        if (fabs(values[i] - target) < precision) {
            return i;
        }
    }
    return -1;
}

double differentiate(double (*func)(double), double x, double h) {
    return (func(x + h) - func(x)) / h;
}

int main() {
    setlocale(LC_CTYPE, "RUS");
    double values[MAX_VALUES];
    int count = 0;
    int mode, n;
    double xmin, xmax, dx, precision;

    while (1) {
        printf("\nМеню:\n");
        printf("1) Вывод таблицы значений на экран и в файл\n");
        printf("2) Определение минимального и максимального из вычисленных значений\n");
        printf("3) Линейный поиск значения аргумента при заданном значении функции\n");
        printf("4) Дифференцирование функции в заданной точке\n");
        printf("5) Задание режима и параметров расчета\n");
        printf("6) Индивидуальное задание\n");
        printf("7) Завершение работы\n");
        printf("Выберите опцию: ");
        scanf("%d", &mode);

        switch (mode) {
        case 1:
            if (count == 0) {
                printf("Нет вычисленных значений.\n");
            }
            else {
                for (int i = 0; i < count; i++) {
                    printf("|%3.d | %6.4lf|\n", i + 1, values[i]);
                }
                output_to_file(values, count);
            }
            break;
        case 2:
            if (count == 0) {
                printf("Нет вычисленных значений.\n");
            }
            else {
                double min, max;
                find_min_max(values, count, &min, &max);
                printf("Минимальное значение: %lf\n", min);
                printf("Максимальное значение: %lf\n", max);
            }
            break;
        case 3: {
            double target;
            printf("Введите значение функции для поиска: ");
            scanf("%lf", &target);
            printf("Введите точность: ");
            scanf("%lf", &precision);
            int index = linear_search(values, count, target, precision);
            if (index != -1) {
                printf("Значение найдено на индексе: %d\n", index);
            }
            else {
                printf("Значение не найдено.\n");
            }
            break;
        }
        case 4: {
            double x, h;
            printf("Введите точку для дифференцирования: ");
            scanf("%lf", &x);
            printf("Введите шаг h: ");
            scanf("%lf", &h);
            double derivative = differentiate(F1, x, h);
            printf("Производная в точке %lf: %lf\n", x, derivative);
            break;
        }
        case 5: {
            int choice;
            printf("Выберите режим:\n");
            printf("1) Заданные значения (cvs-файл)\n");
            printf("2) Сгенерированные от xmin с шагом dx(n=10)\n");
            printf("3) Случайные значения из интервала (xmin; xmax)\n");
            printf("Выберите режим: ");
            scanf("%d", &choice);

            switch (choice) {
            case 1: {
                FILE* file = fopen("input.csv", "r");
                if (file == NULL) {
                    printf("Ошибка открытия файла.\n");
                    break;
                }
                count = 0;
                while (fscanf(file, "%lf", &values[count]) != EOF) {
                    count++;
                }
                fclose(file);
                break;
            }
            case 2: {
                printf("Введите xmin: ");
                scanf("%lf", &xmin);
                printf("Введите шаг dx: ");
                scanf("%lf", &dx);
                n = 10;
                count = 0;
                for (double x = xmin; count < n; x += dx) {
                    values[count++] = F1(x);
                }
                break;
            }
            case 3: {
                printf("Сколько значений? ");
                scanf("%d", &n);
                printf("Введите xmin: ");
                scanf("%lf", &xmin);
                printf("Введите xmax: ");
                scanf("%lf", &xmax);
                count = 0;
                while (count < n) {
                    double x = xmin + 1.f* (xmax - xmin)*rand()/RAND_MAX;
                        values[count++] = F1(x);
                }
                break;
            }
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
            }
            break;
        }
        case 6:
            printf("А его пока нет\n");
            break;
        case 7:
            printf("Завершение работы.\n");
            return 0;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }

    return 0;
}