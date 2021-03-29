#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define FLT_EPSILON 1e-06


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Quantity of arguments is incorrect!\nargument 1: \\lab3\nargument 2: <name of input file>\nargument 3: <name of output file>");
        return 1;
    }

    if ((argv[1] == NULL) || (argv[2] == NULL)) {
        printf("Problem with name of files!");
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");

    if (fin == NULL) {
        printf("Can't open input file");
        return 1;
    }

    int n;
    fscanf(fin, "%d", &n);

    long double **a;
    if (!(a = (long double **) malloc(n * sizeof(long double *)))) {
        printf("Can't allocate memory!");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (!(a[i] = (long double *) malloc((n + 1) * sizeof(long double)))) {
            printf("Can't allocate memory!");
            for (int j = 0; j < i - 1; j++) {
                free(a[j]);
            }
            free(a);
            return 1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            fscanf(fin, "%Lf", &a[i][j]);
        }
    }

    fclose(fin);

    bool many_solutions = false, no_solutions = false;

    int *mas;
    if (!(mas = (int *) malloc(n * sizeof(int)))) {
        printf("Can't allocate memory!");
        for (int i = 0; i < n; i++) {
            free(a[i]);
        }
        free(a);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        mas[i] = -1;
    }

    for (int col = 0, row = 0; col < n && row < n; ++col) {
        int p = row;
        for (int i = row; i < n; ++i) {
            if (fabsl(a[i][col]) > fabsl(a[p][col])) {
                p = i;
            }
        }

        if (fabsl(a[p][col]) < FLT_EPSILON)
            continue;

        for (int i = col; i < n + 1; ++i) {
            long double tmp = a[p][i];
            a[p][i] = a[row][i];
            a[row][i] = tmp;
        }

        mas[col] = row;

        for (int i = 0; i < n; ++i)
            if (i != row) {
                long double p1 = a[i][col];
                long double p2 = a[row][col];
                long double c = p1 / p2;
                for (int j = col; j < n + 1; ++j)
                    a[i][j] -= c * a[row][j];
            }
        ++row;
    }

    long double *ans;

    if (!(ans = (long double *) malloc(n * sizeof(long double)))) {
        printf("Can't allocate memory!");
        for (int i = 0; i < n; i++) {
            free(a[i]);
        }
        free(a);
        free(mas);
        return 1;
    }
    for (int i = 0; i < n; i++)
        ans[i] = 0;

    for (int i = 0; i < n; ++i)
        if (mas[i] != -1) {
            long double p1 = a[mas[i]][n];
            long double p2 = a[mas[i]][i];
            ans[i] = p1 / p2;
        }

    for (int i = 0; i < n; ++i) {
        long double sum = 0;
        for (int j = 0; j < n; ++j) {
            sum += ans[j] * a[i][j];
        }
        long double sum2 = (long double) (sum - a[i][n]);
        long double d = fabsl(sum2);
        if (d > FLT_EPSILON) {
            no_solutions = true;
            break;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (mas[i] == -1) {
            many_solutions = true;
            break;
        }
    }
    free(mas);

    FILE *fout = fopen(argv[2], "w");

    if (fout == NULL) {
        printf("Can't open output file");
        for (int i = 0; i < n; i++)
            free(a[i]);
        free(a);
        free(ans);
        return 1;
    }

    if (no_solutions) {
        fprintf(fout, "no solutions");
    } else if (many_solutions) {
        fprintf(fout, "many solutions");
    } else {
        for (int i = 0; i < n; i++) {
            fprintf(fout, "%Lf\n", ans[i]);
        }
    }
    free(ans);
    for (int i = 0; i < n; i++) {
        free(a[i]);
    }
    free(a);
    fclose(fout);
    return 0;
}
