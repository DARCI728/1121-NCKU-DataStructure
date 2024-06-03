#include <stdio.h>
short int visited[100];
int mat[100][100], queue[100], n;

void bfs(int i) {
    int left = 0, right = 0;
    int cur;

    if (!visited[i]) {
        printf("%d ",i);
        visited[i] = 1;
        queue[right] = i;
        right++;
    }

    // Use left pointer to do pop action, and use right pointer to execute push function
    while (left < right) {
        cur = queue[left];
        for(int i = 0; i < n; i++) {
            if (mat[cur][i]) {
                if (!visited[i]) {
                    printf("%d ",i);
                    visited[i] = 1;
                    queue[right] = i;
                    right++;
                }
            }
        }
        left++;
    }
}

int main(){
    int round, start;
    scanf("%d",&round);

    while (round--) {
        scanf("%d %d", &n, &start);
        for (int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++){
                scanf("%d", &mat[i][j]);
            }
        }

        for (int i = 0; i < n; i++) {
            visited[i] = 0;
        }

        bfs(start);
        printf("\n");
    }
}