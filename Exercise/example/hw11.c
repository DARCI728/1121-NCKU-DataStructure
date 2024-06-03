#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 100000
#define swap(a, b) do {\
    a ^= b;\
    b ^= a;\
    a ^= b;\
} while (0);

int cnt = -1;

void toheap(int *heap, int parent){
    int smallest = parent;
    int left = parent * 2 + 1;
    int right = parent * 2 + 2;

    if (left <= cnt && heap[left] < heap[smallest])
        smallest = left;
     
    if (right <= cnt && heap[right] < heap[smallest])
        smallest = right;
    
    if (smallest != parent){
        swap(heap[parent], heap[smallest]);
        toheap(heap, smallest);
        toheap(heap, (parent - 1) / 2);
    }
}

int main(){
    int  dest, value;
    int  *heap = calloc(SIZE, sizeof(int));
    char buffer[100];
    while (true){
        scanf(" %s", buffer);
        switch (buffer[0]){
            case 'i':
                scanf(" %d", &heap[++cnt]);
                int tmp = cnt;
                while (tmp > 0 && heap[tmp] < heap[(tmp - 1) / 2]){
                    swap(heap[tmp], heap[(tmp - 1) / 2]);
                    tmp = (tmp - 1) / 2;
                    toheap(heap, tmp);
                }
                if (cnt == SIZE - 1) realloc(heap, sizeof(int) * SIZE * 2);
                break;
            case 'r':
                if (cnt == -1){
                    printf("empty\n");
                    break;
                }
                printf("%d\n", heap[0]);
                heap[0] = heap[cnt--];
                toheap(heap, 0);
                break;
            case 'c':
                scanf("%d,%d", &dest, &value);
                if (dest > cnt || dest < 0){
                    printf("out of range\n");
                    break;
                }

                heap[dest] = value;
                toheap(heap, ((dest - 1) / 2) > 0 ? (dest - 1) / 2 : 0);
                toheap(heap, dest);
                break;
            case 'q':
                exit(0);
        }
    }
}