#include<stdio.h>
#include<string.h>
#define buckets 20 
typedef int bool;  
enum { false, true }; 
int array[buckets];

int insert(int num){
    for (int i=0; i<buckets; i++) {
        int index = (num + i) % buckets;
        if (array[index] == -1) {
            array[index] = num;
            return index;
        }
    }
    return -1;	
}

int search(int num){
    for (int i=0; i<buckets; i++) {
        int index = (num + i) % buckets;
        if (array[index] == num) {
            return index;
        } else if (array[index] == -1) {
            return -1;
        }
    }
    return -1;
}

void delete(int index) {
    int target = array[index] % buckets;
    int empty = index;
    for (int i=1; i<buckets; i++) {
        int curr = array[index+i] % buckets;
        if (curr == target && (index+i)%buckets != index) {
            array[empty] = array[index+i];
            array[index+i] = -1;
            empty = index+i; 
        }
    }
}

int main(){
	char s[50];
	int data, number=0;

	for(int i=0; i<buckets; i++) {
		array[i] = -1;
	}

	while (1) {
		scanf("%s", s);
		scanf("%d",&data);
		if (strcmp(s,"insert")==0) {
			if (number<=buckets) {
				int res = insert(data);
				printf("Insert %d %d.\n", res, data);
				number++;
			} else {
				printf("The table is full.\n");
			}
		} else if (strcmp(s,"delete")==0) {
            if (number==0) {
				printf("The table is empty.\n");
            } else {
                int res = search(data);
                if (res == -1) {
                    printf("The key is not in table.\n");
                } else {
                    delete(res);
                    number--;
                    printf("Delete %d.\n", data);
                }
            }
		} else if (strcmp(s,"search")==0){
			int index = search(data);
            if (index==-1) {
				printf("Not found.\n");
            } else {
                printf("Found in %d.\n", index);
            }
		}
        for (int i=0; i<buckets; i++) {
            printf("%d ", array[i]);
        }
	}
    return 0;
}
