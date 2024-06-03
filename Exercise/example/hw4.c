#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MOVE 50000

int inmove[] = {-1, 0, 1, 1, 1, 0, -1, -1};
int jmove[] = {1, 1, 1, 0, -1, -1, -1, 0};

int mLen, nLen, moveCnt = 0;

bool check(int **arr) {
    for (int i = 0; i < mLen; i++) {
        for (int j = 0; j < nLen; j++) {
            if (arr[i][j] == 0)
                return false;
        }
    }
    return true;
}

void move(int **arr, int x, int y) {
    int which_Direction = rand() % 8;
    if (moveCnt >= MAX_MOVE)
        return;
    if (x + inmove[which_Direction] > mLen - 1 || y + jmove[which_Direction] > nLen - 1
        || x + inmove[which_Direction] < 0 || y + jmove[which_Direction] < 0) {
        return move(arr, x, y);
    } else {
        x += inmove[which_Direction];
        y += jmove[which_Direction];
        arr[x][y]++;
        moveCnt++;
        if (moveCnt >= mLen * nLen && check(arr)) {

            return;
        }
        move(arr, x, y);
    }
}


int main() {
    srand(time(NULL));

    char command;
    command = getchar();

    while (command != 'q') {
        if (command == 'a') {
            scanf("%d %d", &mLen, &nLen);
            int **arr = (int **) malloc(mLen * sizeof(int *));
            // initialize
            for (int i = 0; i < mLen; i++) {
                *(arr + i) = calloc(nLen, sizeof(int));
            }
            move(arr, 1, 1);
            printf("%d\n", moveCnt);
            for (int i = 0; i < mLen; i++) {
                for (int j = 0; j < nLen; j++) {
                    printf("%3d ", arr[i][j]);
                }
                printf("\n");
            }
            moveCnt = 0;
        } else if (command == 'b') {
            int problem = 0;
            scanf("%d", &problem);
            if (problem == 1) {
                mLen = 15;
                nLen = 15;
                int **arr = (int **) malloc(mLen * sizeof(int *));
                // initialize
                for (int i = 0; i < mLen; i++) {
                    *(arr + i) = calloc(nLen, sizeof(int));
                }
                move(arr, 10, 10);
                printf("%d\n", moveCnt);
                for (int i = 0; i < mLen; i++) {
                    for (int j = 0; j < nLen; j++) {
                        printf("%3d ", arr[i][j]);
                    }
                    printf("\n");
                }
                moveCnt = 0;
            } else if (problem == 2) {
                mLen = 19;
                nLen = 39;
                int **arr = (int **) malloc(mLen * sizeof(int *));
                // initialize
                for (int i = 0; i < mLen; i++) {
                    *(arr + i) = calloc(nLen, sizeof(int));
                }
                move(arr, 1, 1);
                printf("%d\n", moveCnt);
                for (int i = 0; i < mLen; i++) {
                    for (int j = 0; j < nLen; j++) {
                        printf("%3d ", arr[i][j]);
                    }
                    printf("\n");
                }
                moveCnt = 0;
            }
        }
        getchar();
        command = getchar();
    }

    return 0;
}

