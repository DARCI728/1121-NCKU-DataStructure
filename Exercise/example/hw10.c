#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Node Node;
struct Node {
    int value;
    bool leftThread, rightThread;
    Node *left, *right;
};

Node *newNode(int value) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->leftThread = true;
    node->rightThread = true;
    return node;
}

Node *find(Node *root, int value) {
    if (root->value == value)
        return root;
    if (!root->leftThread)
        return find(root->left, value);
    if (!root->rightThread)
        return find(root->right, value);
    else
        return find(root->right->right, value);
}

void insertLeft(Node *root, int parent, int child) {
    Node *cur = find(root, parent);
    if (cur->left == NULL) {
        cur->left = newNode(child);
        cur->leftThread = false;
        cur->left->right = cur;
    } else {
        Node *temp = cur->left;
        cur->left = newNode(child);
        cur->leftThread = false;
        cur->left->right = cur;
        cur->left->left = temp;
    }
}

void insertRight(Node *root, int parent, int child) {
    Node *cur = find(root, parent);
    if (cur->right == NULL) {
        cur->right = newNode(child);
        cur->rightThread = false;
        cur->right->left = cur;
    } else {
        Node *temp = cur->right;
        cur->right = newNode(child);
        cur->rightThread = false;
        cur->right->left = cur;
        cur->right->right = temp;
    }
}

void inorder(Node *node) {
    while (!node->leftThread)
        node = node->left;
    while (node->right != NULL) {
        printf("%d ", node->value);
        if (!node->rightThread) {
            node = node->right;
            while (!node->leftThread)
                node = node->left;
        } else {
            node = node->right;
        }
    }
    printf("%d ", node->value);
}

int main() {
    int n, r;
    scanf("%d %d", &n, &r);
    Node *root = newNode(r);
    while (n--) {
        int parent, child;
        char cmd[10] = {'\0'};
        scanf("%d %s %d", &parent, cmd, &child);
        if (!strcmp(cmd, "left"))
            insertLeft(root, parent, child);
        else if (!strcmp(cmd, "right"))
            insertRight(root, parent, child);
    }
    inorder(root);
    return 0;
}