#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_M 100
#define MAX_N 100
#define MAX_PATH (MAX_M * MAX_N)

typedef struct {
    int x, y;
    char from; // L = left, T = top
    char path[MAX_PATH];
    int letters;
    int chnages_of_dir;
} Robot;

typedef struct {
    char pathX[MAX_PATH];
    int num;
} path_struct;

int M, N, k_changes;
char game_plan[MAX_M][MAX_N];

path_struct paths[MAX_PATH];
int num_of_paths = 0;

int path_finding(Robot robot);

void load_input() {
    scanf("%d %d %d", &M, &N, &k_changes);
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            scanf(" %c", &game_plan[i][j]);
}

void record_path(Robot *r) {
    r->path[r->letters] = '\0';

    for (int i = 0; i < num_of_paths; i++) {
        if (strcmp(paths[i].pathX, r->path) == 0) {
            paths[i].num++;
            return;
        }
    }

    strcpy(paths[num_of_paths].pathX, r->path);
    paths[num_of_paths].num = 1;
    num_of_paths++;
}

void move_robot(Robot r, char new_dir, int dx, int dy) {
    if (r.letters >= MAX_PATH - 2)
        return;

    r.path[r.letters++] = game_plan[r.x][r.y];

    if (new_dir != r.from)
        r.chnages_of_dir++;

    r.from = new_dir;
    r.x += dx;
    r.y += dy;

    path_finding(r);
}

int path_finding(Robot robot) {
    if (robot.letters >= MAX_PATH - 1)
        return 0;

    if (robot.x == M - 1 && robot.y == N - 1) {
        if (robot.chnages_of_dir == k_changes)
            record_path(&robot);
        return 0;
    }

    if (robot.from == 'L') {
        if (robot.x + 1 < M && robot.chnages_of_dir < k_changes)
            move_robot(robot, 'T', 1, 0);

        if (robot.y + 1 < N)
            move_robot(robot, 'L', 0, 1);

    } else if (robot.from == 'T') {
        if (robot.y + 1 < N && robot.chnages_of_dir < k_changes)
            move_robot(robot, 'L', 0, 1);

        if (robot.x + 1 < M)
            move_robot(robot, 'T', 1, 0);
    }

    return 0;
}

int main() {
    load_input();

    Robot r = {0};

    r.x = 0; r.y = 1; r.from = 'L';
    path_finding(r);

    r.x = 1; r.y = 0; r.from = 'T';
    path_finding(r);

    int max = 0;
    for (int i = 0; i < num_of_paths; i++)
        if (paths[i].num > max)
            max = paths[i].num;

    for (int i = 0; i < num_of_paths; i++)
        if (paths[i].num == max)
            printf("%s\n", paths[i].pathX);

    return 0;
}
