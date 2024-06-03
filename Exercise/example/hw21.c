#include <stdlib.h>
#include <stdio.h>


typedef struct node {
    int value;

    int smallest_value;
    int greatest_value;
    union {
        struct node *way_less;
        struct node *previous;
    };
    union {
        struct node *way_mid;
        struct node *next;
    };

    struct node *way_greater;
    struct node *parent;

    int contain_num;
} b_plus_node;

typedef struct {
    b_plus_node *root;
} b_plus_tree;


b_plus_node *new_index_node(int smaller_index, b_plus_node *less, b_plus_node *mid) {
    b_plus_node *new = malloc(sizeof(b_plus_node));
    if (!new) return NULL;
    new->value = 0;
    new->smallest_value = smaller_index;
    new->greatest_value = -1;
    new->way_less = less;
    if (less) {
        less->parent = new;
    }
    new->way_mid = mid;
    if (mid) {
        mid->parent = new;
    }
    new->way_greater = NULL;

    new->parent = NULL;

    new->contain_num = 1;
    return new;
}

b_plus_node *new_empty_data_node() {
    b_plus_node *new = malloc(sizeof(b_plus_node));
    if (!new) return NULL;
    new->value = 1;
    new->smallest_value = -1;
    new->greatest_value = -1;
    new->previous = NULL;
    new->next = NULL;
    new->way_greater = NULL;

    new->parent = NULL;

    new->contain_num = 0;
    return new;
}

b_plus_node *new_data_node(int data) {
    b_plus_node *new = new_empty_data_node();
    new->smallest_value = data;
    new->contain_num = 1;
    return new;
}

b_plus_node **find_way(b_plus_node *node, int search_value) {
    if (search_value <= node->smallest_value) {
        return &node->way_less;
    }
    if (node->greatest_value == -1) {
        return &node->way_mid;
    }
    if (search_value <= node->greatest_value) {
        return &node->way_mid;
    }
    return &node->way_greater;
}

int has_space(b_plus_node *node) {
    return node->contain_num < 2;
}

int is_leaf(b_plus_node *node) {
    return node->value;
}

void b_node_insert(b_plus_node *node, int value) {

    if (node->smallest_value == -1) {
        node->smallest_value = value;
        node->contain_num = 1;
    } else if (value > node->smallest_value) {
        node->greatest_value = value;
        node->contain_num = 2;
    } else {
        node->greatest_value = node->smallest_value;
        node->smallest_value = value;
        node->contain_num = 2;
    }
}

b_plus_node *data_split(b_plus_node *node, int data, int *head) {
    b_plus_node *splitted;
    if (data < node->smallest_value) {
        *head = node->smallest_value;
        splitted = new_data_node(node->smallest_value);
        splitted->greatest_value = node->greatest_value;
        splitted->contain_num = 2;

        node->greatest_value = -1;
        node->contain_num = 1;
        node->smallest_value = data;
    } else if (data > node->greatest_value) {
        *head = node->greatest_value;
        splitted = new_data_node(node->greatest_value);
        splitted->greatest_value = data;
        splitted->contain_num = 2;

        node->greatest_value = -1;
        node->contain_num = 1;
    } else {
        *head = data;
        splitted = new_data_node(data);
        splitted->greatest_value = node->greatest_value;
        splitted->contain_num = 2;

        node->greatest_value = -1;
        node->contain_num = 1;
    }

    b_plus_node *next = node->next;
    node->next = splitted;
    splitted->previous = node;
    splitted->next = next;
    if (next) {
        next->previous = splitted;
    }

    return splitted;
}

b_plus_node *index_split(b_plus_node *node, int value, b_plus_node *left_node, b_plus_node *right_node, int *head) {

    b_plus_node *splitted;
    if ((left_node && left_node == node->way_less) || value < node->smallest_value) {
        *head = node->smallest_value;
        splitted = new_index_node(node->greatest_value, node->way_mid, node->way_greater);
        node->smallest_value = value;
        node->greatest_value = -1;

        node->way_greater = NULL;
        node->way_mid = right_node;
        if (right_node) {
            right_node->parent = node;
        }
        node->contain_num--;
    } else if ((left_node && left_node == node->way_greater) || value > node->greatest_value) {
        *head = node->greatest_value;
        splitted = new_index_node(value, node->way_greater, right_node);
        node->greatest_value = -1;

        node->way_greater = NULL;
        node->contain_num--;
    } else {
        *head = value;
        splitted = new_index_node(node->greatest_value, right_node, node->way_greater);
        node->greatest_value = -1;

        node->way_greater = NULL;
        node->contain_num--;
    }
    return splitted;
}

b_plus_node *insert_node(b_plus_node *node, int value, b_plus_node *left, b_plus_node *right) {
    if (has_space(node)) {
        if (left == node->way_mid) {
            node->greatest_value = value;
            node->way_greater = right;
            right->parent = node;
            node->way_mid = left;
            left->parent = node;
            node->contain_num = 2;
        } else {
            node->greatest_value = node->smallest_value;
            node->smallest_value = value;
            node->way_greater = node->way_mid;
            node->way_mid = right;
            right->parent = node;
            node->way_less = left;
            left->parent = node;
            node->contain_num = 2;
        }
        return NULL;
    } else {
        int head;
        b_plus_node *splitted = index_split(node, value, left, right, &head);
        if (node->parent) {
            return insert_node(node->parent, head, node, splitted);
        } else {
            b_plus_node *new = new_index_node(head, node, splitted);
            return new;
        }
    }
}

b_plus_tree *new_b_plus_tree() {
    b_plus_tree *new = malloc(sizeof(b_plus_tree));
    if (!new) return NULL;
    new->root = NULL;
    return new;
}

void b_plus_tree_insert(b_plus_tree *tree, int value) {
    if (!tree->root) {
        b_plus_node *data_node = new_data_node(value);
        b_plus_node *empty_node = new_empty_data_node();
        data_node->next = empty_node;
        empty_node->previous = data_node;
        b_plus_node *index_node = new_index_node(value, data_node, empty_node);
        tree->root = index_node;
        return;
    }

    b_plus_node *pre;
    b_plus_node **to_parent = &tree->root;
    b_plus_node *parent = tree->root;
    while (parent && !is_leaf(parent)) {
        pre = parent;
        to_parent = find_way(parent, value);
        parent = *to_parent;
    }

    if (has_space(parent)) {
        b_node_insert(parent, value);
    } else {
        int head;
        b_plus_node *splitted = data_split(parent, value, &head);
        if (parent->parent) {
            b_plus_node *root = insert_node(parent->parent, head, parent, splitted);
            if (root) {
                tree->root = root;
            }
        } else {
            b_plus_node *new_root = new_index_node(head, parent, splitted);
            tree->root = new_root;
        }
    }
}

b_plus_node *b_plus_tree_search(b_plus_tree *tree, int value) {
    b_plus_node *current = tree->root;
    while (current) {
        if (is_leaf(current)) {
            if (value == current->smallest_value || value == current->greatest_value) {
                return current;
            } else if (current->next &&
                       (current->next->smallest_value == value || current->next->greatest_value == value)) {
                return current->next;
            } else {
                return NULL;
            }
        }
        current = *find_way(current, value);
    }
    return NULL;
}

b_plus_node *b_plus_tree_first_in_range(b_plus_tree *tree, int l, int r) {
    b_plus_node *current = tree->root;
    while (current) {
        if (is_leaf(current)) {
            while (current && current->smallest_value == -1) {
                current = current->next;
            }
            return current;
        }
        current = *find_way(current, l);
    }
    return NULL;
}

b_plus_node *get_rotatable_sibling(b_plus_node *node, int *is_left_sibling) {
    if (node == node->parent->way_less) {
        if (node->parent->way_mid->contain_num > 1) {
            *is_left_sibling = 0;
            return node->parent->way_mid;
        }
    } else if (node == node->parent->way_mid) {
        if (node->parent->way_less->contain_num > 1) {
            *is_left_sibling = 1;
            return node->parent->way_less;
        }
        if (node->parent->way_greater && node->parent->way_greater->contain_num > 1) {
            *is_left_sibling = 0;
            return node->parent->way_greater;
        }
    } else {
        if (node->parent->way_mid->contain_num > 1) {
            *is_left_sibling = 1;
            return node->parent->way_mid;
        }
    }
    return NULL;
}

void data_rotate(b_plus_node *node, b_plus_node *sibling, int is_left_sibling) {

    if (!is_left_sibling) {
        node->smallest_value = sibling->smallest_value;
        sibling->smallest_value = sibling->greatest_value;
        sibling->greatest_value = -1;
    } else {
        node->smallest_value = sibling->greatest_value;
        sibling->greatest_value = -1;
    }
    node->contain_num = 1;
    sibling->contain_num = 1;

    if (node == node->parent->way_less) {
        node->parent->smallest_value = sibling->smallest_value;
    } else if (node == node->parent->way_greater) {
        node->parent->greatest_value = node->smallest_value;
    } else {
        if (is_left_sibling) {
            node->parent->smallest_value = node->smallest_value;
        } else {
            node->parent->greatest_value = sibling->smallest_value;
        }
    }
}

void index_rotate(b_plus_node *node, b_plus_node *sibling, int is_left_sibling) {

    if (node == node->parent->way_less) {
        node->smallest_value = node->parent->smallest_value;
        node->parent->smallest_value = sibling->smallest_value;
        sibling->smallest_value = sibling->greatest_value;
        sibling->greatest_value = -1;

        node->way_mid = sibling->way_less;
        if (sibling->way_less) {
            sibling->way_less->parent = node;
        }
        sibling->way_less = sibling->way_mid;
        sibling->way_mid = sibling->way_greater;
        sibling->way_greater = NULL;
        sibling->contain_num--;
    } else if (node == node->parent->way_greater) {
        node->smallest_value = node->parent->greatest_value;
        node->parent->greatest_value = sibling->greatest_value;
        sibling->greatest_value = -1;

        node->way_mid = node->way_less;
        node->way_less = sibling->way_greater;
        if (sibling->way_greater) {
            sibling->way_greater->parent = node;
        }
        sibling->way_greater = NULL;
        sibling->contain_num--;
    } else {
        if (is_left_sibling) {
            node->smallest_value = node->parent->smallest_value;
            node->parent->smallest_value = sibling->greatest_value;
            sibling->greatest_value = -1;

            node->way_mid = node->way_less;
            node->way_less = sibling->way_greater;
            if (sibling->way_greater) {
                sibling->way_greater->parent = node;
            }
            sibling->way_greater = NULL;
            sibling->contain_num--;
        } else {
            node->smallest_value = node->parent->greatest_value;
            node->parent->greatest_value = sibling->smallest_value;
            sibling->smallest_value = sibling->greatest_value;
            sibling->greatest_value = -1;

            node->way_mid = sibling->way_less;
            if (sibling->way_less) {
                sibling->way_less->parent = node;
            }
            sibling->way_less = sibling->way_mid;
            sibling->way_mid = sibling->way_greater;
            sibling->way_greater = NULL;
            sibling->contain_num--;
        }
    }
}

void combine_safe(b_plus_node *node) {
    if (node == node->parent->way_less) {
        b_plus_node *subject = node->parent->way_mid;
        node->smallest_value = node->parent->smallest_value;
        node->greatest_value = subject->smallest_value;


        node->way_mid = subject->way_less;
        if (subject->way_less) {
            subject->way_less->parent = node;
        }
        node->way_greater = subject->way_mid;
        if (subject->way_mid) {
            subject->way_mid->parent = node;
        }
        free(subject);
        node->parent->way_mid = node->parent->way_greater;
        node->parent->way_greater = NULL;
        node->parent->smallest_value = node->parent->greatest_value;
        node->parent->greatest_value = -1;

        node->contain_num = 2;
        node->parent->contain_num = 1;
    } else if (node == node->parent->way_mid) {
        b_plus_node *subject = node->parent->way_less;
        subject->greatest_value = node->parent->smallest_value;
        node->parent->smallest_value = node->parent->greatest_value;
        node->parent->greatest_value = -1;

        subject->way_greater = node->way_less;
        if (node->way_less) {
            node->way_less->parent = subject;
        }
        b_plus_node *parent = node->parent;
        free(node);
        parent->way_mid = parent->way_greater;
        parent->way_greater = NULL;
        parent->contain_num = 1;
        subject->contain_num = 2;
    } else {
        b_plus_node *subject = node->parent->way_mid;
        subject->greatest_value = node->parent->greatest_value;
        node->parent->greatest_value = -1;

        subject->way_greater = node->way_less;
        if (node->way_less) {
            node->way_less->parent = subject;
        }
        b_plus_node *parent = node->parent;
        free(node);
        parent->way_greater = NULL;
        parent->contain_num = 1;
        subject->contain_num = 2;
    }
}

b_plus_node *combine_unsafe(b_plus_node *node) {
    if (node == node->parent->way_less) {
        b_plus_node *subject = node->parent->way_mid;
        node->smallest_value = node->parent->smallest_value;
        node->greatest_value = subject->smallest_value;

        node->way_mid = subject->way_less;
        if (subject->way_less) {
            subject->way_less->parent = node;
        }
        node->way_greater = subject->way_mid;
        if (subject->way_mid) {
            subject->way_mid->parent = node;
        }
        free(subject);
        node->parent->way_mid = NULL;
        node->contain_num = 2;
        return node;
    } else {
        b_plus_node *subject = node->parent->way_less;
        subject->greatest_value = node->parent->smallest_value;

        subject->way_greater = node->way_less;
        if (node->way_less) {
            node->way_less->parent = subject;
        }
        node->parent->way_mid = NULL;
        free(node);
        subject->contain_num = 2;
        return subject;
    }
}

b_plus_node *try_rotate_or_combine(b_plus_node *root, b_plus_node *node) {
    int rotatable_sibling_is_left;
    b_plus_node *rotatable_sibling = get_rotatable_sibling(node, &rotatable_sibling_is_left);
    if (rotatable_sibling) {
        if (node->value) {
            data_rotate(node, rotatable_sibling, rotatable_sibling_is_left);
        } else {
            index_rotate(node, rotatable_sibling, rotatable_sibling_is_left);
        }
        return root;
    }

    if (node->value) {
        if (node == node->parent->way_less) {
            node->parent->way_less = node->parent->way_mid;
            node->parent->way_mid = node->parent->way_greater;
            node->parent->way_greater = NULL;
            node->parent->smallest_value = node->parent->greatest_value;
            node->parent->greatest_value = -1;
        } else if (node == node->parent->way_mid) {
            node->parent->way_mid = node->parent->way_greater;
            node->parent->way_greater = NULL;
            node->parent->greatest_value = -1;
        } else {
            node->parent->way_greater = NULL;
            node->parent->greatest_value = -1;
        }
        node->parent->contain_num--;
        b_plus_node *parent = node->parent;
        if (node->previous) {
            node->previous->next = node->next;
        }
        if (node->next) {
            node->next->previous = node->previous;
        }
        free(node);
        if (parent->contain_num == 0 && parent == root) {
            free(parent);
            return NULL;
        }
        if (parent->contain_num <= 0) {
            return try_rotate_or_combine(root, parent);
        }
        return root;
    }

    if (node->parent->contain_num > 1) {
        combine_safe(node);
        return root;
    }

    node = combine_unsafe(node);
    if (node->parent == root) {
        node->parent = NULL;
        return node;
    }
    return try_rotate_or_combine(root, node->parent);
}

b_plus_node *remove_leaf_node(b_plus_node *root, b_plus_node *node, int value) {

    if (node->contain_num > 1) {
        if (value == node->greatest_value) {
            node->greatest_value = -1;
            node->contain_num = 1;
        } else {
            node->smallest_value = node->greatest_value;
            node->greatest_value = -1;
            node->contain_num = 1;
        }
        return root;
    }

    return try_rotate_or_combine(root, node);
}

int b_plus_tree_remove(b_plus_tree *tree, int value) {
    b_plus_node *n = b_plus_tree_search(tree, value);
    if (!n) {
        return 0;
    }

    tree->root = remove_leaf_node(tree->root, n, value);
    return 1;
}

void free_b_node(b_plus_node *node) {
    if (!node) return;
    if (is_leaf(node)) {
        free(node);
        return;
    }
    free_b_node(node->way_less);
    free_b_node(node->way_mid);
    free_b_node(node->way_greater);
    free(node);
}

void free_b_plus_tree(b_plus_tree *tree) {
    free_b_node(tree->root);
    free(tree);
}

int main() {
    int lines;
    scanf("%d", &lines);
    b_plus_tree *tree = new_b_plus_tree();
    for (int i = 0; i < lines; i++) {
        char cmd[13];
        scanf("%s", cmd);
        if (cmd[1] == 'd') {
            //add
            int x;
            scanf("%d", &x);
            b_plus_tree_insert(tree, x);
        } else if (cmd[1] == 'e') {
            int x;
            scanf("%d", &x);
            b_plus_tree_remove(tree, x);
        } else if (cmd[1] == 'a') {
            int l, r;
            scanf("%d %d", &l, &r);
            b_plus_node *node = b_plus_tree_first_in_range(tree, l, r);
            if (!node) {
                printf("range [%d,%d) = []\n", l, r);
                continue;
            }
            printf("range [%d,%d) = [", l, r);
            int first = 1;
            while (node) {
                if (node->smallest_value == -1 && node->greatest_value == -1) {
                    node = node->next;
                    continue;
                }
                if (node->smallest_value >= r) {
                    break;
                }
                if (l <= node->smallest_value && node->smallest_value < r) {
                    if (first) {
                        printf("%d", node->smallest_value);
                        first = 0;
                    } else {
                        printf(",%d", node->smallest_value);
                    }
                }
                if (l <= node->greatest_value && node->greatest_value < r) {
                    if (first) {
                        printf("%d", node->greatest_value);
                        first = 0;
                    } else {
                        printf(",%d", node->greatest_value);
                    }
                }
                node = node->next;
            }
            printf("]\n");
        }
    }
    return 0;
}