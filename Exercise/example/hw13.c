#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;
struct Node {
    int value;
    Node *left, *right;
};

int pre[1000], po[1000], in[1000];

Node* newNode(int data) {
    Node* node = malloc(sizeof(Node));
    node->value = data;
    node->right = NULL;
    node->left = NULL;
    return node;
}

int findPosition(int start, int end, int key){
    for(int i = start; i <= end; i++){
        if(in[i] == key)
            return i;
    }
    return -1;
}    

Node* buildPre(int preStart, int preEnd, int inStart, int inEnd) {
    Node* root = newNode(pre[preStart]);
    int position = findPosition(inStart, inEnd, pre[preStart]);
    if (position != inStart) {
        root->left = buildPre(preStart+1, preStart+(position-inStart), inStart, position-1);
    }
    if (position != inEnd) {
        root->right = buildPre(preEnd-(inEnd-position)+1, preEnd, position+1, inEnd);
    }
    return root;
}

Node *buildPost(int inStart, int inEnd, int postStart, int postEnd) {
    Node* root = newNode(po[postEnd]);
    int position = findPosition(inStart, inEnd, po[postEnd]);
    if (position != inStart) {
        root->left = buildPost(inStart, position-1, postStart, postStart+(position-inStart)-1);
    }
    if (position != inEnd) {
        root->right = buildPost(position+1, inEnd, postEnd-(inEnd-position), postEnd-1);
    }
    return root;
}

void writePost(Node* node) {
    if (node == NULL) return;
    writePost(node->left);
    writePost(node->right);
    printf("%d ", node->value);
}

void writePre(Node* node) {
    if (node == NULL) return;
    printf("%d ", node->value);
    writePre(node->left);
    writePre(node->right);
}

int main(){
    int n;
    scanf("%d", &n);

    while(n != 0){
        char operation[20];
        int l;
        scanf("%s", operation);
        scanf("%d", &l);

        if(strcmp(operation, "preorder-inorder") == 0) {
            for(int i = 0; i < l; i++) scanf("%d", &pre[i]);
            for(int i = 0; i < l; i++) scanf("%d", &in[i]);
            Node* root = buildPre(0, l-1, 0, l-1);
            writePost(root);
            printf("\n");
        } else if(strcmp(operation, "postorder-inorder") == 0) {
            int *post = (int*)malloc(sizeof(int) * l);
            for(int i = 0; i < l; i++) scanf("%d", &po[i]);
            for(int i = 0; i < l; i++) scanf("%d", &in[i]);
            Node* root = buildPost(0, l-1, 0, l-1);
            writePre(root);
            printf("\n");
        }
        n--;
    }
    return 0;
}
