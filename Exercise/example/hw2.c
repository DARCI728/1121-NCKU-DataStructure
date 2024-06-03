#include <stdio.h>

void hanoi (int n, int from, int to, int temp) {
    if (n == 1) 
    printf("%d %d %d\n", n, from, to);

    else {
        hanoi(n-1, from, temp, to);

        printf("%d %d %d\n", n, from, to);

        hanoi(n-1, temp, to, from);
    }
}

int main() {
    int n;
    int Tower1 = 1;
    int Tower2 = 2;
    int Tower3 = 3;

    scanf("%d", &n);

    hanoi(n, Tower1, Tower3, Tower2);
}