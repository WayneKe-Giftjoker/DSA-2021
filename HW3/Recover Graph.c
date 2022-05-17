#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int edge_start[1<<18], edge_end[1<<18], edge_idx;
int edge_num;
// typedef struct edge{
//     int start, end;
// } Edge;

// Edge adj_new[1<<18];

int find(int **adj, int *adj_idx, int curr_list, int target_list){
    if(edge_idx > edge_num)
        return 1;
    int start = adj_idx[target_list], end = start;
    // adj_idx[curr_list] = adj[curr_list][0] + 1;
    while(end <= adj[target_list][0]){
        if(adj[target_list][end] == curr_list)
            break;
        end++;
    }
    adj_idx[target_list] = end + 1;
    if(end > adj[target_list][0])
        return 0;
    for(int i = start; i < end; i++){
        if(find(adj, adj_idx, target_list, adj[target_list][i]) == 0)
            return 0;
    }
    edge_start[edge_idx] = curr_list;
    edge_end[edge_idx] = target_list;
    edge_idx++;
    return 1;
}

int main(int argc, char *argv[]){
    int N;
    scanf("%d", &N);
    int **adj = (int **)malloc(sizeof(int *) * (N+1));
    for(int i = 1; i <= N; i++){
        int num;
        scanf("%d", &num);
        adj[i] = (int *)malloc(sizeof(int) * (num+1));
        adj[i][0] = num;
        edge_num += num;
        for(int j = 1; j <= num; j++)
            scanf("%d", &adj[i][j]);
    }
    edge_num /= 2;
    int *adj_idx = (int *)malloc(sizeof(int) * (N+1));
    for(int i = 1; i <= N; i++)
        adj_idx[i] = 1;
    int found = 1;
    for(int i = 1; i <= N && found; i++){
        int start = adj_idx[i];
        adj_idx[i] = adj[i][0] + 1;
        for(int j = start; j <= adj[i][0]; j++){
            found = find(adj, adj_idx, i, adj[i][j]);
            if(found == 0){
                printf("No\n");
                // for(int i = 0; i < edge_idx; i++)
                //     printf("%d %d\n", edge_start[i], edge_end[i]);
                break;
            }
        }
    }
    if(found != 0){
        printf("Yes\n");
        for(int i = 0; i < edge_idx; i++)
            printf("%d %d\n", edge_start[i], edge_end[i]);
    }
    
    return 0;
}
