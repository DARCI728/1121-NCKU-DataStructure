#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 1000000

typedef struct Node Node;
struct Node {
    Node *left, *right;
    int key, s;
};
Node *newNode(int key) {
    Node *node = malloc(sizeof(Node));
    node->left = NULL;
    node->right = NULL;
    node->key = key;
    node->s = 1;
    return node;
}
void mirror(Node *a) {
    Node *tmp = a->left;
    a->left = a->right;
    a->right = tmp;
}
void minUnion(Node *a, Node *b) {
    if (a->key > b->key) {
        Node *t = a;
        a = b;
        b = t;
    }
    if (a->right == NULL) {
        a->right = b;
    } else {
        minUnion(a->right, b);
    }
    if (a->left == NULL) {
        mirror(a);
    } else if (a->left->key > a->right->key || (a->left->key == a->right->key && a->left->s < a->right->s)) {
        mirror(a);
    }
    if (a->right != NULL) {
        a->s = a->right->s + 1;
    }
}

int heap[MAX];
int pointer = 1;
void insert(int data) {
    heap[pointer] = data;
    int cur = pointer;
    while (heap[cur / 2] > heap[cur] && cur / 2 >= 1) {
        int t = heap[cur / 2];
        heap[cur / 2] = heap[cur];
        heap[cur] = t;
        cur /= 2;
    }
    pointer += 1;
}
void switchVal(int i, int j) {
    int tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = heap[i];
}
void extractMin() {
    heap[1] = heap[pointer - 1];
    pointer -= 1;
    int parent = 1;
    int left = parent * 2;
    int right = parent * 2 + 1;
    while (left < pointer) {
        if (left == pointer - 1) {
            if (heap[parent] > heap[left]) {
                switchVal(left, parent);
            }
            break;
        }
        if (heap[parent] > heap[left] || heap[parent] > heap[right]) {
            if (heap[left] <= heap[right]) {
                switchVal(left, parent);
                parent = left;
            } else {
                switchVal(right, parent);
                parent = right;
            }
            left = parent * 2;
            right = left + 1;
        } else {
            break;
        }
    }
}

int main() {
    int n, m = 5000;
    int init[m], operation[MAX], insertList[MAX];
    srand(time(NULL));
    scanf("%d", &n);
    for (int i = 0; i < m; i++) {
        init[i] = rand() % 30000;
    }
    for (int i = 0; i <= m; i++) {
        operation[i] = rand() % 2;
        insertList[i] = rand() % 30000;
    }

    clock_t start1, end1;
    start1 = clock();
    Node *root = newNode(insertList[0]);
    Node *sub = newNode(insertList[1]);
    for (int i = 2; i < m; i++) {
        sub = newNode(insertList[i]);
        minUnion(root, sub);
    }
    for (int i = 0; i <= n; i++) {
        if (operation[i] == 0) {  // insert
            Node *t = newNode(insertList[i]);
            minUnion(root, t);
        } else if (operation[i] == 1) {  // delete
            if (root != NULL) {
                if (root->left != NULL && root->right == NULL) {
                    root = root->left;
                } else if (root->left != NULL && root->right != NULL) {
                    Node *t1 = root->left;
                    Node *t2 = root->right;
                    minUnion(t1, t2);
                    root = t1;
                }
            }
        }
    }
    end1 = clock();
    double total1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    printf("time of leftist tree: %lf\n", total1);

    clock_t start2, end2;
    start2 = clock();
    pointer = 1;
    for (int i = 0; i < m; i++) {
        insert(init[i]);
    }
    for (int i = 0; i < n; i++) {
        if (operation[i] == 0) {
            insert(insertList[i]);
        } else if (operation[i] == 1) {
            if (pointer > 1) {
                extractMin();
            }
        }
    }
    end2 = clock();
    double total2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    printf("time of min heap: %lf\n", total2);
    return 0;
}