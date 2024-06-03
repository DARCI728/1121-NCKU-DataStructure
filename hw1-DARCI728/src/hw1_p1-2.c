#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXP_SIZE 10000

int top = -1;
char* stack[MAX_EXP_SIZE];

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

void push(char* ch) {
    stack[++top] = ch;
}

char* pop() {
    return stack[top--];
}

char* PostfixToPrefix(char* postfix) {
    int i;
    int len = strlen(postfix);

    char* prefix = (char*)malloc(sizeof(char) * (len + 1));

    for (i = 0; i < MAX_EXP_SIZE; i++) {
        stack[i] = (char*)malloc(sizeof(char) * (len + 1));
    }

    for (i = 0; i < len; i++) {
        if (isOperator(postfix[i])) {
            char* str1 = pop();
            char* str2 = pop();

            char* operator_str = (char*)malloc(sizeof(char) * 2);
            operator_str[0] = postfix[i];
            operator_str[1] = '\0';

            char* temp = (char*)malloc(sizeof(char) * (strlen(operator_str) + strlen(str2) + strlen(str1) + 1));

            strcpy(temp, operator_str);
            strcat(temp, str2);
            strcat(temp, str1);
            push(temp);

        } else {
            char* operand = (char*)malloc(sizeof(char) * 2);
            operand[0] = postfix[i];
            operand[1] = '\0';
            push(operand);
        }
    }

    while (top != -1) {
        strcat(prefix, pop());
    }

    return prefix;
}

int main(int argc, char* argv[]) {
    char postfix[MAX_EXP_SIZE];
    scanf("%s", postfix);

    char* prefix = PostfixToPrefix(postfix);
    printf("%s\n", prefix);

    return 0;
}