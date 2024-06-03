#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pNode pNode;
struct pNode {
    int bit_number, val;
    char *key;
    pNode *left, *right;
};

pNode *root = NULL;

int compare(char *k1, char*k2){
    int i = 0;
    for(; k1[i] == k2[i]; i++);
    return i + 1;
}

pNode* findKey(char *k){
    pNode *cur = root->left, *pre = root;  
    while (cur->bit_number > pre->bit_number) {
        pre = cur;
        cur = (k[cur->bit_number-1] == '0') ? cur->left : cur->right;
    }
    return cur;
}

void insert(char * k, int value){
    pNode *new = malloc(sizeof(pNode));
    new->key = k;  
	new->val = value;

    if (root == NULL) {
        root = new;  
		root->bit_number = 0;
        root->left = root; 
		root->right = NULL;  
        printf("insert -> %d\n", value);
        return;
    }
    
    pNode *target = findKey(k);
    if (!strcmp(target->key, k)) {
        printf("insert -> conflict\n");
        return;
    }
    printf("insert -> %d\n", value);
    
    int diff = new->bit_number = compare(target->key, new->key);
    pNode *cur = root->left, *pre = root;
    while (cur->bit_number < diff && cur->bit_number > pre->bit_number) {
        pre = cur;
        cur = (k[cur->bit_number-1] == '0') ? cur->left : cur->right;
    }
    if (pre == root || cur == pre->left) {
        pre->left = new;
    } else {
        pre->right = new;
    }

    if (k[new->bit_number-1] == '0') {
        new->left = new;
        new->right = cur;
    } else {
        new->right = new;
        new->left = cur;
    }
}

void delete(char * k){
    if (root == NULL) {
        printf("delete -> not found\n");
        return;
    }

    pNode *cur = root->left, *pre = root;  
    while (cur->bit_number > pre->bit_number) {
        pre = cur;
        cur = (k[cur->bit_number-1] == '0') ? cur->left : cur->right;
    }
    if (strcmp(k,cur->key)) {
        printf("delete -> not found\n");
        return;
    }
    printf("delete -> %d\n", cur->val);
    
    if (cur == root) {
        root = NULL; 
    } else if(cur == pre) {
        pre = (cur == pre->right) ? pre->left : pre->right;  
    } else {
        pre->bit_number = cur->bit_number;
        if(cur->left == pre) {
            pre->right = cur->right;
        } else if (cur->right == pre) {
            pre->left = cur->left;
        } else {
            pre->left = cur->left;
            pre->right = cur->right;
        }
        cur = pre;
    }
}

void search(char *k){
    if (!root) {
        printf("search -> not found\n");
        return;
    }
    pNode *cur = findKey(k);
    if (strcmp(cur->key,k)) {
        printf("search -> not found\n");
    } else {
        printf("search -> %d\n", cur->val);
    }  
}

int main(){
    int n;
    char ord[10], key[1000];
    int val;  
    scanf("%d", &n);

    while (1) {
        scanf("%s", ord);
        char *k = (char*)malloc((n+1) * sizeof(char));
        scanf("%s", k);
        if(strcmp(ord, "insert") == 0) {
            scanf("%d", &val);
            insert(k, val);
        } else if(strcmp(ord,"delete") == 0) {
            delete(k);
        } else if(strcmp(ord,"search") == 0) {
            search(k);
        } else {
            break;
		}
    }
}
