#include <stdio.h>

#define MAX_TRAINS 30

int num_trains;

typedef struct {
    int id;
    int weight;
    int infront[MAX_TRAINS];
} Train;

Train all_trains[MAX_TRAINS];

void load_input() {
    fscanf(stdin, "%d\n", &num_trains);

    for (int idx = 0; idx < num_trains; idx++) {
        fscanf(stdin, "%d %d", &all_trains[idx].id, &all_trains[idx].weight);
        char c = 'a';

        for (int idy = 0; idy < MAX_TRAINS; idy++) {
            fscanf(stdin, "%d%c", &all_trains[idx].infront[idy], &c);
            if (c == '\n') {
                fprintf(stdout, "BREAK\n");
                break;
            }
        }
    }
}


int main () {
    load_input();

    fprintf(stdout, "Number of trains: %d\n", num_trains);
    
    for (int idx = 0; idx < num_trains; idx++) {
        fprintf(stdout, "train ID: %d train WEIGHT: %d\n", all_trains[idx].id, all_trains[idx].weight);
        
        for (int idy = 0; idy < num_trains; idy++) {
            fprintf(stdout, "trains BEFORE: %d ", all_trains[idx].infront[idy]);
        }
        fprintf(stdout, "\n");
    }

    return 0;
}