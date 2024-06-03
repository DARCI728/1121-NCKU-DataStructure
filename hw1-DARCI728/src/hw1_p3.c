#include <stdio.h>
#include <stdlib.h>

struct Edge {
    int s, t, weight;
};

struct Subset {
    int parent, height;
};

int Find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = Find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

void Union(struct Subset subsets[], int a, int b) {
    int rootA = Find(subsets, a);
    int rootB = Find(subsets, b);

    if (subsets[rootA].height > subsets[rootB].height) {
        subsets[rootB].parent = rootA;
    } else if (subsets[rootA].height < subsets[rootB].height) {
        subsets[rootA].parent = rootB;
    } else {
        subsets[rootB].parent = rootA;
        subsets[rootA].height++;
    }
}

int cmp(const void* a, const void* b) {
    return ((struct Edge*)a)->weight - ((struct Edge*)b)->weight;
}

void kruskal(struct Edge edges[], int V, int E) {
    struct Subset* subsets = (struct Subset*)malloc(sizeof(struct Subset) * V);

    for (int i = 0; i < V; i++) {
        subsets[i].parent = i;
        subsets[i].height = 0;
    }

    qsort(edges, E, sizeof(edges[0]), cmp);

    unsigned long long minCost = 0;

    for (int i = 0, e = 0; i < V - 1 && e < E; e++) {
        int x = Find(subsets, edges[e].s);
        int y = Find(subsets, edges[e].t);

        if (x != y) {
            minCost += edges[e].weight;
            Union(subsets, x, y);
            i++;
        }
    }

    printf("%llu\n", minCost);

    free(subsets);

    return;
}

int main(int argc, char* argv[]) {
    int V, E;
    scanf("%d %d", &V, &E);

    struct Edge* edges = (struct Edge*)malloc(sizeof(struct Edge) * E);

    for (int i = 0; i < E; i++) {
        scanf("%d %d %d", &edges[i].s, &edges[i].t, &edges[i].weight);
    }

    kruskal(edges, V, E);

    free(edges);

    return 0;
}