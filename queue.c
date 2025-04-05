#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#define VISITED_SIZE 100003

void enqueue(struct queue *q, struct game_state state) {
    uint64_t ser = serialize(state);
    insert_at_tail(&q->data, (size_t)ser);
}

struct game_state dequeue(struct queue *q) { 
    uint64_t ser = remove_from_head(&q->data);
    return deserialize(ser);
}

int number_of_moves(struct game_state start) {
    int goal[4][4] = {
        {1,  2,  3,  4},
        {5,  6,  7,  8},
        {9,  10, 11, 12},
        {13, 14, 15, 0}
    };
    //check if start state is goal
    int is_goal = 1;
    for (int i = 0; i < 4 && is_goal; i++) {
        for (int j = 0; j < 4; j++) {
            if (start.tiles[i][j] != goal[i][j]) {
                is_goal = 0;
                break;
            }
        }
    }
    if (is_goal){
        return start.num_steps;
    }
    struct queue q;
    q.data.head=NULL;

    uint64_t *visited_table = calloc(VISITED_SIZE, sizeof(uint64_t));
    char *visited_flag = calloc(VISITED_SIZE, sizeof(char));
    //mark start state  as visited
    uint64_t start_ser = serialize(start);
    int idx = start_ser % VISITED_SIZE;
    while (visited_flag[idx]){
        idx = (idx + 1) % VISITED_SIZE;
    }
    visited_table[idx] = start_ser;
    visited_flag[idx] = 1;

    enqueue(&q,start);

    //BFS
    while (q.data.head != NULL){
        struct game_state cur = dequeue(&q);
        int cur_is_goal = 1;
        for(int i =0; i<4 && cur_is_goal; i++){
            for(int j =0; j<4; j++){
                if (cur.tiles[i][j] != goal[i][j]){
                    cur_is_goal = 0;
                    break;
                }
            }
        }
        if (cur_is_goal){
            int moves = cur.num_steps;
            free(visited_table);
            free(visited_flag);
            return moves;

        }

        //move up
        //valid if empty_row <3
        if(cur.empty_row <3){
            struct game_state next = cur;
            move_up(&next);
            uint64_t ser = serialize(next);
            int found = 0;
            idx = ser % VISITED_SIZE;
            while(visited_flag[idx]){
                if (visited_table[idx] == ser){
                    found = 1;
                    break;
                }
                idx = (idx+1)%VISITED_SIZE;
            }
            if(!found){
                visited_table[idx] = ser;
                visited_flag[idx] = 1;
                enqueue(&q, next);
            }
        }
        //move down
        //valid if emptyrow >0
        if(cur.empty_row>0){
            struct game_state next = cur;
            move_down(&next);
            uint64_t ser = serialize(next);
            int found = 0;
            idx = ser % VISITED_SIZE;
            while (visited_flag[idx]){
                if(visited_table[idx]==ser){
                    found = 1;
                    break;
                }
                idx = (idx+1) % VISITED_SIZE;
            }
            if (!found){
                visited_table[idx] = ser;
                visited_flag[idx] = 1;
                enqueue(&q, next);
            }

        }
        //move left
        // valid if empty_col <3
        if(cur.empty_col <3){
            struct game_state next = cur;
            move_left(&next);
            uint64_t ser = serialize(next);
            int found = 0;
            idx = ser%VISITED_SIZE;
            while (visited_flag[idx]){
                if(visited_table[idx] == ser){
                    found = 1;
                    break;
                }
                idx = (idx+1) %VISITED_SIZE;
            }
            if(!found){
                visited_table[idx] = ser;
                visited_flag[idx] =1;
                enqueue(&q, next);
            }
        }
        //move rigjt if emptycol >0
        if(cur.empty_col <3){
            struct game_state next = cur;
            move_right(&next);
            uint64_t ser = serialize(next);
            int found = 0;
            idx = ser%VISITED_SIZE;
            while (visited_flag[idx]){
                if(visited_table[idx] == ser){
                    found = 1;
                    break;
                }
                idx = (idx+1) %VISITED_SIZE;
            }
            if(!found){
                visited_table[idx] = ser;
                visited_flag[idx] =1;
                enqueue(&q, next);
            }
        }
    }
    free(visited_table);
    free(visited_flag);
    return -1; //no solution found

}
