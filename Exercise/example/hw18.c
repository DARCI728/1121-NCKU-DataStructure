#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SWAP(x, y, t) (*t = *x, *x = *y, *y = *t)

#define MAX_HEAP 6000

// Queue
int queue[MAX_HEAP];
int top = -1;
int rear = -1;

bool isEmpty();

bool isFull();

void push(int);

int pop();

// LeftistTree
typedef struct leftistTree {
    struct leftistTree *left;
    struct leftistTree *right;
    int value;
    int dis;
} leftistTree;

leftistTree **inLeft;
leftistTree *Right;

leftistTree *newNode(int);

leftistTree *root;


void insert(int);

void delete();

void check(leftistTree *);


// Binomial
typedef struct binomialTree {
    struct binomialTree *parent;
    struct binomialTree *rightSibling;
    struct binomialTree *child;
    int data;
    int degree;
} *Binomial;

Binomial stack[5000];
int stackIndex = -1;

Binomial newTree(int);

void insertTree(int);

void deleteMin();

void merge(int, int);


int main() {
    clock_t start_t, end_t;
    srand(time(0));

    for (int i = 0; i < 5000; i++) {
        int n = rand() % 5000 + 1;
        push(n);
    }

    //    leftistTree
    top = -1;
    start_t = clock();
    root = (leftistTree *) malloc(sizeof(leftistTree));
    root->value = -1;
    root->left = NULL;
    root->right = NULL;
    root->dis = 0;
    for (int i = 0; i < 5000; i++) {
        int n = rand() % 6 + 1;
        if (n < 4)
            if (root->value == -1)
                root->value = n;
            else
                insert(pop());
        else {
            if (root->left && root->right)
                delete(root);
            else if (root->left && !root->right)
                root = root->left;
            else
                root->value = -1;
        }
    }
    end_t = clock();
    printf("LEFTIST (seconds): %f\n", (double) (end_t - start_t) / CLOCKS_PER_SEC);

    // b-heap
    start_t = clock();
    for (int i = 0; i < 5000; i++) {
        int n = rand() % 6 + 1;
        if (n < 4)
            insertTree(pop());
        else {
            if (stackIndex == 0)
                continue;
            else
                deleteMin();
        }
    }
    end_t = clock();
    printf("B-HEAP (seconds): %f\n", (double) (end_t - start_t) / CLOCKS_PER_SEC);
    return 0;
}

// Queue

bool isEmpty() {
    if (top == rear)
        return true;
    return false;
}

bool isFull() {
    if (rear == MAX_HEAP - 1)
        return true;
    return false;
}

void push(int data) {
    if (isFull())
        return;
    queue[++rear] = data;
}

int pop() {
    if (isEmpty())
        return NAN;
    return queue[++top];
}

// LeftistTree

leftistTree *newNode(int data) {
    leftistTree *node = (leftistTree *) malloc(sizeof(leftistTree));

    node->left = NULL;
    node->right = NULL;
    node->value = data;
    node->dis = 0;
    return node;
}

void insert(int data) {

    leftistTree *node = newNode(data);
    // compare root->value
    if (root->value > node->value) {
        node->left = root;
        node->dis = root->dis;
        root = node;
    } else {
        leftistTree *temp = root;
        while (temp->right && node->value >= temp->value)
            temp = temp->right;

        if (node->value < temp->value) {
            leftistTree *T = (leftistTree *) malloc(sizeof(leftistTree));
            SWAP(node, temp, T);
            temp->dis = node->dis;
            temp->left = node;
        } else if (temp->left == NULL)
            temp->left = node;
        else if (temp->right == NULL) {
            temp->right = node;
            temp->dis = 1;
            check(root);
        }

    }


}

void delete() {
    int delete_top = -1;
    inLeft = (leftistTree **) calloc(5000, sizeof(leftistTree *));
    if (root->left->value > root->right->value) {
        leftistTree *T = (leftistTree *) malloc(sizeof(leftistTree));
        SWAP(root->left, root->right, T);
    }
    inLeft[++delete_top] = root->left;
    Right = root->right;
    for (;;) {
        if (inLeft[delete_top]->right != NULL) {
            leftistTree *nextLeft = inLeft[delete_top]->right;
            if (Right->value < nextLeft->value) {
                leftistTree *T = (leftistTree *) malloc(sizeof(leftistTree));
                SWAP(nextLeft, Right, T);
            }
            inLeft[++delete_top] = nextLeft;
        } else {
            if (inLeft[delete_top]->left == NULL)
                inLeft[delete_top]->left = Right;
            else if (inLeft[delete_top]->right == NULL)
                inLeft[delete_top]->right = Right;

            // check again
            if (delete_top != 0)
                check(inLeft[0]);
            break;
        }
    }
    root = inLeft[0];
}

void check(leftistTree *checking_root) {
    if (checking_root->right != NULL && checking_root->right->right != NULL)
        check(checking_root->right);
    if (checking_root->left->dis < checking_root->right->dis) {
        leftistTree *T = (leftistTree *) malloc(sizeof(leftistTree));
        SWAP(checking_root->left, checking_root->right, T);
    }
    checking_root->dis = checking_root->right->dis + 1;

}


// Binomial
Binomial newTree(int data) {
    Binomial temp = (Binomial) malloc(sizeof(struct binomialTree));
    temp->child = NULL;
    temp->parent = NULL;
    temp->rightSibling = NULL;
    temp->data = data;
    temp->degree = 1;
    return temp;
}

void insertTree(int data) {
    Binomial node = newTree(data);
    stack[++stackIndex] = node;
    int index1 = 0, index2 = 1;
    while (index2 <= stackIndex) {
        if (stack[index1]->degree == stack[index2]->degree) {
            merge(index1, index2);
            index1 = 0;
            index2 = 1;
            continue;
        }
        index1++;
        index2++;
    }
}

void deleteMin() {
    Binomial Min = newTree(9999);
    int toDelete = 0;
    for (int i = 0; i <= stackIndex; i++) {
        if (Min->data > stack[i]->data) {
            Min = stack[i];
            toDelete = i;
        }
    }
    int count = 0, level = Min->degree;
    while (level != 1) {
        count++;
        level /= 2;
    }
    Binomial temp = Min->child;
    Min = NULL;
    for (int i = toDelete + 1; i <= stackIndex; i++)
        stack[i - 1] = stack[i];
    stack[stackIndex--] = NULL;
    if (count == 0)
        return;
    else {
        for (int i = 1; i <= count, temp != NULL; i++) {
            temp->parent = NULL;
            stack[stackIndex + i] = temp;
            temp = temp->rightSibling;
        }
        stackIndex += count;
        int index1 = 0, index2 = 0;
        while (index1 <= stackIndex) {
            while (index2 <= stackIndex) {
                if (index1 != index2 && stack[index1]->degree == stack[index2]->degree) {
                    merge(index1, index2);
                    index1 = 0;
                    index2 = 0;
                    continue;
                }
                index2++;
            }
            index1++;
            index2 = 0;
        }
    }
}

void merge(int index1, int index2) {
    if (stack[index1]->data > stack[index2]->data) {
        Binomial T = (Binomial) malloc(sizeof(struct binomialTree));
        SWAP(stack[index1], stack[index2], T);
    }
    if (stack[index1]->degree > 1 && stack[index2]->degree > 1)
        stack[index2]->rightSibling = stack[index1]->child;
    stack[index1]->child = stack[index2];
    stack[index2]->parent = stack[index1];
    stack[index1]->degree += stack[index2]->degree;
    for (int i = index2 + 1; i <= stackIndex; i++)
        stack[i - 1] = stack[i];
    stack[stackIndex--] = NULL;
}

