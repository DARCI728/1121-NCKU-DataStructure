#include <stdio.h>
#include <string.h>

int wParent[10000];
int hParent[10000];
int wmove, hmove;

void wUnion(int i,int j){
    if (wParent[i] <= wParent[j]) {
        wParent[i] += wParent[j];
        wParent[j] = i;
    } else {
        wParent[j] += wParent[i];
        wParent[i] = j;
    }
}

void hUnion(int x,int y){
    if (x == y) return;
    if (hParent[x] == hParent[y]) {
        hParent[x]--;
        hParent[y] = x;
    } else if (hParent[x] < hParent[y]){
        hParent[y] = x;
    } else {
        hParent[x] = y;
    }
}

int wFind(int i){
    while (wParent[i] >= 0) {
        i = wParent[i];
        wmove++;
    }
    return i;
}

int hFind(int i){
    while (hParent[i] >= 0) {
        i = hParent[i];
        hmove++;
    }
    return i;
}

int main(){
    int round, a, b, n;
    scanf("%d",&round);

    while(round--) {
        int ops;
        wmove = 0;
        hmove = 0;
        scanf("%d %d", &n, &ops);
        for(int i = 0;i < n; i++){
            wParent[i] = -1;
            hParent[i] = -1;
        }

        while(ops != 0) {
            char str[10];
            scanf("%s", str);
            if(strcmp(str, "union") == 0){
                scanf("%d %d" ,&a ,&b);
                hUnion(hFind(a), hFind(b));
                wUnion(wFind(a), wFind(b));
                ops--;
            } else if(strcmp(str, "find") == 0){
                scanf("%d", &a);
                hFind(a);
                wFind(a);
                ops--;
            }
        }
        printf("weight rule move: %d\n", wmove);
        printf("height rule move: %d\n", hmove);
    }
}