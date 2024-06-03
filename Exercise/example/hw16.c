#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void ShortestPath(long);
long node[1000001] = {0}, edge[1000001][3], n, root;

void ShortestPath(long x){
    long tmp = x;
    long prev_cost_sum = 0;

    while (tmp != 0) {
        prev_cost_sum=prev_cost_sum + edge[tmp-2][2];
        tmp=edge[tmp-2][0];
        node[tmp]=prev_cost_sum;
    }

    for(long i = 0; i < n; i++){
        long start = edge[i][0], end = edge[i][1], cost = edge[i][2];
        if (node[end] != 0) {
            continue;
        } else if (x == end) {
            node[x] = 0;
        } else {
            node[end] = node[start] + cost;
        }
    }
}

int main(){
    scanf("%ld", &n);
    for(long i=0; i < n-1; i++){
        scanf("%ld %ld %ld", &edge[i][0], &edge[i][1], &edge[i][2]);
    }

    scanf("%ld", &root);
    ShortestPath(root);

    for(long i=1; i <= n; i++) {
        printf("%ld %ld\n", i, node[i]);
    }
    return 0;
}