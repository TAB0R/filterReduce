#include <stdio.h>
#include <string.h>

void filter(
        void *from,
        void *to,
        size_t count,
        size_t size,
        int (*filter)(void *)
) {
    char *from_bytes = (char *) from;
    char *to_bytes = (char *) to;

    size_t count_to = 0;

    for (size_t i = 0; i < count; i++) {
        int res = filter(from_bytes + (size * i));
        if (res) {
            memcpy(to_bytes + (count_to * size), from_bytes + (size * i), size);
            count_to++;
        }
    }
}

void reduce(
        void *from,
        void *result,
        size_t count,
        size_t size,
        void (*func)(void *, void *)
) {
    char *from_bytes = (char *) from;
    char *result_bytes = (char *) result;

    memcpy(result_bytes, from_bytes, size);

    for (size_t i = 1; i < count; i++) {
        func(result_bytes, from_bytes + (size * i));
    }
}

void func_mul_int(void *a, void *b) {
    int *a_i = (int *) a;
    int *b_i = (int *) b;

    *a_i = *a_i * *b_i;
}

int filter_int(void *a) {
    int *a_i = (int *) a;

    if (*a_i > 0) {
        return 1;
    }

    return 0;
}


int main(void) {
    int a[] = {1, -2, 3, -5, 5};
    int b[] = {0, 0, 0, 0, 0};

    filter(a, b, 5, sizeof(int), filter_int);
    for (int i = 0; i < 5; i++) {
        printf("%d ", b[i]);
    }

    printf("\n");

    int c[] = {1, 2, 3, 4, 5};
    int result = 0;
    reduce(c, &result, 5, sizeof(int), func_mul_int);
    printf("%d\n", result);

    return 0;
}