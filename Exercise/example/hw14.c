#include <stdio.h>

short int visited[100];
int mat[100][100], n;

void dfs(int v){
    printf("%d ",v);
    visited[v]= 1;
    for(int i = 0; i < n; i++){
        if(mat[v][i]){
            if(!visited[i]) dfs(i);
        }
    }
}
int main(){
    int round, t;
    scanf("%d",&round);

    while(round--) {
        scanf("%d %d" ,&n ,&t);
        for(int i = 0; i < n; i++) {
            for(int j = 0 ;j < n ;j++) {
                scanf("%d" ,&mat[i][j]);
            }
        }

        for(int i = 0 ;i < n ;i++) {
            visited[i] = 0;
        }
        dfs(t);
    }
}