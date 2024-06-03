#include <stdio.h>

int fibonacci_iterative(int n) {
  int arr[2] = {0, 1};
  for (int i = 2; i <= n; i++) {
    arr[i % 2] = arr[0] + arr[1];
  }
  return arr[n % 2];
}

int fibonacci_recursive(int n) {
  if (n == 0)
    return 0;
  else if (n == 1)
    return 1;
  return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

int main() {
    int n = 0;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        char s[10];
        int j;
        scanf("%s %d", s, &i);

        if (s[0] == 'r') {
            printf("%d\n", fibonacci_recursive(i));
        } else {
            printf("%d\n", fibonacci_iterative(i));
        }
    }
    return 0;
}