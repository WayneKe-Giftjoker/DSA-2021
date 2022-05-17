#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#define MAXN 1000100

typedef struct inputType{
    int key, left_indx, right_indx;
} InputType;

InputType input[MAXN];

int ans;

void traversal(int curr, int lower, int higher){
    if(curr == -1)
        return;
    if(input[curr].key < higher && input[curr].key > lower)
        ans++;
    int new_higher = (input[curr].key < higher)? input[curr].key : higher;
    traversal(input[curr].left_indx, lower, new_higher);
    int new_lower = (input[curr].key > lower)? input[curr].key : lower;
    traversal(input[curr].right_indx, new_lower, higher);
}

int main(int argc, char *argv[]){
    int N;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
        scanf("%d %d %d", &input[i].key, &input[i].left_indx, &input[i].right_indx);
    traversal(1, 0, 1000000001);
    printf("%d\n", ans);
    return 0;
}
