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
        printf("������ �������� ����� ��� ������.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d, %lf\n", i + 1, values[i]);
    }
    fclose(file);
    printf("������ �������� � ���� %s\n", FILENAME);
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
        printf("\n����:\n");
        printf("1) ����� ������� �������� �� ����� � � ����\n");
        printf("2) ����������� ������������ � ������������� �� ����������� ��������\n");
        printf("3) �������� ����� �������� ��������� ��� �������� �������� �������\n");
        printf("4) ����������������� ������� � �������� �����\n");
        printf("5) ������� ������ � ���������� �������\n");
        printf("6) �������������� �������\n");
        printf("7) ���������� ������\n");
        printf("�������� �����: ");
        scanf("%d", &mode);

        switch (mode) {
        case 1:
            if (count == 0) {
                printf("��� ����������� ��������.\n");
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
                printf("��� ����������� ��������.\n");
            }
            else {
                double min, max;
                find_min_max(values, count, &min, &max);
                printf("����������� ��������: %lf\n", min);
                printf("������������ ��������: %lf\n", max);
            }
            break;
        case 3: {
            double target;
            printf("������� �������� ������� ��� ������: ");
            scanf("%lf", &target);
            printf("������� ��������: ");
            scanf("%lf", &precision);
            int index = linear_search(values, count, target, precision);
            if (index != -1) {
                printf("�������� ������� �� �������: %d\n", index);
            }
            else {
                printf("�������� �� �������.\n");
            }
            break;
        }
        case 4: {
            double x, h;
            printf("������� ����� ��� �����������������: ");
            scanf("%lf", &x);
            printf("������� ��� h: ");
            scanf("%lf", &h);
            double derivative = differentiate(F1, x, h);
            printf("����������� � ����� %lf: %lf\n", x, derivative);
            break;
        }
        case 5: {
            int choice;
            printf("�������� �����:\n");
            printf("1) �������� �������� (cvs-����)\n");
            printf("2) ��������������� �� xmin � ����� dx(n=10)\n");
            printf("3) ��������� �������� �� ��������� (xmin; xmax)\n");
            printf("�������� �����: ");
            scanf("%d", &choice);

            switch (choice) {
            case 1: {
                FILE* file = fopen("input.csv", "r");
                if (file == NULL) {
                    printf("������ �������� �����.\n");
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
                printf("������� xmin: ");
                scanf("%lf", &xmin);
                printf("������� ��� dx: ");
                scanf("%lf", &dx);
                n = 10;
                count = 0;
                for (double x = xmin; count < n; x += dx) {
                    values[count++] = F1(x);
                }
                break;
            }
            case 3: {
                printf("������� ��������? ");
                scanf("%d", &n);
                printf("������� xmin: ");
                scanf("%lf", &xmin);
                printf("������� xmax: ");
                scanf("%lf", &xmax);
                count = 0;
                while (count < n) {
                    double x = xmin + 1.f* (xmax - xmin)*rand()/RAND_MAX;
                        values[count++] = F1(x);
                }
                break;
            }
            default:
                printf("�������� �����. ���������� �����.\n");
            }
            break;
        }
        case 6:
            printf("� ��� ���� ���\n");
            break;
        case 7:
            printf("���������� ������.\n");
            return 0;
        default:
            printf("�������� �����. ���������� �����.\n");
        }
    }

    return 0;
}