#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**Created by
 * Likhanov Maksim M3134
 */

size_t WORDMAXLENGTH = 20;
size_t TELEPHONLENGTH = 11;
size_t countOfNames;
size_t size;
int bySurname, byName, byPatronymic, byTelephone;

typedef struct
{
    char* surname;
    char* name;
    char* patronymic;
    char* telephone;
} Human;

Human* mas;

typedef struct
{
    char* first;
    char* second;
    char* third;
    char* fourth;
} ModeOfComparing;

ModeOfComparing* mode;

int comparatorByName(Human** first, Human** second)
{
    return strcmp((*first)->name, (*second)->name);
}

int comparatorBySurname(Human** first, Human** second)
{
    return strcmp((*first)->surname, (*second)->surname);
}

int comparatorByPatronymic(Human** first, Human** second)
{
    return strcmp((*first)->patronymic, (*second)->patronymic);
}

int comparatorByTelephone(Human** first, Human** second)
{
    return strcmp((*first)->telephone, (*second)->telephone);
}

void swapHumans(Human* px, Human* py)
{
    Human tmp = *px;
    *px = *py;
    *py = tmp;
}

void configure(int* number, char* word)
{
    if (strcmp(word,"surname") == 0)
    {
        *number = bySurname;
    }
    if (strcmp(word, "name") == 0)
    {
        *number = byName;
    }
    if (strcmp(word, "patronymic") == 0)
    {
        *number = byPatronymic;
    }
    if (strcmp(word, "telephone") == 0)
    {
        *number = byTelephone;
    }
}

int compareByMode(Human* left, Human* right)
{
    bySurname = comparatorBySurname(&left, &right);
    byName = comparatorByName(&left, &right);
    byPatronymic = comparatorByPatronymic(&left, &right);
    byTelephone = comparatorByTelephone(&left, &right);
    int first, second, third, fourth;
    configure(&first, mode->first);
    configure(&second, mode->second);
    configure(&third, mode->third);
    configure(&fourth, mode->fourth);
    if (first == 0)
    {
        if (second == 0)
        {
            if (third == 0)
            {
                return fourth;
            } else
            {
                return third;
            }
        } else
        {
            return second;
        }
    } else
    {
        return first;
    }
}

void qsortRecursive(int left, int right)
{
    int i = left;
    int j = right;
    Human mid = (mas)[(left + right) / 2];
    do
    {
        while (compareByMode(&(mas)[i], &mid) < 0)
        {
            i++;
        }
        while (compareByMode(&(mas)[j], &mid) > 0)
        {
            j--;
        }

        if (i <= j)
        {
            swapHumans(&((mas)[i]), &((mas)[j]));
            i++;
            j--;
        }
    } while (i <= j);

    if (left < j)
    {
        qsortRecursive(left, j);
    }
    if (i < right)
    {
        qsortRecursive(i, right);
    }
}

void initialize(char* fileName)
{
    char* fileIn = fileName;
    FILE* streamIn = fopen(fileIn, "r");
    if (!(mode = malloc(sizeof(ModeOfComparing))))
    {
        perror("Can't allocate memory");
        exit(errno);
    };

    /** Here you can configure sort order */
    mode->first = "surname";
    mode->second = "name";
    mode->third = "patronymic";
    mode->fourth = "telephone";

    if (streamIn == NULL)
    {
        perror("Can't open input file");
        exit(errno);
    }
    //assert(streamIn != NULL);
    size = 4;
    if (!(mas = calloc(size, sizeof(Human))))
    {
        perror("Can't allocate memory");
        exit(errno);
    };
    size_t i = 0;
    while (!feof(streamIn))
    {
        char* surname;
        char* name;
        char* patronymic;
        char* telephone;
        if(!(surname = calloc(WORDMAXLENGTH, sizeof(char))) || !(name = calloc(WORDMAXLENGTH, sizeof(char))) ||
            !(patronymic = calloc(WORDMAXLENGTH, sizeof(char))) || !(telephone = calloc(TELEPHONLENGTH, sizeof(char))))
        {
            perror("Can't allocate memory");
            exit(errno);
        }
        fscanf(streamIn, "%s", surname);
        fscanf(streamIn, "%s", name);
        fscanf(streamIn, "%s", patronymic);
        fscanf(streamIn, "%s", telephone);
        (*(mas + i)).surname = surname;
        (*(mas + i)).name = name;
        (*(mas + i)).patronymic = patronymic;
        (*(mas + i)).telephone = telephone;
        i++;
        if (i + 1 > size)
        {
            size *= 2;
            if (!(mas = realloc(mas, size * sizeof(Human))))
            {
                perror("Can't allocate memory");
                exit(errno);
            };
        }
    }
    countOfNames = i;
    fclose(streamIn);
}

int main(int argc, char** argv)
{
    if (argc >= 4)
    {
        initialize(argv[2]);
        printf("end of initialize...\n");
        qsortRecursive(0, countOfNames - 1);
        printf("end of qsortRecursive...\n");
        FILE* streamOut = fopen(argv[3], "w");
        if (streamOut == NULL)
        {
            perror("Can't open output file");
            exit(errno);
        }
        for (int i = 0; i < countOfNames; i++)
        {
            fprintf(streamOut, "%s %s %s %s\n", mas[i].surname, mas[i].name, mas[i].patronymic, mas[i].telephone);
        }
        fclose(streamOut);
        for (int i = 0; i < size; i++)
        {
            free(mas[i].surname);
            free(mas[i].name);
            free(mas[i].patronymic);
            free(mas[i].telephone);
        }
        free(mas);
        printf("well done!");
    } else
    {
        printf("You should enter three arguments to program:\nlab2 <input file name> <output file name>");
        exit(EXIT_FAILURE);
    }
    return 0;
}