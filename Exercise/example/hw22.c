#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define repeat 100
#define max(a, b) ((a >= b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;
    }
}

int medianOfThree(int a, int b, int c) {
    if ((a > b) ^ (a > c))
        return a;
    else if ((b > a) ^ (b > c))
        return b;
    else
        return c;
}

void quickSort(int arr[], int start, int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;
        int a = arr[start], b = arr[mid], c = arr[end];
        arr[start] = min(a, min(b, c));
        arr[end] = max(a, max(b, c));
        arr[mid] = medianOfThree(a, b, c);
        if (end - start <= 1)
            arr[mid] = min(a, c);
        swap(&arr[mid], &arr[end - 1]);
        int pivot = end - 1;
        int i = start;
        int j = end - 1;
        while (1) {
            while (arr[++i] < arr[pivot]) {
            }
            while (j > start && arr[--j] > arr[pivot]) {
            }
            if (i < j)
                swap(&arr[i], &arr[j]);
            else
                break;
        }
        if (i < end)
            swap(&arr[i], &arr[end - 1]);
        quickSort(arr, start, i - 1);
        quickSort(arr, i + 1, end);
    }
}

void mergeSort(int arr[], int n) {
    int left_min, left_max, right_min, right_max;
    int tmp[n];
    for (int i = 1; i < n; i *= 2) {
        for (left_min = 0; left_min < n - i; left_min = right_max) {
            right_min = left_max = left_min + i;
            right_max = left_max + i;
            if (right_max > n)
                right_max = n;

            int j = 0;
            while (left_min < left_max && right_min < right_max) {
                if (arr[left_min] < arr[right_min])
                    tmp[j++] = arr[left_min++];
                else
                    tmp[j++] = arr[right_min++];
            }
            while (left_min < left_max)
                arr[--right_min] = arr[--left_max];
            while (j > 0)
                arr[--right_min] = tmp[--j];
        }
    }
}

void quickSort2(int a[], int first, int last) {
    int i, j, pivot, temp;
    if (first < last) {
        pivot = first;
        i = first;
        j = last;
        while (i < j) {
            while (a[i] <= a[pivot] && i < last)
                i++;
            while (a[j] > a[pivot])
                j--;
            if (i < j)
                swap(&a[i], &a[j]);
        }
        swap(&a[pivot], &a[j]);
        quickSort2(a, first, j - 1);
        quickSort2(a, j + 1, last);
    }
}

void maxheap_down(int arr[], int start, int end) {
    int cur = start;
    int left = 2 * cur + 1;
    int tmp = arr[cur];
    for (; left <= end; cur = left, left = 2 * left + 1) {
        if (left < end && arr[left] < arr[left + 1])
            left++;
        if (tmp >= arr[left])
            break;
        else {
            arr[cur] = arr[left];
            arr[left] = tmp;
        }
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        maxheap_down(arr, i, n - 1);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        maxheap_down(arr, 0, i - 1);
    }
}

void permute(int arr[], int n) {
    for (int i = n; i >= 1; i--) {
        int j = rand() % i;
        swap(&arr[i - 1], &arr[j]);
    }
}

void merge(int arr[], int left[], int right[], int l, int m, int r) {
    int i;
    for (i = 0; i <= m - l; i++)
        arr[i] = left[i];

    for (int j = 0; j < r - m; j++) {
        arr[i + j] = right[j];
    }
}

void seperate(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        int left[m - l + 1];
        int right[r - m];
        for (int i = 0; i <= m - l; i++)
            left[i] = arr[i * 2];
        for (int i = 0; i < r - m; i++)
            right[i] = arr[i * 2 + 1];

        seperate(left, l, m);
        seperate(right, m + 1, r);
        merge(arr, left, right, l, m, r);
    }
}

struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if (end.tv_nsec - start.tv_nsec < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

int main() {
    srand(time(NULL));
    int n;
    printf("n: ");
    scanf("%d", &n);
    int ans[5][n], tmp[n];
    struct timespec start, end;
    double total1 = 0, total2 = 0, total3 = 0, total4 = 0, total5 = 0;
    for (int i = 0; i < n; i++) {
        tmp[i] = rand() % 100000;
        for (int j = 0; j < 5; j++)
            ans[j][i] = tmp[i];
    }
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (ans[0][i] < ans[0][j])
                swap(&ans[0][i], &ans[0][j]);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (ans[4][i] > ans[4][j])
                swap(&ans[4][i], &ans[4][j]);

    double total = 0.0;
    int p3 = 0, l = 100, anstmp3[n];
    for (int p = 10; p <= 10 + l; p++) {
        permute(ans[3], n);
        int j = repeat, tmp3[n];
        double tmp = 0;
        while (j--) {
            for (int i = 0; i < n; i++)
                tmp3[i] = ans[3][i];
            clock_gettime(CLOCK_MONOTONIC, &start);
            heapSort(tmp3, n);
            clock_gettime(CLOCK_MONOTONIC, &end);
            struct timespec temp1 = diff(start, end);
            tmp += temp1.tv_sec + (double)temp1.tv_nsec / 1000000000.0;
        }
        tmp /= repeat;
        if (tmp > total) {
            total = tmp;
            p3 = p;
            for (int i = 0; i < n; i++)
                anstmp3[i] = ans[3][i];
        }
    }
    for (int i = 0; i < n; i++) {
        ans[3][i] = anstmp3[i];
    }

    total = 0.0;
    l = 100;
    int p1 = 0, anstmp1[n];
    for (int p = 10; p <= 10 + l; p++) {
        permute(ans[1], n);
        int j = repeat, tmp1[n];
        double tmp = 0;
        while (j--) {
            for (int i = 0; i < n; i++)
                tmp1[i] = ans[1][i];
            clock_gettime(CLOCK_MONOTONIC, &start);
            heapSort(tmp1, n);
            clock_gettime(CLOCK_MONOTONIC, &end);
            struct timespec temp1 = diff(start, end);
            tmp += temp1.tv_sec + (double)temp1.tv_nsec / 1000000000.0;
        }
        tmp /= repeat;
        if (tmp > total) {
            total = tmp;
            p1 = p;
            for (int i = 0; i < n; i++)
                anstmp1[i] = ans[1][i];
        }
    }
    for (int i = 0; i < n; i++) {
        ans[1][i] = anstmp1[i];
    }

    seperate(ans[2], 0, n - 1);
    for (int i = 1; i < n; i += 2) {
        swap(&ans[2][i], &ans[2][i - 1]);
    }

    int arr[5][n];
    int k = repeat;
    while (k--) {
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < n; i++) {
                arr[j][i] = ans[j][i];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &start);
        insertionSort(arr[0], n);
        clock_gettime(CLOCK_MONOTONIC, &end);
        struct timespec temp1 = diff(start, end);
        total1 += temp1.tv_sec + (double)temp1.tv_nsec / 1000000000.0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        quickSort(arr[1], 0, n - 1);
        clock_gettime(CLOCK_MONOTONIC, &end);
        struct timespec temp2 = diff(start, end);
        total2 += temp2.tv_sec + (double)temp2.tv_nsec / 1000000000.0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        mergeSort(arr[2], n);
        clock_gettime(CLOCK_MONOTONIC, &end);
        struct timespec temp3 = diff(start, end);
        total3 += temp3.tv_sec + (double)temp3.tv_nsec / 1000000000.0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        heapSort(arr[3], n);
        clock_gettime(CLOCK_MONOTONIC, &end);
        struct timespec temp4 = diff(start, end);
        total4 += temp4.tv_sec + (double)temp4.tv_nsec / 1000000000.0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        quickSort2(arr[4], 0, n - 1);
        clock_gettime(CLOCK_MONOTONIC, &end);
        struct timespec temp5 = diff(start, end);
        total5 += temp5.tv_sec + (double)temp5.tv_nsec / 1000000000.0;
    }

    printf("insertion sort: %lf\n", total1 / repeat);
    printf("quick sort: %lf\n", total5 / repeat);
    printf("quick sort(median of 3): %lf\n", total2 / repeat);
    printf("merge sort: %lf\n", total3 / repeat);
    printf("heap sort: %lf\n", total4 / repeat);

    return 0;
}
