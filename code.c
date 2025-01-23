#define _CRT_SECURE_NO_WARNINGS // Отключение предупреждений 
#include <stdio.h> // Подключение стандартной библиотеки ввода/вывода
#include <stdlib.h> // Подключение стандартной библиотеки для работы с памятью и другими утилитами
#include <math.h> // Подключение математической библиотеки для работы с математическими функциями
#include <locale.h> // Подключение библиотеки для работы с локализацией

#define precision 0.015 // Определение точности для вычислений
#define FILENAME "output.csv" // Имя файла для вывода данных
#define MAX_T 100 // Максимальное количество повторов для F3

// Определение структуры для хранения координат (x, y) функции
struct function {
    double x; // Значение аргумента
    double y; // Значение функции
};

int count = 0; // Глобальная переменная для хранения количества вычисленных значений
struct function* values; // Указатель на массив значений функции
double F1(double); // Прототип функции F1
double F2(double); // Прототип функции F2
double F3(double); // Прототип функции F3
double (*func)(double) = F1; // Указатель на функцию, по умолчанию указывает на F1

// Прототипы функций для работы с данными
int input_file(char*); // Функция для чтения данных из файла
int output_to_file(struct function values[], int); // Функция для записи данных в файл
double find_max(struct function values[], int); // Функция для нахождения максимального значения
double find_min(struct function values[], int); // Функция для нахождения минимального значения
unsigned long long factorial(int n); // Функция для вычисления факториала
int linear_search(struct function values[], int, double); // Функция для линейного поиска
double differentiate(double (*func)(double), double, double); // Функция для дифференцирования
void tabulation(int, struct function values[]); // Функция для табуляции значений
void find_sign_changes(double (*func)(double), struct function values[], int count); // Функция для нахождения изменений знака функции
int handle_generated_values_mode(double, double); // Функция для обработки сгенерированных значений
int handle_random_values_mode(double, double); // Функция для обработки случайных значений

int main() {
    setlocale(LC_CTYPE, "RUS"); // Установка русской локали для корректного отображения текста
    int mode; // Переменная для хранения выбранного режима работы
    double xmin, xmax, dx; // Переменные для хранения границ и шага

    while (1) { // Бесконечный цикл для меню
        // Вывод меню на экран
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
        scanf("%d", &mode); // Считывание выбранной опции

        switch (mode) { // Обработка выбранной опции
        case 0: { // Выбор функции
            int choice_func; // Переменная для хранения выбора функции
            printf("Выберите функцию(F1 - 1,F2 - 2,F3 - 3):\n");
            scanf("%d", &choice_func); // Считывание выбора функции
            switch (choice_func) { // Установка выбранной функции
            case 1: { func = F1; break; }
            case 2: { func = F2; break; }
            case 3: { func = F3; break; }
            default: func = F1; break; // По умолчанию устанавливаем F1
            }
            break;
        case 1: // Вывод таблицы значений
            tabulation(count, values); // Вызов функции для табуляции значений
            break;
        case 2: // Определение минимального и максимального значений
            if (count == 0) { // Проверка, были ли вычислены значения
                printf("Нет вычисленных значений.\n"); // Сообщение об отсутствии значений
            }
            else {
                // Вывод минимального и максимального значений
                printf("Минимальное значение: %lf\n", find_min(values, count));
                printf("Максимальное значение: %lf\n", find_max(values, count));
            }
            break;
        case 3: { // Линейный поиск значения функции
            double target; // Переменная для хранения искомого значения
            printf("Введите значение функции для поиска: ");
            scanf("%lf", &target); // Считывание искомого значения
            int index = linear_search(values, count, target); // Поиск значения
            if (index != -1) { // Если значение найдено
                printf("Значение найдено на индексе: %d\n", index);
            }
            else {
                printf("Значение не найдено.\n"); // Сообщение о том, что значение не найдено
            }
            break;
        }
        case 4: { // Дифференцирование функции
            double x, h; // Переменные для точки и приращения
            printf("Введите точку для дифференцирования: ");
            scanf("%lf", &x); // Считывание точки
            printf("Введите приращение h: ");
            scanf("%lf", &h); // Считывание приращения
            double derivative = differentiate(func, x, h); // Вычисление производной
            printf("Производная в точке %lf: %lf\n", x, derivative); // Вывод результата
            break;
        }
        case 5: { // Выбор режима и параметров расчета
            int choice; // Переменная для хранения выбора режима
            printf("Выберите режим:\n");
            printf("1) Заданные значения (cvs-файл)\n");
            printf("2) Сгенерированные от xmin с шагом dx(n=10)\n");
            printf("3) Случайные значения из интервала (xmin; xmax)\n");
            printf("Выберите режим: ");
            scanf("%d", &choice); // Считывание выбора режима
            switch (choice) { // Обработка выбора режима
            case 1: { // Заданные значения из файла
                printf("Введите имя файла\n");
                char inputfile[255]; // Массив для хранения имени файла
                scanf("%s", inputfile); // Считывание имени файла
                input_file(inputfile); // Вызов функции для чтения данных из файла
                for (int i = 0; i < count; i++) // Вычисление значений функции для считанных x
                    values[i].y = func(values[i].x);
                break;
            }
            case 2: { // Сгенерированные значения
                printf("Введите xmin: ");
                scanf("%lf", &xmin); // Считывание xmin
                printf("Введите шаг dx: ");
                scanf("%lf", &dx); // Считывание шага
                handle_generated_values_mode(xmin, dx); // Вызов функции для обработки сгенерированных значений
                break;
            }
            case 3: { // Случайные значения
                printf("Введите xmin: ");
                scanf("%lf", &xmin); // Считывание xmin
                printf("Введите xmax: ");
                scanf("%lf", &xmax); // Считывание xmax
                handle_random_values_mode(xmin, xmax); // Вызов функции для обработки случайных значений
                break;
            }
            default:
                printf("Неверный выбор. Попробуйте снова.\n"); // Сообщение об ошибке выбора
            }
            break;
        }
        case 6: { // Определение изменения знака функции
            find_sign_changes(func, values, count); // Вызов функции для нахождения изменений знака
            break;
        }
        case 7: // Вычисление модуля разности максимума и минимума
            printf("Модуль разности макс/мин - %lg \n", fabs(find_max(values, count) - find_min(values, count))); // Вывод результата
            break;
        case 8: // Проверка пересечения с осью OY
            if (isnan(func(0)) || isinf(func(0))) { // Проверка, есть ли значение функции в точке x=0
                printf("Функция не пересекает OY.\n"); // Сообщение о том, что функция не определена
            }
            else {
                printf("Функция пересекает OY\n"); // Вывод сообщения о том, что функция пересекает OY
            }
            break;
        case 9: // Завершение работы программы
            printf("Завершение работы.\n"); // Сообщение о завершении работы
            free(values); // Освобождение выделенной памяти
            return 0; // Выход из программы
        default: // Обработка неверного выбора
            printf("Неверный выбор. Попробуйте снова.\n"); // Сообщение об ошибке выбора
            break;
        }
        }
    }
}
    // Функция для табуляции значений функции
    void tabulation(int size, struct function values[]) {
        if (count == 0) { // Проверка, есть ли вычисленные значения
            printf("Нет вычисленных значений.\n"); // Сообщение об отсутствии значений
        }
        else {
            printf("|    x        |    y        |\n"); // Заголовок таблицы
            for (int i = 0; i < count; i++) { // Цикл по всем значениям
                printf("| %11.4lf | %11.4lf |\n", values[i].x, values[i].y); // Вывод значений x и y
            }
            int a;
            printf("Вывести в файл?(1 - да; остальные - нет)"); // Запрос на вывод в файл
            scanf("%d", &a); // Считывание выбора пользователя
            if (a == 1) {
                output_to_file(values, count); // Вызов функции для записи в файл
            }
        }
    }

    // Функция для вычисления производной функции в заданной точке
    double differentiate(double (*func)(double), double x, double h) {
        return (func(x + h) - func(x)) / h; // Формула для численного дифференцирования
    }

    // Линейный поиск значения в массиве
    int linear_search(struct function values[], int count, double target) {
        for (int i = 0; i < count; i++) { // Цикл по всем значениям
            if (fabs(values[i].y - target) < precision) { // Проверка на близость значения
                return i; // Возвращаем индекс найденного значения
            }
        }
        return -1; // Если значение не найдено
    }

    // Функция для считывания данных из файла
    int input_file(char* inputfile) {
        FILE* file = fopen(inputfile, "r"); // Открытие файла для чтения
        if (file == NULL) {
            return -1; // Ошибка при открытии файла
        }
        values = (struct function*)malloc(MAX_T * sizeof(struct function)); // Выделение памяти для значений
        count = 0; // Сброс счетчика
        while (fscanf(file, "%lf", &values[count].x) != EOF) { // Считывание значений x из файла
            values[count].y = 0; // Инициализация y
            count++; // Увеличение счетчика
        }
        fclose(file); // Закрытие файла
        return count; // Возвращаем количество считанных значений
    }

    // Функция для записи значений в файл
    int output_to_file(struct function values[], int count) {
        FILE* file = fopen(FILENAME, "w"); // Открытие файла для записи
        if (file == NULL) {
            return -1; // Ошибка при открытии файла
        }
        for (int i = 0; i < count; i++) { // Цикл по всем значениям
            fprintf(file, "%d, %lf, %lf\n", i + 1, values[i].x, values[i].y); // Запись значений в файл
        }
        fclose(file); // Закрытие файла
        printf("Данные записаны в файл %s\n", FILENAME); // Сообщение о записи
        return 0; // Успешное завершение
    }

    // Функция для нахождения изменений знака функции
    void find_sign_changes(double (*func)(double), struct function values[], int count) {
        int flag = 0; // Флаг для отслеживания изменений знака
        int d = count; // Сохраняем значение count
        for (int i = 1; i < d; d--) { // Цикл по значениям
            if (values[d - 1].y * values[d].y < 0) { // Проверка на изменение знака
                printf("Между x = %lf и x = %lf\n", values[i - 1].x, values[i].x); // Вывод интервала
                flag = 1; // Установка флага
            }
        }
        if (flag != 1) // Если не было изменений знака
            printf("Не меняет знак в заданном интервале.\n"); // Сообщение об отсутствии изменений
    }

    // Функция для нахождения максимального значения
    double find_max(struct function values[], int count) {
        double max = values[0].y; // Инициализация максимального значения
        for (int i = 1; i < count; i++) { // Цикл по всем значениям
            if (values[i].y > max) max = values[i].y; // Обновление максимума
        }
        return max; // Возврат максимального значения
    }

    // Функция для нахождения минимального значения
    double find_min(struct function values[], int count) {
        double min = values[0].y; // Инициализация минимального значения
        for (int i = 1; i < count; i++) { // Цикл по всем значениям
            if (values[i].y < min) min = values[i].y; // Обновление минимума
        }
        return min; // Возврат минимального значения
    }

    // Обработка режима генерации значений
    int handle_generated_values_mode(double xmin, double dx) {
        int n = 10; // Количество значений
        count = 0; // Сброс счетчика
        values = (struct function*)malloc(n * sizeof(struct function)); // Выделение памяти
        for (double x = xmin; count < n; x += dx) { // Генерация значений
            values[count].x = x; // Присвоение x
            values[count].y = func(x); // Вычисление y
            count++; // Увеличение счетчика
        }
        return count; // Возврат количества значений
    }

    // Обработка режима случайных значений
    int handle_random_values_mode(double xmin, double xmax) {
        int n; // Количество значений
        printf("Сколько значений? "); // Запрос на количество
        scanf("%d", &n); // Считывание количества
        values = (struct function*)malloc(n * sizeof(struct function)); // Выделение памяти
        count = 0; // Сброс счетчика
        while (count < n) { // Генерация случайных значений
            double x = xmin + 1.f * (xmax - xmin) * rand() / RAND_MAX; // Генерация случайного x
            values[count].x = x; // Присвоение x
            values[count].y = func(x); // Вычисление y
            count++; // Увеличение счетчика
        }
        return count; // Возврат количества значений
    }

    // Функция для вычисления факториала
    unsigned long long factorial(int n) {
        if (n == 0 || n == 1) return 1; // Базовый случай
        unsigned long long result = 1; // Инициализация результата
        for (int i = 2; i <= n; i++) { // Цикл для вычисления факториала
            result *= i; // Умножение
        }
        return result; // Возврат факториала
    }

    double F1(double x) {
        if (fabs(sin(x)) + pow(x, 2.0 / 5.0) > 0) // Проверка на положительность
            return log(fabs(sin(x)) + pow(x, 2.0 / 5.0)) / log(5); // Возврат логарифма
        else return NAN; // Возврат NaN в случае ошибки
    }

    double F2(double x) {
        if (x <= 1) {
            return (pow(x, 2) - 7 * x + 3) / exp(x); // Вычисление для x <= 1
        }
        else {
            return log10((pow(x, 2))); // Вычисление для x > 1
        }
    }

    double F3(double x) {
        if (x > 1) {
            double sum = 0.0; // Инициализация суммы
            for (int n = 1; n < MAX_T; n++) { // Цикл для вычисления суммы
                double term = ((double)factorial(2 * n) / ((pow(2, 2 * n) * pow(factorial(n), 2)))) * (pow(x, -2 * n) / (2 * n)); //расчёт 1 члена
                sum += term; // Добавление члена к сумме
                if (fabs(term) < precision) { // Проверка на точность
                    break; // Прерывание, если достигнута необходимая точность
                }
            }
            return log(2 * x) - sum; //  Вычисление функции
        }
        else return NAN; // Возврат NaN в случае ошибки
       }
