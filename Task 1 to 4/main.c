PROGRAM 1 – B1: RECURSIVE POWER FUNCTIONS
============================================================

#include <stdio.h>

long long calls = 0;

long long power_linear(long long base, int exp)
{
    calls++;

    if (exp == 0)
        return 1;

    return base * power_linear(base, exp - 1);
}

long long power_fast(long long base, int exp)
{
    calls++;

    if (exp == 0)
        return 1;

    long long half = power_fast(base, exp / 2);

    if (exp % 2 == 0)
        return half * half;

    return base * half * half;
}

int main(void)
{
    calls = 0;
    long long a = power_linear(2, 30);
    printf("linear: %lld in %lld calls\n", a, calls);

    calls = 0;
    long long b = power_fast(2, 30);
    printf("fast : %lld in %lld calls\n", b, calls);

    return 0;
}


EXPECTED OUTPUT:

linear: 1073741824 in 31 calls
fast : 1073741824 in 6 calls


COMPILE AND RUN:

gcc B1_recursive_power.c -o B1
./B1


============================================================
PROGRAM 2 – B3: BINARY FILE STOCK UPDATE
============================================================

#include <stdio.h>

typedef struct {
    int group_code;
    int units;
} Stock;

/* Updates one stock record directly in the binary file. */
int update_units(FILE *fp, int index, int units)
{
    Stock s;

    if (fp == NULL || index < 0)
        return 0;

    if (fseek(fp, (long)index * sizeof(Stock), SEEK_SET) != 0)
        return 0;

    if (fread(&s, sizeof(Stock), 1, fp) != 1)
        return 0;

    s.units = units;

    if (fseek(fp, -(long)sizeof(Stock), SEEK_CUR) != 0)
        return 0;

    if (fwrite(&s, sizeof(Stock), 1, fp) != 1)
        return 0;

    return 1;
}

int main(void)
{
    FILE *fp = tmpfile();

    if (fp == NULL) {
        perror("tmpfile");
        return 1;
    }

    Stock initial[5] = {
        {1, 40},
        {2, 25},
        {3, 12},
        {4, 8},
        {5, 30}
    };

    if (fwrite(initial, sizeof(Stock), 5, fp) != 5) {
        perror("fwrite");
        fclose(fp);
        return 1;
    }

    printf("update record 3: %s\n",
        update_units(fp, 3, 20) ? "done" : "failed");

    printf("update record 9: %s\n",
        update_units(fp, 9, 20) ? "done" : "failed");

    rewind(fp);

    Stock s;

    while (fread(&s, sizeof s, 1, fp) == 1)
        printf("%d:%d ", s.group_code, s.units);

    printf("\n");

    fclose(fp);
    return 0;
}


EXPECTED OUTPUT:

update record 3: done
update record 9: failed
1:40 2:25 3:12 4:20 5:30


COMPILE AND RUN:

gcc B3_binary_stock_update.c -o B3
./B3


============================================================
PROGRAM 3 – B4: DYNAMIC MEMORY ALLOCATION
============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char group[4];
} Donor;

typedef struct {
    Donor *data;
    int size;
    int capacity;
} DonorList;

/* Adds a donor and doubles capacity whenever the list is full. */
int add_donor(DonorList *list, int id, const char *group)
{
    if (list == NULL || group == NULL)
        return 0;

    if (list->size == list->capacity) {
        int new_capacity =
            (list->capacity == 0) ? 2 : list->capacity * 2;

        Donor *p = realloc(
            list->data,
            (size_t)new_capacity * sizeof(Donor)
        );

        if (p == NULL)
            return 0;

        list->data = p;
        list->capacity = new_capacity;
    }

    list->data[list->size].id = id;

    strncpy(list->data[list->size].group, group, 3);
    list->data[list->size].group[3] = '\0';

    list->size++;

    return 1;
}

int main(void)
{
    DonorList list = {NULL, 0, 0};

    const char *groups[] = {
        "O+", "A+", "B-", "AB+", "O-"
    };

    for (int i = 0; i < 5; i++) {
        if (!add_donor(&list, 7001 + i, groups[i])) {
            printf("Memory allocation failed\n");
            free(list.data);
            return 1;
        }

        printf("added %d (%s): size = %d, capacity = %d\n",
            list.data[i].id,
            list.data[i].group,
            list.size,
            list.capacity);
    }

    free(list.data);
    return 0;
}


EXPECTED OUTPUT:

added 7001 (O+): size = 1, capacity = 2
added 7002 (A+): size = 2, capacity = 2
added 7003 (B-): size = 3, capacity = 4
added 7004 (AB+): size = 4, capacity = 4
added 7005 (O-): size = 5, capacity = 8


COMPILE AND RUN:

gcc B4_dynamic_donor_list.c -o B4
./B4


============================================================
END OF CO5 C PROGRAMS
