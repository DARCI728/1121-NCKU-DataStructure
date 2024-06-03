#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

int index = 0;

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *newNode() {
    Node *node = malloc(sizeof(Node));
    node->data = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void buildTree(Node *node, int arr[], int n) {
    if (index >= n)
        return;
    node->data = arr[index];
    index += 1;
    if (arr[index] == -1) {
        node->left = NULL;
    } else {
        node->left = newNode();
        buildTree(node->left, arr, n);
    }
    index += 1;
    if (arr[index] == -1) {
        node->right = NULL;
    } else {
        node->right = newNode();
        buildTree(node->right, arr, n);
    }
}

void swapTree(Node *node) {
    Node *t = node->left;
    node->left = node->right;
    node->right = t;
    if (node->left != NULL)
        swapTree(node->left);
    if (node->right != NULL)
        swapTree(node->right);
}

void preorder(Node *node) {
    printf("%d ", node->data);
    if (node->left != NULL)
        preorder(node->left);
    if (node->right != NULL)
        preorder(node->right);
}

void inorder(Node *node) {
    if (node->left != NULL)
        inorder(node->left);
    printf("%d ", node->data);
    if (node->right != NULL)
        inorder(node->right);
}

int main() {
    int n;
    scanf("%d", &n);
    int arr[MAX];
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    Node *root = newNode(arr[0]);
    buildTree(root, arr, n);
    swapTree(root);
    preorder(root);
    printf("\n");
    inorder(root);
    return 0;
}