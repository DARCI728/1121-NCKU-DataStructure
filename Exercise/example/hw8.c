#include <stdio.h>
#include <stdlib.h>


typedef struct Node_T {
    int data;
    struct Node_T *left;
    struct Node_T *right;
} Node;

#define push(stack, top, num) (stack[top++] = num)
#define pop(stack, top) (stack[--top])
#define peak(stack, top) (stack[top-1])


void free_tree(Node *root) {
    if (!root) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int parse(int **expr, Node **out_root) {
    if (**expr == -1) {
        *out_root = NULL;
        (*expr)++;
        return 1;
    }

    Node *root = malloc(sizeof(Node));
    if (!root) {
        *out_root = NULL;
        return 0;
    }

    root->data = **expr;
    root->left = NULL;
    root->right = NULL;
    (*expr)++;
    Node *left;
    int result = parse(expr, &left);
    if (!result) {
        free(root);
        *out_root = NULL;
        return 0;
    }
    root->left = left;
    Node *right;
    result = parse(expr, &right);
    if (!result) {
        free_tree(root);
        *out_root = NULL;
        return 0;
    }
    root->right = right;
    *out_root = root;
    return 1;
}


void preorder(Node *root, int n) {
    Node *stack[1000];
    int stack_index = 0;
    int printed = 0;
    while (1) {
        if (root == NULL) {
            root = pop(stack, stack_index);
            continue;
        }
        push(stack, stack_index, root->right);
        if (printed == n - 1) {
            printf("%d", root->data);
            return;
        } else {
            printf("%d ", root->data);
        }
        printed++;
        root = root->left;
    }
}

void postorder(Node *root, int n) {
    Node *stack[1000];
    int stack_index = 0;
    int printed = 0;
    while (1) {
        while (root) {
            push(stack, stack_index, root->right);
            push(stack, stack_index, root);
            root = root->left;
        }
        right_tree:
        root = pop(stack, stack_index);
        if (stack_index > 0 && root->right == peak(stack, stack_index)) {
            Node *r = root;
            root = pop(stack, stack_index);
            push(stack, stack_index, r);
            continue;
        }
        if (printed == n - 1) {
            printf("%d", root->data);
            return;
        } else {
            printf("%d ", root->data);
            printed++;
            goto right_tree;
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);
    int expr[n];
    int non_null = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &expr[i]);
        if (expr[i] != -1) {
            non_null++;
        }
    }

    Node *root;
    int *expr_p = expr;
    if (parse(&expr_p, &root)) {
        preorder(root, non_null);
        printf("\n");
        postorder(root, non_null);
        free_tree(root);
    }
}
