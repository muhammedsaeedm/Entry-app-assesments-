#include <stdio.h>
#include <stdbool.h>

int find_secnd_largest(const int ar[], int siz) {
    int max_val;
    int secnd_max;
    bool max_set = false;
    bool secnd_set = false;

    for (int i = 0; i < siz; i++) {
        if (!max_set || ar[i] > max_val) {
            if (max_set) {
                secnd_max = max_val;
                secnd_set = true;
            }
            max_val = ar[i];
            max_set = true;
        }
        else if (ar[i] < max_val) {
            if (!secnd_set || ar[i] > secnd_max) {
                secnd_max = ar[i];
                secnd_set = true;
            }
        }
    }

    return secnd_max;
}

int main(void) {
    int my_arr[] = {12, 35, 1, 10, 34, 1};
    int lengh1 = sizeof(my_arr) / sizeof(my_arr[0]);
    printf("Second largest: %d\n", find_secnd_largest(my_arr, lengh1));

    int test_arr2[] = {-10, -5, -20, -2};
    int lengh2 = sizeof(test_arr2) / sizeof(test_arr2[0]);
    printf("Second largest: %d\n", find_secnd_largest(test_arr2, lengh2));

    return 0;
}
