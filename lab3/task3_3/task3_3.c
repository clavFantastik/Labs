#include <stdio.h>
#define _GNU_SOURCE
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

#define EPS 1e-10

typedef enum Status {
    OK,
    INP_ERROR,
    FILE_ERROR,
    MEM_ERROR,
} status;

typedef struct Employee {
    unsigned int id;
    char name[64];
    char surname[64];
    double salary;
} Employee;



status inp_validate(int argc, char **argv) {
    if (argc != 4 || (argv[2][0] != '-' && argv[2][0] != '/') || (argv[2][1] != 'a' && argv[2][1] != 'd') || argv[2][2] != '\0') 
        return INP_ERROR;
    char path1[PATH_MAX], path2[PATH_MAX];
    if (!realpath(argv[1], path1) || !realpath(argv[3], path2)) {
        return INP_ERROR;
    }
    if (!strcmp(path1, path2)) {
        return INP_ERROR;
    }
    return OK;
}


status validate_data(Employee data) {
    int len = strlen(data.name), i;
    for (i = 0; i < len; ++i)
    {
        if (!((data.name[i] >= 'a' && data.name[i] <= 'z') || (data.name[i] >= 'A' && data.name[i] <= 'Z')))
            return INP_ERROR;
    }
    len = strlen(data.surname);
    for (i = 0; i < len; ++i)
    {
        if (!((data.surname[i] >= 'a' && data.surname[i] <= 'z') || (data.surname[i] >= 'A' && data.surname[i] <= 'Z')))
            return INP_ERROR;
    }
    if (data.salary < EPS)
    {
        return INP_ERROR;
    }
    return OK;
}

status read_from_file(FILE * input, Employee **out_data, int *data_size) {
    int capacity = 2, count_read;
    *data_size = 0;

    Employee *data = (Employee *)malloc(sizeof(Employee) * capacity);
    Employee *for_realloc;
    if(!data)
        return MEM_ERROR;

    while ((count_read = fscanf(input, "%u %s %s %lf\n", &data[*data_size].id, data[*data_size].name, data[*data_size].surname, &data[*data_size].salary)) == 4)
    {
        if (validate_data(data[(*data_size)])) {
            free(data);
            return INP_ERROR;
        }
        (*data_size)++;
        if (*data_size == capacity) {
            capacity *= 2;
            for_realloc = (Employee *)realloc(data, capacity * sizeof(Employee));
            if (!for_realloc) {
                free(data);
                return MEM_ERROR;
            }
            data = for_realloc;

        }
    }

    if (count_read < 4 && count_read > 0)
    {
        free(data);
        return INP_ERROR;
    }
    *out_data = data;
    return OK;
}

int compare_a(const void *a, const void *b) {
    const Employee *emp_a = (const Employee *)a;
    const Employee *emp_b = (const Employee *)b;

    if (emp_a->salary - emp_b->salary > EPS)
    {
        return 1;
    }
    if (emp_b->salary - emp_a->salary > EPS)
    {
        return -1;
    }

    int temp = strcmp(emp_a->surname, emp_b->surname);
    if (temp)
    {
        return temp;
    }
    temp = strcmp(emp_a->name, emp_b->name);
    if (temp)
    {
        return temp;
    }
    return emp_a->id - emp_b->id;
}

int compare_d(const void *a, const void *b)
{
    return compare_a(a, b) * -1;
}

int main(int argc, char **argv) {

    if (inp_validate(argc, argv) == INP_ERROR) {
        printf("Input error\n");
        return INP_ERROR;
    }

    Employee *data = NULL;

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Can't open input file\n");
        return 0;
    }

    FILE *output = fopen(argv[3], "w+"); 
    if(!output) {
        printf("Can't open input file\n");
        return 0;
    }

    int data_size;
    if (read_from_file(input, &data, &data_size) != OK) {
        printf("Error while reading file\n");
        free(data); 
        fclose(input);
        fclose(output);
        return 0;
    }

    switch (argv[2][1]) {
        case 'a':
            qsort(data, data_size, sizeof(Employee), compare_a);
            break;
        case 'd':
            qsort(data, data_size, sizeof(Employee), compare_d);
            break;
    }
    for(int i = 0; i < data_size; i++) {
        Employee max = data[i];
        fprintf(output, "%u %s %s %f\n", max.id, max.name, max.surname, max.salary);
    }
    
    free(data);
    fclose(input);
    fclose(output);
}