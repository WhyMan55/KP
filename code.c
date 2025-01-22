#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h> 

#define precision 0.015
#define FILENAME "output.csv"
#define MAX_T 100

struct function {
    double x;
    double y;
};

int count = 0;
struct function* values; 
double F1(double);
double F2(double);
double F3(double);
double (*func)(double) = F1;

int input_file(char*);
int output_to_file(struct function values[], int);
double find_max(struct function values[], int);
double find_min(struct function values[], int);
unsigned long long factorial(int n);
int linear_search(struct function values[], int, double);
double differentiate(double (*func)(double), double, double);
void tabulation(int, struct function values[]);
void find_sign_changes(double (*func)(double), struct function values[], int count);
int handle_generated_values_mode(double, double);
int handle_random_values_mode(double, double);

int main() {
    setlocale(LC_CTYPE, "RUS");
    int mode;
    double xmin, xmax, dx;

    while (1) {
        printf("\nМеню:\n");
        printf("0) Выбор функции для работы(по умолчанию F1)\n");
        printf("1) Вывод таблицы значений на экран и в файл\n");
        printf("2) Определение минимального и максимального из вычисленных значений\n");
        printf("3) Линейный поиск значения аргумента при заданном значении функции\n");
        printf("4) Дифференцирование функции в заданной точке\n");
        printf("5) Задание режима и параметров расчета\n");
        printf("6) Определение изменения знака функции\n");
        printf("7) Модуль разности минимума/максимума функции.\n");
        printf("8) Проверка пересечения OY.\n");
        printf("9) Завершение работы\n");
        printf("Выберите опцию: ");
        scanf("%d", &mode);
        switch (mode) {
        case 0: {
            int choice_func;
            printf("Выберите функцию(F1 - 1,F2 - 2,F3 - 3):\n");
            scanf("%d", &choice_func);
            switch (choice_func) {
            case 1: { func = F1; break; }
            case 2: { func = F2; break; }
            case 3: { func = F3; break; }
            default: func = F1; break;
            }
            break;
        }

        case 1:
            tabulation(count, values);
            break;
        case 2:
            if (count == 0) {
                printf("Нет вычисленных значений.\n");
            }
            else {
                printf("Минимальное значение: %lf\n", find_min(values, count));
                printf("Максимальное значение: %lf\n", find_max(values, count));
            }
            break;
        case 3: {
            double target;
            printf("Введите значение функции для поиска: ");
            scanf("%lf", &target);
            int index = linear_search(values, count, target);
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
            printf("Введите приращение h: ");
            scanf("%lf", &h);
            double derivative = differentiate(func, x, h);
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
                printf("Введите имя файла\n");
                char inputfile[255];
                scanf("%s", inputfile);
                input_file(inputfile);
                for (int i = 0; i < count; i++)
                    values[i].y = func(values[i].x);
                break;
            }
            case 2: {
                printf("Введите xmin: ");
                scanf("%lf", &xmin);
                printf("Введите шаг dx: ");
                scanf("%lf", &dx);
                handle_generated_values_mode(xmin, dx);
                break;
            }
            case 3: {
                printf("Введите xmin: ");
                scanf("%lf", &xmin);
                printf("Введите xmax: ");
                scanf("%lf", &xmax);
                handle_random_values_mode(xmin, xmax);
                break;
            }
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
            }
            break;
        }
        case 6: {
                find_sign_changes(func, values, count);
            break;
        }
        case 7:
            printf("Модуль разности макс/мин - %lg \n", fabs(find_max(values, count)-find_min(values, count)));
            break;
        case 8:
            if (isnan(func(0)) || isinf(func(0)))
                printf("Не пересекает OY.\n");
            else printf("Пересекает OY.\n");
        case 9:
            printf("Завершение работы.\n");
            free(values);
            return 0;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }

    return 0;
}

void tabulation(int size, struct function values[])
{
    if (count == 0) {
        printf("Нет вычисленных значений.\n");
    }
    else {
        printf("|    x        |    y        |\n");
        for (int i = 0; i < count; i++) {
            printf("| %11.4lf | %11.4lf |\n", values[i].x, values[i].y);
        }
        int a;
        printf("Вывести в файл?(1 - да; остальные - нет)");
        scanf("%d", &a);
        if (a == 1)
        {
            output_to_file(values, count);
        }
    }
}

double differentiate(double (*func)(double), double x, double h) {
    return (func(x + h) - func(x)) / h;
}

int linear_search(struct function values[], int count, double target) {
    for (int i = 0; i < count; i++) {
        if (fabs(values[i].y - target) < precision) {
            return i;
        }
    }
    return -1;
}

int input_file(char* inputfile) {
    FILE* file = fopen(inputfile, "r");
    if (file == NULL) {
        return -1;
    }
    values = (struct function*)malloc(MAX_T * sizeof(struct function));
    count = 0;
    while (fscanf(file, "%lf", &values[count].x) != EOF) { 
        values[count].y = 0; 
        count++;
    }
    fclose(file);
    return count; 
}

int output_to_file(struct function values[], int count) {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        return -1;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d, %lf, %lf\n", i + 1, values[i].x, values[i].y);
    }
    fclose(file);
    printf("Данные записаны в файл %s\n", FILENAME);
    return 0;
}

void find_sign_changes(double (*func)(double), struct function values[], int count) {
    int flag = 0;
    int d = count;
    for (int i = 1; i <= d; d--) {
        if ((values[d - 1].y > 0 && values[d].y < 0) || (values[d - 1].y < 0 && values[d].y > 0)) {
            printf("Между x = %lf и x = %lf\n", values[i - 1].x, values[i].x);
            flag = 1;
        }
    }
        if (flag != 1)
            printf("Не меняет знак в заданном интервале.\n");
}

double find_max(struct function values[], int count) {
    double max = values[0].y;
    for (int i = 1; i < count; i++) {
        if (values[i].y > max) max = values[i].y;
    }
    return max;
}

double find_min(struct function values[], int count) {
    double min = values[0].y; // Ищем по y
    for (int i = 1; i < count; i++) {
        if (values[i].y < min) min = values[i].y;
    }
    return min;
}

int handle_generated_values_mode(double xmin, double dx) {
    int n = 10;
    count = 0;
    values = (struct function*)malloc(n * sizeof(struct function));
    for (double x = xmin; count < n; x += dx) {
        values[count].x = x;
        values[count].y = func(x);
        count++;
    }
    return count;
}

int handle_random_values_mode(double xmin, double xmax) {
    int n;
    printf("Сколько значений? ");
    scanf("%d", &n);
    values = (struct function*)malloc(n * sizeof(struct function));
    count = 0;
    while (count < n) {
        double x = xmin + 1.f * (xmax - xmin) * rand() / RAND_MAX;
        values[count].x = x;
        values[count].y = func(x);
        count++;
    }
    return count;
}

unsigned long long factorial(int n) {
    if (n == 0 || n == 1) return 1;
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double F1(double x) {
    if (x != 0)
        return log(fabs(sin(x)) + pow(x, 2.0 / 5.0)) / log(5);
    else return NAN;
}

double F2(double x) {
    if (x <= 1) {
        return (pow(x, 2) - 7 * x + 3) / exp(x);
    }
    else {
        return log10((pow(x, 2)));
    }

}

double F3(double x) {
    if (x > 1)
    {
        double sum = 0.0;
        for (int n = 1; n < MAX_T; n++) {
            double term = ((double)factorial(2 * n) / ((pow(2, 2 * n) * pow(factorial(n), 2)))) * (pow(x, -2 * n) / (2 * n));
            sum += term;
            if (fabs(term) < precision) {
                break;
            }
        }
        return log(2 * x) - sum;
    }
    else return NAN;
}
