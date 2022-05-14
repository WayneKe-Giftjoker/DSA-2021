#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAXROAD 1024

typedef struct node{
    int label;
    struct node *nxt, *prev;
} Node;

typedef struct linkedList{
    struct node *head, *tail;
    int length;
} LinkedList;

Node *genNode(int label){
    Node *new = (Node *)malloc(sizeof(Node));
    new->label = label;
    new->nxt = new->prev = NULL;
    return new;
}

LinkedList map[1<<10];
int road_num;

void enter(int rail, int label){
    Node *new = genNode(label);
    if(map[rail].length == 0){
        map[rail].head = new;
        map[rail].tail = new;
        map[rail].length++;
        // printf("===Entered (first)===\n");
        return;
    }
    map[rail].tail->nxt = new;
    new->prev = map[rail].tail;
    map[rail].tail = new;
    map[rail].length++;
    // printf("===Entered===\n");
}

void leave(int rail){
    if(map[rail].length == 0){
        // printf("===Left (nothing)===\n");
        return;
    }
    else if(map[rail].length == 1){
        free(map[rail].head);
        map[rail].head = map[rail].tail = NULL;
        map[rail].length--;
        return;
    }
    Node *new_tail = map[rail].tail->prev;
    if(new_tail->nxt == map[rail].tail){//regular
        new_tail->nxt = NULL;
    }
    else{
        new_tail->prev = new_tail->nxt;
        new_tail->nxt = NULL;
    }
    free(map[rail].tail);
    map[rail].tail = new_tail;
    map[rail].length--;
    // printf("===Left===\n");
}

void migrate(int rail_src, int rail_des){//維護 head 跟 tail 的 prev 跟 nxt 是正確的方向?
    if(map[rail_src].length == 0){
        // printf("===Migrated (nothing)===\n");
        return;
    }
    if(map[rail_src].length == 1){
        if(map[rail_des].length != 0){
            map[rail_des].tail->nxt = map[rail_src].head;
            map[rail_src].head->prev = map[rail_des].tail;
            map[rail_des].tail = map[rail_src].head;
        }
        else
            map[rail_des].head = map[rail_des].tail = map[rail_src].head;
        map[rail_src].head = map[rail_src].tail = NULL;
        map[rail_des].length += map[rail_src].length;
        map[rail_src].length = 0;
        return;
    }
    if(map[rail_des].length != 0){
        map[rail_src].tail->nxt = map[rail_des].tail;
        map[rail_des].tail->nxt = map[rail_src].tail;
        map[rail_src].head->prev = map[rail_src].head->nxt;
        map[rail_src].head->nxt = NULL;
        map[rail_des].tail = map[rail_src].head;
    }
    else{
        map[rail_src].head->prev = map[rail_src].head->nxt;
        map[rail_src].head->nxt = NULL;
        map[rail_src].tail->nxt = map[rail_src].tail->prev;
        map[rail_src].tail->prev = NULL;
        map[rail_des].head = map[rail_src].tail;
        map[rail_des].tail = map[rail_src].head;
    }
    map[rail_src].head = map[rail_src].tail = NULL;
    map[rail_des].length += map[rail_src].length;
    map[rail_src].length = 0;
    // printf("===Migrated===\n");
}

void printMap(){
    for(int road_indx = 0; road_indx < road_num; road_indx++){
        if(map[road_indx].length == 0){
            printf("\n");
            continue;
        }
        Node *prev = NULL;
        Node *curr = map[road_indx].head;
        for(int cabin_indx = 0; cabin_indx < map[road_indx].length; cabin_indx++){
            if(cabin_indx != map[road_indx].length - 1)
                printf("%d ", curr->label);
            else{
                printf("%d\n", curr->label);
                continue;
            }
            if(prev == NULL){
                prev = curr;
                curr = curr->nxt;
                continue;
            }
            if(curr->nxt != prev){
                prev = curr;
                curr = curr->nxt;
            }
            else{
                prev = curr;
                curr = curr->prev;
            }
        }
    }
}

int main(int argc, char *argv[]){
    int log_num;
    scanf("%d %d", &road_num, &log_num);
    for(int i = 0; i < log_num; i++){
        char command[16];
        int rail;
        scanf("%s %d", command, &rail);
        if(strcmp(command, "enter") == 0){
            int label;
            scanf("%d", &label);
            enter(rail, label);
        }
        else if(strcmp(command, "leave") == 0){
            leave(rail);
        }
        else if(strcmp(command, "migrate") == 0){
            int rail_des;
            scanf("%d", &rail_des);
            migrate(rail, rail_des);
        }
    }
    // printf("===Done===\n");
    printMap();
    // printf("===Print Done===\n");
    
    return 0;
}