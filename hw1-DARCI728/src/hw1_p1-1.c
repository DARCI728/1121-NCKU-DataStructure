#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXP_SIZE 101

int isOprator(const char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int precedence(const char operator) {
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return -1;
    }
}

char* InfixToPostfix(char* infix) {
    int i, j;
    int len = strlen(infix), top = -1;
    char* postfix = (char*)malloc(sizeof(char) * (len + 1));
    char stack[MAX_EXP_SIZE];

    for (i = 0, j = 0; i < len; i++) {
        if (isalnum(infix[i])) {
            postfix[j++] = infix[i];
        } else if (infix[i] == '(') {
            stack[++top] = infix[i];
        } else if (infix[i] == ')') {
            while (stack[top] != '(') {
                postfix[j++] = stack[top--];
            }
            top--;
        } else if (isOprator(infix[i])) {
            while (top > -1 && precedence(infix[i]) <= precedence(stack[top])) {
                postfix[j++] = stack[top--];
            }
            stack[++top] = infix[i];
        }
    }

    while (top > -1) {
        postfix[j++] = stack[top--];
    }

    postfix[j] = '\0';
    return postfix;
}

int main(int argc, char* argv[]) {
    char infix[MAX_EXP_SIZE];
    scanf("%s", infix);

    char* postfix = InfixToPostfix(infix);
    printf("%s", postfix);
    free(postfix);
    return 0;
}
