    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    typedef struct node {
        int data_smallest;
        int data_largest;

        struct node *way_less;
        struct node *way_mid;
        struct node *way_greater;

        struct node *parent;

        int less_cnt;
        int mid_cnt;
        int greater_cnt;

        int contain_num;
    } bTreeNode;

    typedef struct {
        bTreeNode *root;
    } b_tree;

    bTreeNode *new_b_node(int data) {
        bTreeNode *new = malloc(sizeof(bTreeNode));
        if (!new) return NULL;
        new->way_less = NULL;
        new->data_smallest = data;
        new->way_mid = NULL;
        new->data_largest = -1;
        new->way_greater = NULL;

        new->parent = NULL;

        new->less_cnt = 0;
        new->mid_cnt = 0;
        new->greater_cnt = 0;
        new->contain_num = 1;
        return new;
    }

    bTreeNode **find_way(bTreeNode *node, int search_value) {
        if (search_value <= node->data_smallest) {
            return &node->way_less;
        }
        if (node->data_largest == -1) {
            return &node->way_mid;
        }
        if (search_value <= node->data_largest) {
            return &node->way_mid;
        }
        return &node->way_greater;
    }

    int has_space(bTreeNode *node) {
        return node->data_largest == -1;
    }

    int is_leaf(bTreeNode *node) {
        return !node->way_less && !node->way_mid && !node->way_greater;
    }

    int total_cnt(bTreeNode *node) {
        if (!node) {
            return 0;
        }
        return node->less_cnt + node->mid_cnt + node->greater_cnt;
    }

    void update_cnt(bTreeNode *node) {
        int less_node_cnt = total_cnt(node->way_less) + (node->way_less ? node->way_less->contain_num : 0);
        int mid_node_cnt = total_cnt(node->way_mid) + (node->way_mid ? node->way_mid->contain_num : 0);
        int greater_node_cnt = total_cnt(node->way_greater) + (node->way_greater ? node->way_greater->contain_num : 0);
        int changed =
                (less_node_cnt ^ node->less_cnt) | (mid_node_cnt ^ node->mid_cnt) | (greater_node_cnt ^ node->greater_cnt);

        if (changed) {
            node->less_cnt = less_node_cnt;
            node->mid_cnt = mid_node_cnt;
            node->greater_cnt = greater_node_cnt;
        }
        if (node->parent) {
            update_cnt(node->parent);
        }
    }

    void node_insert(bTreeNode *node, int insert_value) {
        if (insert_value > node->data_smallest) {
            node->data_largest = insert_value;
        } else {
            node->data_largest = node->data_smallest;
            node->data_smallest = insert_value;
            node->way_greater = node->way_mid;
            node->way_mid = node->way_less;
            node->way_less = NULL;
        }
        node->contain_num++;
        update_cnt(node);
    }

    bTreeNode *split(bTreeNode *node, int data, bTreeNode *left, bTreeNode *right, int *head) {
        bTreeNode *splitted;
        if (data < node->data_smallest) {
            *head = node->data_smallest;
            splitted = new_b_node(node->data_largest);
            node->data_smallest = data;
            node->data_largest = -1;
            splitted->way_mid = node->way_greater;
            if (node->way_greater) {
                node->way_greater->parent = splitted;
            }
            splitted->way_less = node->way_mid;
            if (node->way_mid) {
                node->way_mid->parent = splitted;
            }
            node->way_greater = NULL;
            node->way_mid = right;
            if (right) {
                right->parent = node;
            }
            node->contain_num--;
            update_cnt(node);
            update_cnt(splitted);
        } else if (data > node->data_largest) {
            *head = node->data_largest;
            splitted = new_b_node(data);
            node->data_largest = -1;
            splitted->way_less = node->way_greater;
            if (node->way_greater) {
                node->way_greater->parent = splitted;
            }
            splitted->way_mid = right;
            if (right) {
                right->parent = splitted;
            }
            node->way_greater = NULL;
            node->contain_num--;
            update_cnt(node);
        } else {
            *head = data;
            splitted = new_b_node(node->data_largest);
            node->data_largest = -1;
            splitted->way_less = right;
            if (right) {
                right->parent = splitted;
            }
            splitted->way_mid = node->way_greater;
            if (node->way_greater) {
                node->way_greater->parent = splitted;
            }
            node->way_greater = NULL;
            node->contain_num--;
            update_cnt(node);
            update_cnt(splitted);
        }
        return splitted;
    }

    bTreeNode *insert_node(bTreeNode *node, int data, bTreeNode *left, bTreeNode *right) {
        if (has_space(node)) {
            if (data > node->data_smallest) {
                node->data_largest = data;
                node->way_greater = right;
                right->parent = node;
                node->way_mid = left;
                left->parent = node;
                node->contain_num++;
                update_cnt(node);
            } else {
                node->data_largest = node->data_smallest;
                node->data_smallest = data;
                node->way_greater = node->way_mid;
                node->way_mid = right;
                right->parent = node;
                node->way_less = left;
                left->parent = node;
                node->contain_num++;
                update_cnt(node);
            }
            return NULL;
        } else {
            int head;
            bTreeNode *splitted = split(node, data, left, right, &head);
            update_cnt(splitted);
            if (node->parent) {
                return index_node_insert_node(node->parent, head, node, splitted);
            } else {
                bTreeNode *new = new_b_node(head);
                new->way_mid = splitted;
                splitted->parent = new;
                new->way_less = node;
                node->parent = new;
                update_cnt(new);
                return new;
            }
        }
    }

    b_tree *new_b_tree() {
        b_tree *new = malloc(sizeof(b_tree));
        if (!new) return NULL;
        new->root = NULL;
        return new;
    }

    void b_tree_insert(b_tree *tree, int data) {
        if (!tree->root) {
            tree->root = new_b_node(data);
            return;
        }

        bTreeNode *parent = tree->root;
        while (!is_leaf(parent)) {
            parent = *find_way(parent, data);
        }

        if (has_space(parent)) {
            node_insert(parent, data);
        } else {
            int head;
            bTreeNode *splitted;
            splitted = split(parent, data, NULL, NULL, &head);
            if (parent->parent) {
                bTreeNode *root = index_node_insert_node(parent->parent, head, parent, splitted);
                if (root) {
                    tree->root = root;
                }
            } else {
                bTreeNode *new_root = new_b_node(head);
                new_root->way_less = parent;
                parent->parent = new_root;
                new_root->way_mid = splitted;
                splitted->parent = new_root;
                update_cnt(new_root);
                tree->root = new_root;
            }
        }
    }

    bTreeNode *b_tree_search(b_tree *tree, int x) {
        bTreeNode *current = tree->root;
        while (current) {
            if (x == current->data_smallest || x == current->data_largest) {
                return current;
            }
            current = *find_way(current, x);
        }
        return NULL;
    }

    bTreeNode *find_replacer(bTreeNode *node, int x, int *replace_num) {
        if (x == node->data_smallest) {
            bTreeNode *current = node->way_mid;
            while (current->way_less) {
                current = current->way_less;
            }
            *replace_num = current->data_smallest;
            return current;
        } else {
            bTreeNode *current = node->way_greater;
            while (current->way_less) {
                current = current->way_less;
            }
            *replace_num = current->data_smallest;
            return current;
        }
    }

    bTreeNode *get_rotatable_sibling(bTreeNode *n, int *is_left_sibling) {
        if (n == n->parent->way_less) {
            if (n->parent->way_mid->contain_num > 1) {
                *is_left_sibling = 0;
                return n->parent->way_mid;
            }
        } else if (n == n->parent->way_mid) {
            if (n->parent->way_less->contain_num > 1) {
                *is_left_sibling = 1;
                return n->parent->way_less;
            }
            if (n->parent->way_greater && n->parent->way_greater->contain_num > 1) {
                *is_left_sibling = 0;
                return n->parent->way_greater;
            }
        } else {
            if (n->parent->way_mid->contain_num > 1) {
                *is_left_sibling = 1;
                return n->parent->way_mid;
            }
        }
        return NULL;
    }

    void rotate(bTreeNode *n, bTreeNode *sibling, int is_left_sibling) {
        if (n == n->parent->way_less) {
            n->data_smallest = n->parent->data_smallest;
            n->parent->data_smallest = sibling->data_smallest;
            sibling->data_smallest = sibling->data_largest;
            sibling->data_largest = -1;

            n->way_mid = sibling->way_less;
            if (sibling->way_less) {
                sibling->way_less->parent = n;
            }
            sibling->way_less = sibling->way_mid;
            sibling->way_mid = sibling->way_greater;
            sibling->way_greater = NULL;
            sibling->contain_num--;
            update_cnt(n);
            update_cnt(sibling);
        } else if (n == n->parent->way_greater) {
            n->data_smallest = n->parent->data_largest;
            n->parent->data_largest = sibling->data_largest;
            sibling->data_largest = -1;

            n->way_mid = n->way_less;
            n->way_less = sibling->way_greater;
            if (sibling->way_greater) {
                sibling->way_greater->parent = n;
            }
            sibling->way_greater = NULL;
            sibling->contain_num--;
            update_cnt(n);
            update_cnt(sibling);
        } else {
            if (is_left_sibling) {
                n->data_smallest = n->parent->data_smallest;
                n->parent->data_smallest = sibling->data_largest;
                sibling->data_largest = -1;

                n->way_mid = n->way_less;
                n->way_less = sibling->way_greater;
                if (sibling->way_greater) {
                    sibling->way_greater->parent = n;
                }
                sibling->way_greater = NULL;
                sibling->contain_num--;
                update_cnt(n);
                update_cnt(sibling);
            } else {
                n->data_smallest = n->parent->data_largest;
                n->parent->data_largest = sibling->data_smallest;
                sibling->data_smallest = sibling->data_largest;
                sibling->data_largest = -1;

                n->way_mid = sibling->way_less;
                if (sibling->way_less) {
                    sibling->way_less->parent = n;
                }
                sibling->way_less = sibling->way_mid;
                sibling->way_mid = sibling->way_greater;
                sibling->way_greater = NULL;
                sibling->contain_num--;
                update_cnt(n);
                update_cnt(sibling);
            }
        }
    }

    void combine_safe(bTreeNode *n) {
        if (n == n->parent->way_less) {
            bTreeNode *subject = n->parent->way_mid;
            n->data_smallest = n->parent->data_smallest;
            n->data_largest = subject->data_smallest;

            n->way_mid = subject->way_less;
            if (subject->way_less) {
                subject->way_less->parent = n;
            }
            n->way_greater = subject->way_mid;
            if (subject->way_mid) {
                subject->way_mid->parent = n;
            }
            free(subject);
            n->parent->way_mid = n->parent->way_greater;
            n->parent->way_greater = NULL;
            n->parent->data_smallest = n->parent->data_largest;
            n->parent->data_largest = -1;

            n->contain_num = 2;
            n->parent->contain_num--;
            update_cnt(n);
        } else if (n == n->parent->way_mid) {
            bTreeNode *subject = n->parent->way_less;
            subject->data_largest = n->parent->data_smallest;
            n->parent->data_smallest = n->parent->data_largest;
            n->parent->data_largest = -1;

            subject->way_greater = n->way_less;
            if (n->way_less) {
                n->way_less->parent = subject;
            }
            bTreeNode *parent = n->parent;
            free(n);
            parent->way_mid = parent->way_greater;
            parent->way_greater = NULL;
            parent->contain_num = 1;
            subject->contain_num = 2;
            update_cnt(subject);
        } else {
            bTreeNode *subject = n->parent->way_mid;
            subject->data_largest = n->parent->data_largest;
            n->parent->data_largest = -1;

            subject->way_greater = n->way_less;
            if (n->way_less) {
                n->way_less->parent = subject;
            }
            bTreeNode *parent = n->parent;
            free(n);
            parent->way_greater = NULL;
            parent->contain_num = 1;
            subject->contain_num = 2;
            update_cnt(subject);
        }
    }

    bTreeNode *combine_unsafe(bTreeNode *n) {
        if (n == n->parent->way_less) {
            bTreeNode *subject = n->parent->way_mid;
            n->data_smallest = n->parent->data_smallest;
            n->data_largest = subject->data_smallest;

            n->way_mid = subject->way_less;
            if (subject->way_less) {
                subject->way_less->parent = n;
            }
            n->way_greater = subject->way_mid;
            if (subject->way_mid) {
                subject->way_mid->parent = n;
            }
            free(subject);
            n->parent->way_mid = NULL;
            n->contain_num = 2;
            update_cnt(n);
            return n;
        } else {
            bTreeNode *subject = n->parent->way_less;
            subject->data_largest = n->parent->data_smallest;

            subject->way_greater = n->way_less;
            if (n->way_less) {
                n->way_less->parent = subject;
            }
            n->parent->way_mid = NULL;
            free(n);
            subject->contain_num = 2;
            update_cnt(subject);
            return subject;
        }
    }

    bTreeNode *try_rotate_or_combine(bTreeNode *root, bTreeNode *n) {
        int rotatable_sibling_is_left;
        bTreeNode *rotatable_sibling = get_rotatable_sibling(n, &rotatable_sibling_is_left);
        if (rotatable_sibling) {
            rotate(n, rotatable_sibling, rotatable_sibling_is_left);
            return root;
        }

        if (n->parent->contain_num > 1) {
            combine_safe(n);
            return root;
        }

        n = combine_unsafe(n);
        if (n->parent == root) {
            n->parent = NULL;
            return n;
        }
        return try_rotate_or_combine(root, n->parent);
    }

    bTreeNode *remove_leaf_node(bTreeNode *root, bTreeNode *node, int number) {

        if (node->contain_num > 1) {
            if (number == node->data_largest) {
                node->data_largest = -1;
                node->way_greater = NULL;
                node->contain_num--;
                update_cnt(node);
            } else {
                node->data_smallest = node->data_largest;
                node->data_largest = -1;
                node->way_less = node->way_mid;
                node->way_mid = node->way_greater;
                node->way_greater = NULL;
                node->contain_num--;
                update_cnt(node);
            }
            return root;
        }

        if (node == root) { //root of all
            free(node);
            return NULL;
        }
        return try_rotate_or_combine(root, node);
    }


    int b_tree_remove(b_tree *tree, int x) {
        bTreeNode *n = b_tree_search(tree, x);
        if (!n) {
            return 0;
        }
        if (!is_leaf(n)) {
            int replace_num;
            bTreeNode *replacer = find_replacer(n, x, &replace_num);
            if (n->data_smallest == x) {
                n->data_smallest = replace_num;
                n = replacer;
                x = replace_num;
            } else {
                n->data_largest = replace_num;
                n = replacer;
                x = replace_num;
            }
        }

        tree->root = remove_leaf_node(tree->root, n, x);
        return 1;
    }

    bTreeNode *b_tree_getk(b_tree *tree, int k, int *out_number) {
        bTreeNode *current = tree->root;
        int acc = 0;

        while (current) {
            if (acc + current->less_cnt >= k) {
                current = current->way_less;
                continue;
            }
            acc += current->less_cnt;
            if (acc + 1 == k) {
                *out_number = current->data_smallest;
                return current;
            }
            acc++;
            if (acc + current->mid_cnt >= k) {
                current = current->way_mid;
                continue;
            }
            acc += current->mid_cnt;
            if (acc + 1 == k) {
                if (current->data_largest != -1) {
                    *out_number = current->data_largest;
                    return current;
                }
                return NULL;
            }
            acc++;
            current = current->way_greater;
        }

        return NULL; //return NULL represent for found
    }

    int b_tree_removek(b_tree *tree, int k) {
        int number;
        int deleted_num;
        bTreeNode *n = b_tree_getk(tree, k, &number);
        if (!n) return -1;
        deleted_num = number;
        if (!is_leaf(n)) {
            int replace_num;
            bTreeNode *replacer = find_replacer(n, number, &replace_num);
            if (n->data_smallest == number) {
                n->data_smallest = replace_num;
                n = replacer;
                number = replace_num;
            } else {
                n->data_largest = replace_num;
                n = replacer;
                number = replace_num;
            }
        }

        tree->root = remove_leaf_node(tree->root, n, number);
        return deleted_num;
    }

    void delete_node(bTreeNode *node) {
        if (!node) return;
        free_b_node(node->way_less);
        free_b_node(node->way_mid);
        free_b_node(node->way_greater);
        free(node);
    }

    void free_b_tree(b_tree *tree) {
        free_b_node(tree->root);
        free(tree);
    }


    int main() {
        int n;
        scanf("%d", &n);
        b_tree *tree = new_b_tree();
        for (int i = 0; i < n; i++) {
            char cmd[8];
            scanf("%s", cmd);
            if (strcmp(cmd, "add") == 0) {
                int in;
                scanf("%d", &in);
                b_tree_insert(tree, in);
                printf("add(%d) = ok\n", in);
            } else if (strcmp(cmd, "getk") == 0) {
                int k;
                scanf("%d", &k);
                int number;
                if (b_tree_getk(tree, k, &number)) {
                    printf("getk(%d) = %d\n", k, number);
                } else {
                    printf("getk(%d) = not found\n", k);
                }
            } else if (strcmp(cmd, "get") == 0) {
                int in;
                scanf("%d", &in);
                if (b_tree_search(tree, in)) {
                    printf("get(%d) = %d\n", in, in);
                } else {
                    printf("get(%d) = not found\n", in);
                }
            } else if (strcmp(cmd, "removek") == 0) {
                int k;
                scanf("%d", &k);
                int x;
                if ((x = b_tree_removek(tree, k)) != -1) {
                    printf("removek(%d) = %d\n", k, x);
                } else {
                    printf("removek(%d) = not found\n", k);
                }
            } else if (strcmp(cmd, "remove") == 0) {
                int in;
                scanf("%d", &in);
                if (b_tree_remove(tree, in)) {
                    printf("remove(%d) = %d\n", in, in);
                } else {
                    printf("remove(%d) = not found\n", in);
                }
            }
            if (tree->root) {
            }
        }

        free_b_tree(tree);
        return 0;
    }