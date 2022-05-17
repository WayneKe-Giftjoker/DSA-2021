#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define MAXNODE 343
#define MAXNUM 1225

typedef struct node{
    int array[MAXNUM];
    int *sorted;
    struct node *nxt;
    int is_reversed;
    int data_num;
} Node;

typedef struct unrolledlinkedlist{
    Node *head;
    int node_num, data_num;
} ULList;

ULList list;

int cmp(const void *a, const void *b){
    return (*(int *)a - *(int *)b);
}

void initList(){
    list.data_num = list.node_num = 0;
    list.head = NULL;
}

Node *genNode(){
    Node *new = (Node *)malloc(sizeof(Node));
    new->data_num = 0;
    new->is_reversed = 0;
    new->sorted = (int *)malloc(MAXNUM * sizeof(int));
    new->nxt = NULL;
    return new;
}

int rootOf(int num){
    return (int)sqrt(num);
}

int biSearch_q(int data, Node *curr, int start, int end){
    if(start == end){
        if(start == curr->data_num - 1 && data > curr->sorted[start])
            return start + 1;
        return start;
    }
    int mid = (start + end) / 2;
    if(data > curr->sorted[mid])
        return biSearch_q(data, curr, mid + 1, end);
    else
        return biSearch_q(data, curr, start, mid);
}

void split(Node *n, int cut){
    if(cut == 0 || cut >= n->data_num)
        return;
    Node *original_nxt = n->nxt;
    Node *new = genNode();
    for(int i = cut; i < n->data_num; i++)
        new->array[i - cut] = n->array[i];
    new->data_num = n->data_num - cut;
    n->data_num -= new->data_num;
    for(int i = 0; i < new->data_num; i++)
        new->sorted[i] = new->array[i];
    for(int i = 0; i < n->data_num; i++)
        n->sorted[i] = n->array[i];
    qsort(n->sorted, n->data_num, sizeof(int), cmp);
    qsort(new->sorted, new->data_num, sizeof(int), cmp);
    n->nxt = new;
    new->nxt = original_nxt;
    list.node_num++;
}

void freeList(){
    Node *ptr = list.head;
    while(ptr != NULL){
        Node *next = ptr->nxt;
        free(ptr->sorted);
        free(ptr);
        ptr = next;
    }
}

void rebuild(){
    Node *ptr = list.head;
    int all[100010], total_data = 0;
    while(ptr != NULL){
        if(!ptr->is_reversed){
            for(int i = 0; i < ptr->data_num; i++)
                all[total_data++] = ptr->array[i];
        }
        else{
            for(int i = ptr->data_num - 1; i >= 0; i--)
                all[total_data++] = ptr->array[i];
        }
        ptr = ptr->nxt;
    }
    Node *new_head = genNode();
    Node *new_ptr = new_head;
    int all_indx = 0, node_num = 1, new_data_num = total_data;
    for(int indx = 0; (indx + MAXNUM / 2) < total_data; indx += MAXNUM / 2){
        for(int i = 0; i < MAXNUM / 2; i++){
            new_ptr->array[i] = new_ptr->sorted[i] = all[all_indx++];
        }
        qsort(new_ptr->sorted, MAXNUM / 2, sizeof(int), cmp);
        new_ptr->data_num = MAXNUM / 2;
        new_ptr->nxt = genNode();
        node_num++;
        new_ptr = new_ptr->nxt;
    }
    if(all_indx < total_data){
        int leftover = total_data - all_indx;
        for(int i = 0; i < leftover; i++)
            new_ptr->array[i] = new_ptr->sorted[i] = all[all_indx++];
        qsort(new_ptr->sorted, leftover, sizeof(int), cmp);
        new_ptr->data_num = leftover;
        new_ptr->nxt = NULL;
    }
    freeList();
    list.head = new_head;
    list.data_num = new_data_num;
    list.node_num = node_num;
}

void printList(){
    Node *ptr = list.head;
    int node_idx = 0;
    while(ptr != NULL){
        printf("=====Node: %d=====\n", node_idx);
        printf("====Data_num: %d====\n", ptr->data_num);
        printf("====Array ");
        if(!ptr->is_reversed){
            for(int i = 0; i < ptr->data_num; i++)
                printf("%d ", ptr->array[i]);
        }
        else{
            for(int i = ptr->data_num - 1; i >= 0; i--)
                printf("%d ", ptr->array[i]);
        }
        printf("====\n====Sorted ");
        for(int i = 0; i < ptr->data_num; i++)
            printf("%d ", ptr->sorted[i]);
        printf("====\n");
        node_idx++;
        ptr = ptr->nxt;
    }
}

void insert(int pos, int data){
    if(list.head == NULL){
        list.head = genNode();
        list.head->array[0] = data;
        list.head->sorted[0] = data;
        list.head->data_num++;
        list.data_num++;
        list.node_num++;
        return;
    }
    Node *ptr = list.head;
    int curr_idx = 0;
    while(ptr->nxt != NULL){
        if((curr_idx + ptr->data_num) >= pos)
            break;
        curr_idx += ptr->data_num;
        ptr = ptr->nxt;
    }
    if(ptr->is_reversed){
        for(int i = 0; i < ptr->data_num / 2; i++){
            int temp = ptr->array[i];
            ptr->array[i] = ptr->array[ptr->data_num - 1 - i];
            ptr->array[ptr->data_num - 1 - i] = temp;
        }
        ptr->is_reversed = 0;
    }
    ptr->sorted[ptr->data_num] = data;
    qsort(ptr->sorted, ptr->data_num + 1, sizeof(int), cmp);
    for(int i = pos - curr_idx; i < ptr->data_num + 1; i++){
        int temp = ptr->array[i];
        ptr->array[i] = data;
        data = temp;
    }
    ptr->data_num++;
    list.data_num++;
    if(ptr->data_num >= MAXNUM)
        split(ptr, ptr->data_num / 2);
    if(list.node_num > MAXNODE)
        rebuild();
}

void delete(int pos){
    Node *ptr = list.head, *prev = list.head;
    int curr_indx = 0;
    while(ptr != NULL){
        if((curr_indx + ptr->data_num) > pos)
            break;
        curr_indx += ptr->data_num;
        prev = ptr;
        ptr = ptr->nxt;
    }
    if(ptr->is_reversed){
        for(int i = 0; i < ptr->data_num / 2; i++){
            int temp = ptr->array[i];
            ptr->array[i] = ptr->array[ptr->data_num - 1 - i];
            ptr->array[ptr->data_num - 1 - i] = temp;
        }
        ptr->is_reversed = 0;
    }
    if(ptr->data_num == 1){
        free(ptr->sorted);
        if(ptr == list.head){
            list.head = ptr->nxt;
            free(ptr);
        }
        else{
            Node *next = ptr->nxt;
            free(ptr);
            prev->nxt = next;
        }
        list.node_num--;
        list.data_num--;
        return;
    }
    else{
        for(int i = pos - curr_indx; i < ptr->data_num - 1; i++)//-1?
            ptr->array[i] = ptr->array[i + 1];
        for(int i = 0; i < ptr->data_num - 1; i++)
            ptr->sorted[i] = ptr->array[i];
        qsort(ptr->sorted, ptr->data_num - 1, sizeof(int), cmp);
        ptr->data_num--;
        list.data_num--;
    }
    if(ptr->data_num >= MAXNUM)
        split(ptr, ptr->data_num / 2);
    if(list.node_num > MAXNODE)
        rebuild();
}

void reverse(int start, int end){
    Node *left_ptr = list.head, *right_ptr = list.head, *left_prev = list.head;
    int left_curr_idx = 0, right_curr_idx = 0, complete_node = -1;
    while(left_ptr != NULL){
        if((left_curr_idx + left_ptr->data_num) > start){
            right_ptr = left_ptr;
            right_curr_idx = left_curr_idx;
            break;
        }
        left_curr_idx += left_ptr->data_num;
        left_prev = left_ptr;
        left_ptr = left_ptr->nxt;
    }
    while(right_ptr != NULL){
        if((right_curr_idx + right_ptr->data_num) > end)
            break;
        complete_node++;
        right_curr_idx += right_ptr->data_num;
        right_ptr = right_ptr->nxt;
    }
    if(left_ptr == right_ptr){
        if(left_ptr->is_reversed){
            for(int i = 0; i < left_ptr->data_num / 2; i++){
                int temp = left_ptr->array[i];
                left_ptr->array[i] = left_ptr->array[left_ptr->data_num - 1 - i];
                left_ptr->array[left_ptr->data_num - 1 - i] = temp;
            }
            left_ptr->is_reversed = 0;
        }
        int start_prum = start - left_curr_idx, end_prum = end - left_curr_idx;
        for(int i = start_prum; i < (start_prum + end_prum + 1) / 2; i++){
            int temp = left_ptr->array[i];
            left_ptr->array[i] = left_ptr->array[end_prum - i + start_prum];
            left_ptr->array[end_prum - i + start_prum] = temp;
        }
        return;
    }
    else if(complete_node >= 0){
        if(left_ptr->is_reversed){
            for(int i = 0; i < left_ptr->data_num / 2; i++){
                int temp = left_ptr->array[i];
                left_ptr->array[i] = left_ptr->array[left_ptr->data_num - 1 - i];
                left_ptr->array[left_ptr->data_num - 1 - i] = temp;
            }
            left_ptr->is_reversed = 0;
        }
        if(right_ptr->is_reversed){
            for(int i = 0; i < right_ptr->data_num / 2; i++){
                int temp = right_ptr->array[i];
                right_ptr->array[i] = right_ptr->array[right_ptr->data_num - 1 - i];
                right_ptr->array[right_ptr->data_num - 1 - i] = temp;
            }
            right_ptr->is_reversed = 0;
        }
        split(left_ptr, start - left_curr_idx);
        split(right_ptr, end - right_curr_idx + 1);
        Node *reverse_start_ptr, *reverse_end_ptr;
        reverse_start_ptr = (start == left_curr_idx)? left_ptr : left_ptr->nxt;
        reverse_end_ptr = right_ptr->nxt;
        Node *ptr = reverse_start_ptr, *prev = (reverse_start_ptr == left_ptr)? left_prev : left_ptr, *next;
        Node *prev_cpy = prev;
        while(ptr != reverse_end_ptr){
            ptr->is_reversed = (ptr->is_reversed == 1)? 0 : 1;
            next = ptr->nxt;
            ptr->nxt = prev;
            prev = ptr;
            ptr = next;
        }
        if(prev != prev_cpy){
            if(reverse_start_ptr == left_ptr)
                left_prev->nxt = prev;
            else
                left_ptr->nxt = prev;
            reverse_start_ptr->nxt = reverse_end_ptr;
        }
        if(reverse_start_ptr == list.head)
            list.head = prev;
    }
    if(list.node_num > MAXNODE)
        rebuild();
}

int queryHelper(int data, Node *left, Node *right, int start, int end){//start: left_node->array starting indx; end: right_node->array neding indx
    int count = 0;
    Node *ptr = left->nxt;
    if(left == right){
        if(left->is_reversed){
            for(int i = 0; i < left->data_num / 2; i++){
                int temp = left->array[i];
                left->array[i] = left->array[left->data_num - 1 - i];
                left->array[left->data_num - 1 - i] = temp;
            }
            left->is_reversed = 0;
        }
        for(int i = start; i <= end; i++){
            if(left->array[i] < data)
                count++;
        }
        return count;
    }
    if(!left->is_reversed){
        for(int i = start; i < left->data_num; i++)
            if(left->array[i] < data)
                count++;
    }
    else{
        for(int i = left->data_num - 1 - start; i >= 0; i--)
            if(left->array[i] < data)
                count++;
    }
    if(!right->is_reversed){
        for(int i = 0; i <= end; i++)
            if(right->array[i] < data)
                count++;
    }
    else{
        for(int i = right->data_num - 1; i >= right->data_num - 1 - end; i--)
            if(right->array[i] < data)
                count++;
    }
    while(ptr != right){
        count += biSearch_q(data, ptr, 0, ptr->data_num - 1);
        ptr = ptr->nxt;
    }
    return count;
}

int query(Node *left, Node *right, int start, int end, int k, int left_bound, int right_bound){//start: left_array_indx, not all_indx; end: right_array_indx, not all_indx
    int mid = (left_bound + right_bound + 1) / 2;
    if(left_bound == right_bound)
        return left_bound;
    int num = queryHelper(mid, left, right, start, end);
    if(num > k - 1 ){
        return query(left, right, start, end, k, left_bound, mid - 1);
    }
    else if(num <= k - 1){
        return query(left, right, start, end, k, mid, right_bound);
    }
}


int main(int argc, char *argv[]){
    initList();
    int init_num, cmd_num;
    scanf("%d %d", &init_num, &cmd_num);
    for(int i = 0; i < init_num; i++){
        int data;
        scanf("%d", &data);
        insert(i, data);
    }
    char cmd[16];
    for(int i = 0; i < cmd_num; i++){
        scanf("%s", cmd);
        if(strcmp(cmd, "Insert") == 0){
            int pos, data;
            scanf("%d %d", &pos, &data);
            insert(pos - 1, data);
        }
        else if(strcmp(cmd, "Delete") == 0){
            int pos;
            scanf("%d", &pos);
            delete(pos - 1);
        }
        else if(strcmp(cmd, "Reverse") == 0){
            int l, r;
            scanf("%d %d", &l, &r);
            reverse(l - 1, r - 1);
        }
        else if(strcmp(cmd, "Query") == 0){
            int l, r, k;
            scanf("%d %d %d", &l, &r, &k);
            Node *left = list.head, *right;
            int curr_idx = 0;
            while(left != NULL){
                if((curr_idx + left->data_num) > l - 1)
                    break;
                curr_idx += left->data_num;
                left = left->nxt;
            }
            int l_prum = l - curr_idx - 1;
            right = left;
            while(right != NULL){
                if((curr_idx + right->data_num) > r - 1)
                    break;
                curr_idx += right->data_num;
                right = right->nxt;
            }
            int r_prum = r - curr_idx - 1;
            printf("%d\n", query(left, right, l_prum, r_prum, k, -100000, 100000));
        }
    }
    return 0;
}
