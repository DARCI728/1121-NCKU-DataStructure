#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Element {
    int parent;
    int height;
};

void MakeSet(struct Element* set, int n) {
    for (int i = 0; i < n; i++) {
        set[i].parent = i;
        set[i].height = 0;
    }
}

int Find(struct Element* set, int a) {
    if (set[a].parent != a) {
        set[a].parent = Find(set, set[a].parent);
    }
    return set[a].parent;
}

void Union(struct Element* set, int a, int b) {
    int rootA = Find(set, a);
    int rootB = Find(set, b);

    if (rootA != rootB) {
        if (set[rootA].height > set[rootB].height) {
            set[rootB].parent = rootA;
        } else if (set[rootA].height < set[rootB].height) {
            set[rootA].parent = rootB;
        } else {
            set[rootB].parent = rootA;
            set[rootA].height++;
        }
    }
}

int Same(struct Element* set, int a, int b) {
    return Find(set, a) == Find(set, b);
}

int main(int argc, char* argv[]) {
    int testcases, first = 1;
    scanf("%d", &testcases);

    while (testcases--) {
        int n, ops;
        scanf("%d %d", &n, &ops);

        struct Element* set = (struct Element*)malloc(sizeof(struct Element) * n);
        MakeSet(set, n);

        while (ops--) {
            int a, b;
            char operation[10];
            scanf("%s ", operation);

            if (!strcmp(operation, "union")) {
                scanf("%d %d", &a, &b);
                Union(set, a, b);

            } else if (!strcmp(operation, "find")) {
                scanf("%d", &a);

                if (first == 1) {
                    printf("%d", Find(set, a));
                    first = 0;
                } else {
                    printf("\n%d", Find(set, a));
                }

            } else if (!strcmp(operation, "same")) {
                scanf("%d %d", &a, &b);

                if (first == 1) {
                    printf("%s", Same(set, a, b) ? "true" : "false");
                    first = 0;
                } else {
                    printf("\n%s", Same(set, a, b) ? "true" : "false");
                }
            }
        }

        free(set);
    }

    return 0;
}