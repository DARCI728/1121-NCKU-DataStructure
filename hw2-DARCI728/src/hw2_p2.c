#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    RED,
    BLACK
} Color;

typedef struct Node {
    int data;
    struct Node* parent;
    struct Node* left_child;
    struct Node* right_child;
    Color color;
} Node;

Node* root = NULL;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->parent = NULL;
    newNode->left_child = NULL;
    newNode->right_child = NULL;
    newNode->color = RED;
    return newNode;
}

void leftRotate(Node* x) {
    Node* y = x->right_child;
    x->right_child = y->left_child;

    if (y->left_child != NULL)
        y->left_child->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->left_child)
        x->parent->left_child = y;
    else
        x->parent->right_child = y;

    y->left_child = x;
    x->parent = y;
}

void rightRotate(Node* y) {
    Node* x = y->left_child;
    y->left_child = x->right_child;

    if (x->right_child != NULL)
        x->right_child->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        root = x;
    else if (y == y->parent->left_child)
        y->parent->left_child = x;
    else
        y->parent->right_child = x;

    x->right_child = y;
    y->parent = x;
}

Node* leftmost(Node* current) {
    while (current->left_child != NULL) {
        current = current->left_child;
    }
    return current;
}

Node* successor(Node* current) {
    if (current->right_child != NULL) {
        return leftmost(current->right_child);
    }

    Node* node = current->parent;

    while (node != NULL && current == node->right_child) {
        current = node;
        node = node->parent;
    }

    return node;
}

Node* search(int data) {
    Node* current = root;
    while (current != NULL && data != current->data) {
        if (data < current->data) {
            current = current->left_child;
        } else {
            current = current->right_child;
        }
    }
    return current;
}

void insertFixUp(Node* z) {
    while (z != NULL && z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left_child) {
            Node* y = z->parent->parent->right_child;

            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right_child) {
                    z = z->parent;
                    leftRotate(z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left_child;

            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left_child) {
                    z = z->parent;
                    rightRotate(z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }

    root->color = BLACK;
}

void insert(int data) {
    Node* z = createNode(data);
    Node* y = NULL;
    Node* x = root;

    while (x != NULL) {
        y = x;
        if (z->data < x->data)
            x = x->left_child;
        else
            x = x->right_child;
    }

    z->parent = y;

    if (y == NULL)
        root = z;
    else if (z->data < y->data)
        y->left_child = z;
    else
        y->right_child = z;

    insertFixUp(z);
}

void deleteFixUp(Node* x) {
    while (x != NULL && x != root && x->color == BLACK) {
        if (x == x->parent->left_child) {
            Node* sibling = (x->parent != NULL) ? x->parent->right_child : NULL;

            if (sibling != NULL) {
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    if (x->parent != NULL) x->parent->color = RED;
                    leftRotate(x->parent);
                    sibling = (x->parent != NULL) ? x->parent->right_child : NULL;
                }

                if (sibling != NULL) {
                    if ((sibling->left_child == NULL || sibling->left_child->color == BLACK) &&
                        (sibling->right_child == NULL || sibling->right_child->color == BLACK)) {
                        if (sibling != NULL) sibling->color = RED;
                        x = (x->parent != NULL) ? x->parent : NULL;
                    } else {
                        if (sibling != NULL && (sibling->right_child == NULL || sibling->right_child->color == BLACK)) {
                            if (sibling->left_child != NULL) sibling->left_child->color = BLACK;
                            if (sibling != NULL) sibling->color = RED;
                            rightRotate(sibling);
                            sibling = (x->parent != NULL) ? x->parent->right_child : NULL;
                        }

                        if (sibling != NULL) sibling->color = (x->parent != NULL) ? x->parent->color : BLACK;
                        if (x->parent != NULL) x->parent->color = BLACK;
                        if (sibling != NULL && sibling->right_child != NULL) sibling->right_child->color = BLACK;
                        leftRotate((x->parent != NULL) ? x->parent : sibling);
                        x = root;
                    }
                } else {
                    x = NULL;
                }
            } else {
                x = NULL;
            }
        } else {
            Node* sibling = (x->parent != NULL) ? x->parent->left_child : NULL;

            if (sibling != NULL) {
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    if (x->parent != NULL) x->parent->color = RED;
                    rightRotate(x->parent);
                    sibling = (x->parent != NULL) ? x->parent->left_child : NULL;
                }

                if (sibling != NULL) {
                    if ((sibling->right_child == NULL || sibling->right_child->color == BLACK) &&
                        (sibling->left_child == NULL || sibling->left_child->color == BLACK)) {
                        if (sibling != NULL) sibling->color = RED;
                        x = (x->parent != NULL) ? x->parent : NULL;
                    } else {
                        if (sibling != NULL && (sibling->left_child == NULL || sibling->left_child->color == BLACK)) {
                            if (sibling->right_child != NULL) sibling->right_child->color = BLACK;
                            if (sibling != NULL) sibling->color = RED;
                            leftRotate(sibling);
                            sibling = (x->parent != NULL) ? x->parent->left_child : NULL;
                        }

                        if (sibling != NULL) sibling->color = (x->parent != NULL) ? x->parent->color : BLACK;
                        if (x->parent != NULL) x->parent->color = BLACK;
                        if (sibling != NULL && sibling->left_child != NULL) sibling->left_child->color = BLACK;
                        rightRotate((x->parent != NULL) ? x->parent : sibling);
                        x = root;
                    }
                } else {
                    x = NULL;
                }
            } else {
                x = NULL;
            }
        }
    }

    if (x != NULL) x->color = BLACK;
}

void delete(int data) {
    Node* z = search(data);
    if (z == NULL) {
        return;
    }

    Node* y;
    Node* x;

    if (z->left_child == NULL || z->right_child == NULL)
        y = z;
    else
        y = successor(z);

    if (y->left_child != NULL)
        x = y->left_child;
    else
        x = y->right_child;

    if (x != NULL)
        x->parent = y->parent;

    if (y->parent == NULL)
        root = x;
    else if (y == y->parent->left_child)
        y->parent->left_child = x;
    else
        y->parent->right_child = x;

    if (y != z)
        z->data = y->data;

    if (y->color == BLACK)
        deleteFixUp(x);

    free(y);
}

int main() {
    while (true) {
        int data;
        char cmd[20];
        scanf("%s", cmd);

        if (!strcmp(cmd, "search")) {
            scanf("%d", &data);
            Node* node = search(data);
            if (node != NULL) {
                if (node->color == RED)
                    printf("red\n");
                else
                    printf("black\n");
            } else {
                printf("Not found\n");
            }
        } else if (!strcmp(cmd, "insert")) {
            scanf("%d", &data);
            insert(data);
        } else if (!strcmp(cmd, "delete")) {
            scanf("%d", &data);
            delete (data);
        } else if (!strcmp(cmd, "quit")) {
            break;
        }
    }

    return 0;
}