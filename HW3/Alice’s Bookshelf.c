#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
//Thanks to B06303131 沈家睿 for helping debugging & discussing!!!

typedef struct node{
    int prio, cnt;//prio -> for treap //cnt -> how many nodes in subtree
    int rev, add;
    int priority, max;// priority -> for bookself
    struct node *l, *r;
} Node;

Node *genNode(int priority, int prio){
    Node *new = (Node *)malloc(sizeof(Node));
    new->priority = new->max = priority;
    new->prio = prio;
    new->add = new->rev = 0;
    new->cnt = 1;
    new->l = new->r = NULL;
    return new;
}

void push(Node *n){
    if(n != NULL){
        if(n->rev){
            Node *tmp = n->l;
            n->l = n->r;
            n->r = tmp;
            n->rev = 0;
            if(n->l != NULL) n->l->rev ^= 1;
            if(n->r != NULL) n->r->rev ^= 1;
        }
        if(n->add != 0){
            n->priority += n->add;
            n->max += n->add;
            if(n->l != NULL) n->l->add += n->add;
            if(n->r != NULL) n->r->add += n->add;
            n->add = 0;
        }
    }
}

void update(Node *n){
    if(n != NULL){
        int l_cnt = (n->l != NULL)? n->l->cnt : 0;
        long max = n->priority + n->add;
        if(n->l != NULL){
            if(n->l->max + n->l->add > max){
                max = n->l->max + n->l->add;
            }
        }
        if(n->r != NULL){
            if(n->r->max + n->r->add > max){
                max = n->r->max + n->r->add;
            }
        }
        n->max = max;
        int r_cnt = (n->r != NULL)? n->r->cnt : 0;
        n->cnt = l_cnt + r_cnt + 1;
    }
}

void output(Node *root){
    if(root == NULL)
        return;
    push(root);
    output(root->l);
    printf("cnt: %d || priority: %d || max: %d || add: %d\n", root->cnt, root->priority, root->max, root->add);
    output(root->r);
}

void split(Node *root, Node **l, Node **r, int key){
    if(root == NULL){
        *l = *r = NULL;
        return;
    }
    push(root);
    int l_cnt = (root->l != NULL)? root->l->cnt : 0;
    if(key <= l_cnt){
        *r = root;
        split(root->l, l, &((*r)->l), key);
    }
    else{
        *l = root;
        split(root->r, &((*l)->r), r, key - l_cnt - 1);
    }
    update(root);
}

Node *merge(Node *l, Node *r){
    if(l == NULL || r == NULL)
        return (l != NULL)? l : r;
    if(l->prio < r->prio){
        push(l);
        l->r = merge(l->r, r);
        update(l);
        return l;
    }
    else{
        push(r);
        r->l = merge(l, r->l);
        update(r);
        return r;
    }
}

void insert(Node **root, int priority, int prio, int pos){
    if(root == NULL){
        *root = genNode(priority, prio);
        return;
    }
    Node *l, *r, *mid;
    l = r = NULL;
    mid = genNode(priority, prio);
    split(*root, &l, &r, pos);
    l = merge(l, mid);
    *root = merge(l, r);
}

void erase_pos(Node **root, int key){
    Node *l, *r, *mid;
    l = r = mid = NULL;
    split(*root, &l, &r, key);
    split(l, &l, &mid, key - 1);
    push(mid);
    *root = merge(l, r);
}

void reverse(Node **root, int l, int r){
    Node *t1, *t2, *t3;
    split(*root, &t1, &t2, l - 1);
    split(t2, &t2, &t3, r - l + 1);
    t2->rev ^= 1;
    *root = merge(t1, t2);
    *root = merge(*root, t3);
}

void paint(Node **root, int lB, int rB, int add){
    Node *l, *r, *mid;
    split(*root, &l, &mid, lB - 1);
    split(mid, &mid, &r, rB - lB + 1);
    mid->add += add;
    r = merge(mid, r);
    *root = merge(l, r);
}

int query(Node **root, int lB, int rB){
    Node *l, *r, *mid;
    // l = r = mid = NULL;
    split(*root, &l, &r, lB - 1);
    split(r, &mid, &r, rB - lB + 1);
    push(mid);
    int ans = mid->max;
    r = merge(mid, r);
    *root = merge(l, r);
    return ans;
}

void erase_Max(Node **root, int N){
    // int max = query(root, 1, N);
    Node *ptr = *root, *l = (*root)->l, *r = (*root)->r;
    int pos = 1;
    push(ptr);
    int max = ptr->max;
    for(;;){
        push(ptr->l), push(ptr->r);
        int l_max = (ptr->l != NULL)? ptr->l->max : 1<<31;
		int r_max = (ptr->r != NULL)? ptr->r->max : 1<<31;
        if(ptr->priority == max && l_max != max){
            break;
        }
        if(l_max == max){
            ptr = ptr->l;
		}
        else{
			pos += (ptr->l != NULL)? (ptr->l->cnt + 1) : 1;
            ptr = ptr->r;
		}
    }
    pos += (ptr->l != NULL)? ptr->l->cnt : 0;
    erase_pos(root, pos);
}

int main(int argc, char *argv[]){
    int N, Q;
    scanf("%d %d", &N, &Q);
    Node *root;
    root = NULL;
	srand(1115);
    for(int i = 0; i < N; i++){
        int priority;
        scanf("%d", &priority);
        insert(&root, priority, rand(), i);
    }
    while(Q--){
        int cmd;
        scanf("%d", &cmd);
        int priority;
        int k;
        int l, r;
        switch (cmd){
        case 1:
            scanf("%d %d", &priority, &k);
            insert(&root, priority, rand(), k);
            break;
        case 2:
            scanf("%d", &k);
            erase_pos(&root, k);
            break;
        case 3:
            scanf("%d %d %d", &l, &r, &priority);
            paint(&root, l, r, priority);
            break;
        case 4:
            scanf("%d %d", &l, &r);
            printf("%d\n", query(&root, l, r));
            break;
        case 5:
            scanf("%d %d", &l, &r);
            reverse(&root, l, r);
            break;
        case 6:
            erase_Max(&root, root->cnt);
            break;
        default:
            break;
        }
    }
    return 0;
}
