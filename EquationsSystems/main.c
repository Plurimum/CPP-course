#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**Created by
 * Likhanov Maksim M3134
 */

#define EPS 1e-16

size_t number;
int solutions;

double** readEquations(char* file)
{
    FILE* fin = fopen(file, "r");
    if (!fin)
    {
        perror("Can't open input file");
        exit(errno);
    }
    number = 0;
    if (fscanf(fin, "%d", &number) == EOF)
    {
        errno = 5;
        perror("File should contain size (integer decimal number) of system of equations on first line");
        exit(errno);
    }
    double** equations = malloc(sizeof(double*) * number);
    if (!equations)
    {
        perror("Can't allocate memory");
        exit(errno);
    }
    for (size_t i = 0; i < number; i++)
    {
        equations[i] = malloc(sizeof(double) * (number + 1));
        if (!equations[i])
        {
            perror("Can't allocate memory");
            exit(errno);
        }
    }
    for (size_t i = 0; i < number; i++)
    {
        for (size_t j = 0; j < number + 1; j++)
        {
            if (fscanf(fin, "%lf", &equations[i][j]) == EOF)
            {
                errno = 5;
                perror("The size of the equations shouldn't be less than the one specified in the first line");
                exit(errno);
            }
        }
    }
    fclose(fin);
    return equations;
}

double* gauss(double** equations, size_t n)
{
    double d;
    double* roots = malloc(sizeof(double) * n);
    double* freeCoefficients = malloc(sizeof(double) * n);
    for (size_t i = 0; i < n; i++)
    {
        freeCoefficients[i] = equations[i][n];
    }
    // direct traversal by Gauss
    for (size_t k = 0; k < n; k++)
    {
        for (size_t j = k + 1; j < n; j++)
        {
            // checking for a non-zero annihilating element
            if (fabs(equations[k][k]) <= EPS)
            {
                int found = 0;
                // finding a non-zero element on lower lines
                for (size_t line = k + 1; line < n; line++)
                {
                    if (equations[line][k] > EPS)
                    {
                        for (size_t elem = k; elem < n + 1; elem++)
                        {
                            double tmp = equations[k][elem];
                            equations[k][elem] = equations[line][elem];
                            equations[line][elem] = tmp;
                        }
                        double tmp = freeCoefficients[k];
                        freeCoefficients[k] = freeCoefficients[line];
                        freeCoefficients[line] = tmp;
                        found = 1;
                    }
                }
                if (found == 0)
                {
                    break;
                }
            }
            d = equations[j][k] / equations[k][k];
            for (size_t i = k; i < n; i++)
            {
                equations[j][i] = equations[j][i] - d * equations[k][i];
            }
            freeCoefficients[j] = freeCoefficients[j] - d * freeCoefficients[k];
            equations[j][n] = freeCoefficients[j];
        }
    }
    // reverse traversal by Gauss
    for (int k = n - 1; k >= 0; k--)
    {
        d = 0;
        for (size_t j = (size_t) (k + 1); j < n; j++)
        {
            double s = equations[k][j] * roots[j];
            d = d + s;
        }
        roots[k] = (freeCoefficients[k] - d) / equations[k][k];
    }
    // checking system for incompatibility
    for (size_t i = 0; i < n; i++)
    {
        if (fabs(freeCoefficients[i]) > EPS)
        {
            int trivialCoeffs = 1;
            for (size_t j = 0; j < n; j++)
            {
                if (fabs(equations[i][j]) > EPS)
                {
                    trivialCoeffs = 0;
                    break;
                }
            }
            if (trivialCoeffs == 1)
            {
                solutions = -1;
                return roots;
            }
        }
    }
    // checking system for indeterminacy
    for (size_t i = 0; i < n; i++)
    {
        if (fabs(freeCoefficients[i]) <= EPS)
        {
            if (fabs(equations[i][i]) <= EPS)
            {
                solutions = 1;
                return roots;
            }
        }
    }
    solutions = 0;
    return roots;
}

void printRoots(double* roots, char* file)
{
    FILE* fout = fopen(file, "w");
    if (!fout)
    {
        perror("Can't open output file");
        exit(errno);
    }
    if (solutions == 0)
    {
        for (size_t i = 0; i < number; i++)
        {
            fprintf(fout, "%.16f\n", roots[i]);
        }
    } else
    {
        fprintf(fout, (solutions == 1 ? "Many solutions" : "No solutions"));
    }
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        printf("You should enter three arguments to program:\nlab3 <input file name> <output file name>");
        exit(EXIT_FAILURE);
    }
    double** equations = readEquations(argv[2]);
    double* roots = gauss(equations, number);
    printRoots(roots, argv[3]);
    free(roots);
    for (size_t i = 0; i < number; i++)
    {
        free(equations[i]);
    }
    free(equations);
    return 0;
}