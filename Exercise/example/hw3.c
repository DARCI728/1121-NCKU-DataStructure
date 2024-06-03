#include <stdio.h>
#include <string.h>

typedef struct {
  int row;
  int col;
  int value;
} term;

void fastTranspose(term a[], term b[]) {
    int numCols = a[0].col;
    int numTerms = a[0].value;

    b[0].row = numCols;
    b[0].col = a[0].row;
    b[0].value = numTerms;

    int starting[b[0].row+1];

    if (numTerms > 0) {
        memset(starting, 0, sizeof(int) * (b[0].row+1));

        for (int i = 1; i <= numTerms; i++) 
        starting[a[i].col]++;

        int acc = 1;
        for (int i = 1; i <= numTerms; i++) {
            int tmp = starting[i];
            starting[i] = acc;
            acc += tmp;
        }

        for (int i = 1; i <= numTerms; i++) {
            int j = starting[a[i].col];
            starting[a[i].col]++;
            b[j].row = a[i].col;
            b[j].col = a[i].row;
            b[j].value = a[i].value;
        }
    }
}


int main() {
    int R, C, T;
    scanf("%d %d %d", &R, &C, &T);
    term a[T+1];

    a[0].row = R;
    a[0].col = C;
    a[0].value = T;

    for (int i = 1; i <= T; i++) {
        int r, c, v;
        scanf("%d %d %d", &r, &c, &v);

        a[i].row = r;
        a[i].col = c;
        a[i].value = v;
    }

    term b[T+1];
    fastTranspose(a, b);

    for (int i = 0; i <= T; i++) {
        printf("%d %d %d\n", b[i].row, b[i].col, b[i].value);
    }

    return 0;
}