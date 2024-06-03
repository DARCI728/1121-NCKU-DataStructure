#include <stdlib.h>
#include <stdio.h>

typedef struct Node_T {
    char data;
    struct Node_T *children;
    struct Node_T *sibling;
} Node;

Node *trees[100] = {0};

void free_tree(Node *node) {
    Node *child = node->children;
    free(node);
    while (child) {
        Node *tmp = child;
        child = child->sibling;
        free_tree(tmp);
    }
}

Node *create(char **tree_expr) {
    while (**tree_expr < 'A' || **tree_expr > 'Z') {
        (*tree_expr)++;
    }
    Node *node = malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->sibling = NULL;
    node->data = **tree_expr;
    node->children = NULL;
    if (*((*tree_expr) + 1) == ',' || *((*tree_expr) + 1) == ')') {
        (*tree_expr)++;
        return node;
    }
    (*tree_expr)++;
    while (**tree_expr != ')') {
        Node *child = create(tree_expr);
        if (!child) {
            free_tree(node);
            return NULL;
        }
        child->sibling = NULL;
        Node **to_cur = &(node->children);
        Node *cur = node->children;
        while (cur && cur->data < child->data) {
            to_cur = &(cur->sibling);
            cur = cur->sibling;
        }
        *to_cur = child;
        child->sibling = cur;
    }
    (*tree_expr)++;
    return node;
}

Node *copy(Node *node) {
    Node *new = malloc(sizeof(Node));
    if (!new) {
        return NULL;
    }
    new->data = node->data;
    new->children = NULL;
    new->sibling = NULL;
    if (!(node->children)) {
        return new;
    }
    Node *child = node->children;
    Node **child_pos = &(new->children);
    while (child) {
        Node *copied = copy(child);
        if (!copied) {
            free_tree(new);
            return NULL;
        }
        *child_pos = copied;
        child_pos = &(copied->sibling);
        child = child->sibling;
    }

    return new;
}

int equal(Node *n1, Node *n2) {
    if (n1->data != n2->data) {
        return 0;
    }
    Node *n1_child = n1->children;
    Node *n2_child = n2->children;
    while (n1_child && n2_child) {
        if (!equal(n1_child, n2_child)) {
            return 0;
        }
        n1_child = n1_child->sibling;
        n2_child = n2_child->sibling;
    }

    return !n1_child && !n2_child;
}

void print_tree(Node *node) {
    printf("%c", node->data);
    if (!(node->children)) {
        return;
    }
    Node *child = node->children;
    printf("(");
    while (child) {
        print_tree(child);
        if (child->sibling) {
            printf(",");
        }
        child = child->sibling;
    }
    printf(")");
}


int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        char cmd[8] = {0};
        scanf("%s", cmd);
        switch (cmd[1]) {
            case 'e': {
                int id;
                scanf("%d", &id);
                char expr[100];
                scanf("%s", expr);
                char *exp_p = expr;
                Node *h = create(&exp_p);
                if (h) {
                    trees[id] = h;
                    printf("readed");
                    if (i != n - 1) {
                        printf("\n");
                    }
                }
            }
                break;
            case 'o': {
                int a, b;
                scanf("%d %d", &a, &b);
                Node *h = copy(trees[a]);
                if (h) {
                    trees[b] = h;
                    printf("copied");
                    if (i != n - 1) {
                        printf("\n");
                    }
                }
            }
                break;
            case 's': {
                int a, b;
                scanf("%d %d", &a, &b);
                if (equal(trees[a], trees[b])) {
                    printf("true");
                } else {
                    printf("false");
                }
                if (i != n - 1) {
                    printf("\n");
                }
            }
                break;
            case 'l': {
                int id;
                scanf("%d", &id);
                free_tree(trees[id]);
                trees[id] = NULL;
                printf("cleared");
                if (i != n - 1) {
                    printf("\n");
                }
            }
                break;
            case 'r': {
                int id;
                scanf("%d", &id);
                printf("(");
                print_tree(trees[id]);
                printf(")");
                if (i != n - 1) {
                    printf("\n");
                }
            }
                break;
        }
    }
}
