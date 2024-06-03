#include<stdio.h>
#include<string.h>
#include <stdbool.h>

static int array[50];
static int next_index = 1;

void insert(int x) {
    int current, done, grandparent, grandparent_left_child, grandparent_right_child;
    current = ++next_index;
    if (current % 2 == 1 && x < array[current - 1]) {
        array[current] = array[current - 1];
        current--;
    }
    done = false;
    while (!done && current >= 4) {
        grandparent = current / 4;
        grandparent_left_child = 2 * grandparent;
        grandparent_right_child = grandparent_left_child + 1;
        if (x < array[grandparent_left_child]) {
            array[current] = array[grandparent_left_child];
            current = grandparent_left_child;
        } else if (x > array[grandparent_right_child]) {
            array[current] = array[grandparent_right_child];
            current = grandparent_right_child;
        } else {
            done = true;
        }
    }
    array[current] = x;
}

void delete_min(int min_delete) {
    int left_child = min_delete * 2;
    int sibling_left_child = (min_delete + 1) * 2;
    int change;
    bool exist = false;
    if (sibling_left_child <= next_index) {
        if (array[left_child] < array[sibling_left_child]) {
            change = left_child;
        } else {
            change = sibling_left_child;
        }
        exist = true;
    } else if (left_child <= next_index) {
        change = left_child;
        exist = true;
    } else {
        if (array[min_delete] > array[min_delete + 1]) {
            int temp = array[min_delete];
            array[min_delete] = array[min_delete + 1];
            array[min_delete + 1] = temp;
        }
    }
    if (exist == true && array[change] < array[min_delete]) {
        int temp = array[min_delete];
        array[min_delete] = array[change];
        array[change] = temp;
        delete_min(change);
    }
    if (exist == false && min_delete + 1 <= next_index) {
        if (array[min_delete] > array[min_delete + 1]) {
            int temp = array[min_delete];
            array[min_delete] = array[min_delete + 1];
            array[min_delete + 1] = temp;
        }
    }

}

void delete_max(int max_delete) {
    int right_child = max_delete * 2 + 1;
    int sibling_right_child = (max_delete - 1) * 2 + 1;
    int change;
    bool exist = false;
    if (sibling_right_child <= next_index) {
        if (array[right_child] > array[sibling_right_child]) {
            change = right_child;
        } else {
            change = sibling_right_child;
        }
        exist = true;
    } else if (right_child <= next_index) {
        change = right_child;
        exist = true;
    } else {
        if (array[max_delete] < array[max_delete - 1]) {
            int temp = array[max_delete];
            array[max_delete] = array[max_delete - 1];
            array[max_delete - 1] = temp;
        }
    }
    if (exist == true && array[change] > array[max_delete]) {
        int temp = array[max_delete];
        array[max_delete] = array[change];
        array[change] = temp;
        delete_max(change);
    }
    if (exist == false) {
        if (array[max_delete] < array[max_delete - 1]) {
            int temp = array[max_delete];
            array[max_delete] = array[max_delete - 1];
            array[max_delete - 1] = temp;
        }
    }

}

void show() {
    int i;
    int k = 3;
    printf("NULL\n");
    for (i = 0; i < next_index - 1; i++) {
        if (i + 2 != k && i != next_index - 2) {
            printf("%d ", array[i + 2]);
        } else if (i + 2 != k && i == next_index - 2) {
            printf("%d\n", array[i + 2]);
        }
        if (i + 2 == k) {
            k = k * 2 + 1;
            printf("%d\n", array[i + 2]);
        }
    }
}

int main() {
    int val;
    char ch[50];

    while (true) {
        scanf("%s", ch);
        if (strcmp(ch, "insert") == 0) {
            scanf("%d", &val);
            insert(val);
        } else if (strcmp(ch, "delete") == 0) {
            scanf("%s", ch);
            if (strcmp(ch, "min") == 0) {
                array[2] = array[next_index];
                delete_min(2);
                next_index--;
            }
            if (strcmp(ch, "max") == 0) {
                array[3] = array[next_index];
                delete_max(3);
                next_index--;
            }
        } else if (strcmp(ch, "show") == 0) {
            show();
        } else {
            break;
        }
    }
}
