#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;
    int value;
    int degree;
    struct Node* child;
    struct Node* parent;
    struct Node* left_sibling;
    struct Node* right_sibling;
    bool childCut;
    bool visited;
} Node;

typedef struct FHeap {
    Node* min;
    int num_nodes;
} FHeap;

Node* CreateNode(int key, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->degree = 0;
    node->child = NULL;
    node->parent = NULL;
    node->left_sibling = node;
    node->right_sibling = node;
    node->childCut = false;
    node->visited = false;
    return node;
}

FHeap* CreateFHeap() {
    FHeap* fheap = (FHeap*)malloc(sizeof(FHeap));
    fheap->min = NULL;
    fheap->num_nodes = 0;
    return fheap;
}

void Insert(FHeap* fheap, int x, int val) {
    Node* node = CreateNode(x, val);

    if (fheap->min == NULL) {
        fheap->min = node;
    } else {
        node->left_sibling = fheap->min;
        node->right_sibling = fheap->min->right_sibling;
        fheap->min->right_sibling = node;
        node->right_sibling->left_sibling = node;

        if (node->key < fheap->min->key) {
            fheap->min = node;
        }
    }

    fheap->num_nodes++;
}

void Link(FHeap* fheap, Node* y, Node* x) {
    y->left_sibling->right_sibling = y->right_sibling;
    y->right_sibling->left_sibling = y->left_sibling;

    if (x->right_sibling == x) {
        fheap->min = x;
    }

    y->left_sibling = y;
    y->right_sibling = y;
    y->parent = x;

    if (x->child == NULL) {
        x->child = y;
    }

    y->left_sibling = x->child;
    y->right_sibling = x->child->right_sibling;
    x->child->right_sibling = y;
    y->right_sibling->left_sibling = y;

    if (y->key < x->child->key) {
        x->child = y;
    }

    x->degree++;
}

void Consolidate(FHeap* fheap) {
    int num_nodes = fheap->num_nodes;
    int maxDegree = 0;

    while (num_nodes > 0) {
        maxDegree++;
        num_nodes /= 2;
    }

    Node *degreeArray[maxDegree], *x = fheap->min;
    for (int i = 0; i <= maxDegree; i++) {
        degreeArray[i] = NULL;
    }

    do {
        int degree = x->degree;

        while (degreeArray[degree] != NULL) {
            Node* y = degreeArray[degree];

            if (x->key > y->key) {
                Node* temp = x;
                x = y;
                y = temp;
            }

            if (y == fheap->min) {
                fheap->min = x;
            }

            Link(fheap, y, x);

            if (y->right_sibling == x) {
                fheap->min = x;
            }

            degreeArray[degree] = NULL;
            degree++;
        }

        degreeArray[degree] = x;
        x = x->right_sibling;
    } while (x != fheap->min);

    fheap->min = NULL;

    for (int i = 0; i < maxDegree; i++) {
        if (degreeArray[i] != NULL) {
            degreeArray[i]->left_sibling = degreeArray[i];
            degreeArray[i]->right_sibling = degreeArray[i];

            if (fheap->min == NULL) {
                fheap->min = degreeArray[i];
            } else {
                degreeArray[i]->left_sibling = fheap->min;
                degreeArray[i]->right_sibling = fheap->min->right_sibling;
                fheap->min->right_sibling = degreeArray[i];
                degreeArray[i]->right_sibling->left_sibling = degreeArray[i];

                if (degreeArray[i]->key < fheap->min->key) {
                    fheap->min = degreeArray[i];
                }
            }

            if (fheap->min == NULL) {
                fheap->min = degreeArray[i];
            } else if (degreeArray[i]->key < fheap->min->key) {
                fheap->min = degreeArray[i];
            }
        }
    }
}

Node* ExtractMin(FHeap* fheap) {
    if (fheap->min == NULL) {
        return fheap->min;
    }

    Node* minNode = fheap->min;
    Node* nextChild = minNode;
    Node* child;

    if (minNode->child != NULL) {
        child = minNode->child;
        do {
            nextChild = child->right_sibling;
            fheap->min->left_sibling->right_sibling = child;
            child->right_sibling = fheap->min;
            child->left_sibling = fheap->min->left_sibling;
            fheap->min->left_sibling = child;
            if (child->key < fheap->min->key) {
                fheap->min = child;
            }
            child->parent = NULL;
            child = nextChild;
        } while (nextChild != minNode->child);
    }

    minNode->left_sibling->right_sibling = minNode->right_sibling;
    minNode->right_sibling->left_sibling = minNode->left_sibling;
    fheap->min = minNode->right_sibling;

    if (minNode == minNode->right_sibling && minNode->child == NULL) {
        fheap->min = NULL;
    } else {
        fheap->min = minNode->right_sibling;
        Consolidate(fheap);
    }

    fheap->num_nodes--;

    return minNode;
}

void Cut(FHeap* fheap, Node* x, Node* y) {
    if (x == x->right_sibling) {
        y->child = NULL;
    }

    x->left_sibling->right_sibling = x->right_sibling;
    x->right_sibling->left_sibling = x->left_sibling;

    if (x == y->child) {
        y->child = x->right_sibling;
    }

    y->degree--;

    x->left_sibling = x;
    x->right_sibling = x;

    x->left_sibling = fheap->min;
    x->right_sibling = fheap->min->right_sibling;
    fheap->min->right_sibling = x;
    x->right_sibling->left_sibling = x;

    x->parent = NULL;
    x->childCut = false;
}

void CascadingCut(FHeap* fheap, Node* y) {
    Node* z = y->parent;

    if (z != NULL) {
        if (y->childCut == false) {
            y->childCut = true;
        } else {
            Cut(fheap, y, z);
            CascadingCut(fheap, z);
        }
    }
}

void DecreaseKey(FHeap* fheap, Node* nodeToDecrease, int y) {
    if (nodeToDecrease != NULL) {
        nodeToDecrease->key -= y;

        Node* parent = nodeToDecrease->parent;

        if (parent != NULL && nodeToDecrease->key < parent->key) {
            Cut(fheap, nodeToDecrease, parent);
            CascadingCut(fheap, parent);
        }

        if (nodeToDecrease->key < fheap->min->key) {
            fheap->min = nodeToDecrease;
        }
    }
}

void FindNode(FHeap* fheap, Node* node, int x, int val, int y) {
    node->visited = true;

    if (node->key == x && node->value == val) {
        node->visited = false;
        Node* find = node;
        DecreaseKey(fheap, find, y);
    }

    if (node->child != NULL) {
        FindNode(fheap, node->child, x, val, y);
    }

    if (node->right_sibling != NULL && !node->right_sibling->visited) {
        FindNode(fheap, node->right_sibling, x, val, y);
    }

    node->visited = false;
}

void DeleteNode(FHeap* fheap, int x, int val) {
    if (fheap->min != NULL) {
        FindNode(fheap, fheap->min, x, val, 100000);
        Node* node = ExtractMin(fheap);
    }
}

void Print_heap(Node* n) {
    Node* x;
    for (x = n;; x = x->right_sibling) {
        if (x->child == NULL) {
            printf("node with no child (%d) \n", x->key);
        } else {
            printf("NODE(%d) with child (%d)\n", x->key, x->child->key);
            Print_heap(x->child);
        }
        if (x->right_sibling == n) {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    FHeap* fheap = CreateFHeap();
    int x, y, val;

    while (1) {
        char cmd[20];
        scanf("%s", cmd);

        if (!strcmp(cmd, "insert")) {
            scanf("%d %d", &x, &val);
            Insert(fheap, x, val);
        } else if (!strcmp(cmd, "extract")) {
            Node* minNode = ExtractMin(fheap);
            if (minNode != NULL) {
                printf("(%d)%d\n", minNode->key, minNode->value);
            }
        } else if (!strcmp(cmd, "delete")) {
            scanf("%d %d", &x, &val);
            DeleteNode(fheap, x, val);
        } else if (!strcmp(cmd, "decrease")) {
            scanf("%d %d %d", &x, &val, &y);
            FindNode(fheap, fheap->min, x, val, y);
        } else if (!strcmp(cmd, "quit")) {
            break;
        }
    }

    return 0;
}
