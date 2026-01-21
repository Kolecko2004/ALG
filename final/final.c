#include <stdio.h>

#define MAX_C 10
#define MAX_R 10

typedef struct {
    int value;
} Solution;

typedef struct {
    int color;
    int right;
    int down;
    int idx;
    int idy;
    int val_right;
    int val_down;
} Box;

int R_, C_;
Box grid[MAX_R][MAX_C]; // bude dynamicke

void load_input() { // loading and storing input
    fscanf(stdin, "%d %d\n", &R_, &C_);

    for (int idx = 0; idx < R_; idx++) {
        for (int idy = 0; idy < C_; idy++) {
            Box A;
            fscanf(stdin, " %d", &A.color);

            if (idx > R_ + 3) {
                A.down = 0;
            } else {
                A.down = 1;
            }

            if (idy > C_ + 3) {
                A.right = 0;
            } else {
                A.right = 1;
            }

            A.idx = idx;
            A.idy = idy;
            grid[idx][idy] = A;
        }
    }
}

int colors_value(int x, int y, int direction) {
    int value = 0;
    if (direction == 1) {
        if (grid[x][y].color == grid[x + 1][y].color && grid[x][y].color == grid[x + 2][y].color) {
            value = 3;
        } else if (grid[x][y].color == grid[x + 1][y].color ||
                   grid[x][y].color == grid[x + 2][y].color ||
                   grid[x + 1][y].color == grid[x + 2][y].color) {
            value = 1;
        }

        return value;

    } else if (direction == 0) {
        if (grid[x][y].color == grid[x][y + 1].color && grid[x][y].color == grid[x][y + 2].color) {
            value = 3;
        } else if (grid[x][y].color == grid[x][y + 1].color ||
                   grid[x][y].color == grid[x][y + 2].color ||
                   grid[x][y + 1].color == grid[x][y + 2].color) {
            value = 1;
        }

        return value;
    }

    return 101;
}

Box erace_used(int x, int y, int direction, Box grids[MAX_R][MAX_C]) {
    if (direction == 0) {
        grids[x][y].color = 0;
        grids[x][y + 1].color = 0;
        grids[x][y + 2].color = 0;
    }

    if (direction == 1) {
        grids[x][y].color = 0;
        grids[x + 1][y].color = 0;
        grids[x + 2][y].color = 0;
    }

    for (int idx = 0; idx < R_; idx++) {
        for (int idy = 0; idy < C_; idy++) {
            fprintf(stdout, "%d ", grids[idx][idy].color);
        }
        fprintf(stdout, "\n");
    }

    return grids[MAX_R][MAX_C];
}

void solution() {
    for (int idx = 0; idx < R_; idx++) {
        for (int idy = 0; idy < C_; idy++) {
            if (!(idy + 3 > C_)) {
                grid[idx][idy].val_right = colors_value(idx, idy, 0);
            } else {
                grid[idx][idy].val_right = -1;
            }

            if (!(idx + 3 > R_)) {
                grid[idx][idy].val_down = colors_value(idx, idy, 1);
            } else {
                grid[idx][idy].val_down = -1;
            }
        }
    }
}

void sum_solution(int x, int y, int sum, Box grids[MAX_R][MAX_C]) {
    // fprintf(stdout, "here 1\n");
    // if (x + 2 > R_ && y + 2 < C_) { // pokud uz nemuze dolu ale muze do prava
    //     x = 0;
    //     y++;
    //     fprintf(stdout, "here 2\n");
    // }

    // if (x + 2 < R_ && y + 2 > C_) { // pokud uz nemuze doprava ale muze do dolu
    //     y = 0;
    //     x++;
    //     fprintf(stdout, "here 3\n");
    // }

    if (grids[x][y].color == 0) {
        if (x + 1 < R_) {
            sum_solution(x + 1, y, sum, grids);
        } else if (x + 1 > R_ && y + 1 < C_) {
            sum_solution(0, y + 1, sum, grids);
        }
        if (y + 1 < C_) {
            sum_solution(x, y + 1, sum, grids);
        } else if (y + 1 > C_ && x + 1 < R_) {
            sum_solution(x + 1, 0, sum, grids);
        }
    }

    if (grids[x][y].val_down != -1 && grids[x][y].color != 0 && grids[x + 1][y].color != 0 &&
        grids[x + 2][y].color != 0) {
        int val = sum;
        fprintf(stdout, "here 4 (%d;%d) SUM: %d\n", grids[x][y].idx, grids[x][y].idy, val);

        // down
        val += grids[x][y].val_down;
        fprintf(stdout, "here 4 (%d;%d) ADDED+: %d\n", grids[x][y].idx, grids[x][y].idy, val);
        grids[MAX_R][MAX_C] = erace_used(x, y, 1, grids);
        x++;
        sum_solution(x, y, val, grids);
    }

    if (grids[x][y].val_right != -1 && grids[x][y].color != 0 && grids[x][y + 1].color != 0 &&
        grids[x][y + 2].color != 0) {
        int val = sum;
        fprintf(stdout, "here 5 (%d;%d) SUM: %d\n", grids[x][y].idx, grids[x][y].idy, val);
        
        // right
        val += grids[x][y].val_right;
        fprintf(stdout, "here 5 (%d;%d) ADDED+: %d\n", grids[x][y].idx, grids[x][y].idy, val);
        grids[MAX_R][MAX_C] = erace_used(x, y, 0, grids);
        y++;
        sum_solution(x, y, val, grids);
    }

    if (grids[x][y].val_down == -1 &&
        grids[x][y].val_right == -1) { // pokud nemuze ani dolu ani doprava
        fprintf(stdout, "END SUM: %d\n", sum);
        fprintf(stdout, "-- (%d;%d)\n", grids[x][y].idx, grids[x][y].idy);
    }
}

void pass(Box grids[MAX_R][MAX_C]) {
    fprintf(stdout, "%d %d\n", R_, C_);

    for (int idx = 0; idx < R_; idx++) {
        for (int idy = 0; idy < C_; idy++) {
            fprintf(stdout, "%d ", grids[idx][idy].color);
        }
        fprintf(stdout, "\n");
    }
}

int main() {
    load_input();

    fprintf(stdout, "%d %d\n", R_, C_);

    for (int idx = 0; idx < R_; idx++) {
        for (int idy = 0; idy < C_; idy++) {
            fprintf(stdout, "%d ", grid[idx][idy].color);
        }
        fprintf(stdout, "\n");
    }

    solution();

    sum_solution(0, 0, 0, grid);

    for (int idx = 0; idx < R_; idx++) {
        for (int idy = 0; idy < C_; idy++) {
            fprintf(stdout, "(%d;%d) ", grid[idx][idy].val_down, grid[idx][idy].val_right);
        }
        fprintf(stdout, "\n");
    }

    return 0;
}