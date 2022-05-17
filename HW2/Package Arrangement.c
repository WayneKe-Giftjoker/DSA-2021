#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
#define MAX 100100

typedef struct node{
    int data, dist;
    struct node *deque_p, *deque_n, *leftist_l, *leftist_r;
} Node;

typedef struct line{
    Node *head, *tail, *max;
} Line;

typedef struct cmd{
    char type;
    int n1, n2;
} Cmd;

Node *genNode(){
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = new->dist = 0;
    new->deque_n = new->deque_p = new->leftist_l = new->leftist_l = new->leftist_r = NULL;
    return new;
}

Line rails[MAX];
int pack_num, line_num;
Cmd oper[2*MAX];
int ling[MAX], ling_idx;
int record[MAX];
int where[MAX];

void freeLine(int line){
    rails[line].max = NULL;
    rails[line].tail = NULL;
    if(rails[line].head == NULL)
        return;
    Node *temp = rails[line].head;
    rails[line].head = rails[line].head->deque_n;
    free(temp);
    freeLine(line);
}

Node *mergeLeftist(Node *root1, Node *root2){
    if(root1 == NULL)
        return root2;
    if(root2 == NULL)
        return root1;
    if(root1->data > root2->data){
        if(root1->leftist_l == NULL)
            root1->leftist_l = root2;
        else{
            root1->leftist_r = mergeLeftist(root1->leftist_r, root2);
            if(root1->leftist_l->dist < root1->leftist_r->dist){
                Node *temp = root1->leftist_l;
                root1->leftist_l = root1->leftist_r;
                root1->leftist_r = temp;
            }
            root1->dist = root1->leftist_r->dist + 1;
        }
        return root1;
    }
    else{
        if(root2->leftist_l == NULL)
            root2->leftist_l = root1;
        else{
            root2->leftist_r = mergeLeftist(root2->leftist_r, root1);
            if(root2->leftist_l->dist < root2->leftist_r->dist){
                Node *temp = root2->leftist_l;
                root2->leftist_l = root2->leftist_r;
                root2->leftist_r = temp;
            }
            root2->dist = root2->leftist_r->dist + 1;
        }
        return root2;
    }
}

void push(int height, int line_idx){
    if(rails[line_idx].head == NULL){
        rails[line_idx].head = genNode();
        rails[line_idx].tail = rails[line_idx].head;
        rails[line_idx].head->data = height;
        rails[line_idx].max = rails[line_idx].head;
        return;
    }
    Node *new_pack = genNode();
    new_pack->data = height;
    rails[line_idx].max = mergeLeftist(new_pack, rails[line_idx].max);
    rails[line_idx].tail->deque_n = new_pack;
    new_pack->deque_p = rails[line_idx].tail;
    rails[line_idx].tail = new_pack;
    return;
}

void merge(int line1, int line2){
    if(rails[line1].head == NULL)
        return;
    if(rails[line2].head == NULL){
        where[rails[line1].head->data] = where[rails[line1].tail->data] = where[rails[line1].max->data] = line2;
        rails[line2].head = rails[line1].head;
        rails[line2].tail = rails[line1].tail;
        rails[line2].max = rails[line1].max;
        rails[line1].head = rails[line1].max = rails[line1].tail = NULL;
        return;
    }
    where[rails[line1].head->data] = where[rails[line1].tail->data] = where[rails[line1].max->data] = line2;
    rails[line2].tail->deque_n = rails[line1].head;
    rails[line1].head->deque_p = rails[line2].tail;
    rails[line2].tail = rails[line1].tail;
    rails[line2].max = mergeLeftist(rails[line2].max, rails[line1].max);
    rails[line1].head = rails[line1].max = rails[line1].tail = NULL;
    return;
}

void pop(int line, char type){
    switch(type){
        case 'h':
            record[rails[line].head->data] = 1;
            while(rails[line].head != NULL && record[rails[line].head->data] == 1){
                while(rails[line].max != NULL && record[rails[line].max->data] == 1){
                    rails[line].max = mergeLeftist(rails[line].max->leftist_l, rails[line].max->leftist_r);
                }
                rails[line].head = rails[line].head->deque_n;
                if(rails[line].head == NULL){
                    rails[line].tail = rails[line].max = NULL;
                    return;
                }
                rails[line].head->deque_p = NULL;
            }
            where[rails[line].head->data] = where[rails[line].tail->data] = where[rails[line].max->data] = line;
            break;
        case 't':
            record[rails[line].tail->data] = 1;
            while(rails[line].tail != NULL && record[rails[line].tail->data] == 1){
                while(rails[line].max != NULL && record[rails[line].max->data] == 1){
                    rails[line].max = mergeLeftist(rails[line].max->leftist_l, rails[line].max->leftist_r);
                }
                rails[line].tail = rails[line].tail->deque_p;
                if(rails[line].tail == NULL){
                    rails[line].head = rails[line].max = NULL;
                    return;
                }
                rails[line].tail->deque_n = NULL;
            }
            where[rails[line].head->data] = where[rails[line].tail->data] = where[rails[line].max->data] = line;
            break;
        case 'm':
            record[rails[line].max->data] = 1;
            while(rails[line].max != NULL && record[rails[line].max->data] == 1){
                while(rails[line].head != NULL && record[rails[line].head->data] == 1){
                    rails[line].head = rails[line].head->deque_n;
                    if(rails[line].head == NULL){
                        rails[line].tail = rails[line].max = NULL;
                        return;
                    }
                    rails[line].head->deque_p = NULL;
                }
                if(rails[line].tail != NULL && record[rails[line].tail->data] == 1){
                    rails[line].tail = rails[line].tail->deque_p;
                    if(rails[line].tail == NULL){
                        rails[line].head = rails[line].max = NULL;
                        return;
                    }
                    rails[line].tail->deque_n = NULL;
                }
                rails[line].max = mergeLeftist(rails[line].max->leftist_l, rails[line].max->leftist_r);
            }
            where[rails[line].head->data] = where[rails[line].tail->data] = where[rails[line].max->data] = line;
            break;
    }
}

int main(int argc, char *argv[]){
    int T;
    scanf("%d", &T);
    int cmd_num;
    for(int i = 0; i < MAX; i++)
        where[i] = -1;
    while(T--){
        scanf("%d %d %d", &pack_num, &cmd_num, &line_num);
        char str[8];
        int n1, n2;
        for(int i = 0; i < cmd_num; i++){
            scanf("%s %d %d", str, &n1, &n2);
            oper[i].type = str[0];
            oper[i].n1 = n1;
            oper[i].n2 = n2;
        }
        for(int i = 0; i < pack_num; i++)
            scanf("%d", &ling[i]);
        for(int cmd_idx = 0; cmd_idx < cmd_num; cmd_idx++){
            if(oper[cmd_idx].type == 'p'){
                int height, line;
                height = oper[cmd_idx].n1;
                line = oper[cmd_idx].n2;
                where[height] = line;
                push(height, line);
                int target_line = where[ling[ling_idx]];
                while(target_line != -1 && rails[target_line].head != NULL && (rails[target_line].head->data == ling[ling_idx] || rails[target_line].tail->data == ling[ling_idx] || rails[target_line].max->data == ling[ling_idx])){
                    if(ling[ling_idx] == rails[target_line].head->data){
                        pop(target_line, 'h');
                    }
                    else if(ling[ling_idx] == rails[target_line].tail->data){
                        pop(target_line, 't');
                    }
                    else{
                        pop(target_line, 'm');
                    }
                    ling_idx++;
                    if(ling_idx == pack_num)
                        break;
                    target_line = where[ling[ling_idx]];
                }
            }
            else if(oper[cmd_idx].type == 'm'){
                int src = oper[cmd_idx].n1, des = oper[cmd_idx].n2;
                merge(src, des);
            }
        }
        if(ling_idx < pack_num)
            printf("impossible\n");
        else
            printf("possible\n");
        ling_idx = 0;
        for(int i = 1; i <= pack_num; i++){
            record[i] = 0;
            where[i] = -1;
        }
        for(int i = 0; i < line_num; i++)
            rails[i].head = rails[i].max = rails[i].tail = NULL;
    }
    
    return 0;
}
