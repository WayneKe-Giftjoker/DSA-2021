#include <stdio.h>
#include <stdlib.h>
#include "generator.h"

typedef struct triangle{
    int top, left, right;
} Tri;

Tri position[3000100];
int position_l[3000100];
int position_r[3000100];
int record[3000100];
long int ans;

int cmp(const void *a, const void *b){
    int top1 = ((Tri *)a)->top, top2 = ((Tri *)b)->top;
    if(top1 != top2)
        return (top1 - top2);
    else{
        return ((Tri *)b)->right - ((Tri *)a)->right;
        // if(((Tri *)a)->right < ((Tri *)b)->right)//記得滿足全序條件!!!!
        //     return 1;
        // else
        //     return -1;
    }
}

void mergeSort(int start, int end){
    if(end > start){
        int mid = (start + end) / 2;
        mergeSort(start, mid);
        mergeSort(mid + 1, end);
        
        int n1 = mid - start + 1, n2 = end - mid;
        int idx = start, idx1 = start, idx2 = mid + 1;

        while(idx1 <= mid && idx2 <= end){
            if(position_l[idx2] <= position_r[idx1])
                idx2++;
            else{
                ans += (idx2 - mid - 1);
                idx1++;
            }
        }
        while(idx1 <= mid){
            ans += (idx2 - mid - 1);
            idx1++;
        }

        idx1 = start; idx2 = mid + 1;
        while(idx1 <= mid && idx2 <= end){
            if(position_l[idx1] <= position_l[idx2])
                record[idx++] = position_l[idx1++];
            else
                record[idx++] = position_l[idx2++];
        }
        while(idx1 <= mid){
            record[idx++] = position_l[idx1++];
        }
        while(idx2 <= end){
            record[idx++] = position_l[idx2++];
        }
        for(int i = start; i <= end; i++)
            position_l[i] = record[i];
        idx = start; idx1 = start; idx2 = mid + 1;
        while(idx1 <= mid && idx2 <= end){
            if(position_r[idx1] <= position_r[idx2])
                record[idx++] = position_r[idx1++];
            else
                record[idx++] = position_r[idx2++];
        }
        while(idx1 <= mid){
            record[idx++] = position_r[idx1++];
        }
        while(idx2 <= end){
            record[idx++] = position_r[idx2++];
        }
        for(int i = start; i <= end; i++)
            position_r[i] = record[i];
    }
}

int main(int argc, char *argv[]) {
    generator.init();
    int t = generator.getT();
    while (t--) {
        int n, *p, *q, *r;
        generator.getData(&n, &p, &q, &r);
        for(int i = 0; i < n; i++){
            position[i].top = p[i];
            if(q[i] > r[i]){
                position[i].left = r[i];
                position[i].right = q[i];
            }
            else{
                position[i].left = q[i];
                position[i].right = r[i];
            }
        }
        qsort(position, n, sizeof(Tri), cmp);
        for(int i = 0; i < n; i++){
            position_l[i] = position[i].left;
            position_r[i] = position[i].right;
        }
        mergeSort(0, n - 1);
        printf("%ld\n", ans);
        ans = 0;
    }
}
