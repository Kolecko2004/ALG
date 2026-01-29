#include <stdio.h>

#define MAX 10

int R, C;
int grid[MAX][MAX];
int best = 0;

/* skóre trojice */
int score(int a, int b, int c) {
    if (a == b && b == c) return 3;
    if (a == b || a == c || b == c) return 1;
    return 0;
}

/* výpis mřížky */
void print_grid(int depth, int current) {
    for (int d = 0; d < depth; d++) printf("  ");
    printf("Score = %d\n", current);

    for (int i = 0; i < R; i++) {
        for (int d = 0; d < depth; d++) printf("  ");
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == 0)
                printf(". ");
            else
                printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* backtracking */
void dfs(int current, int depth) {
    if (current > best)
        best = current;

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {

            /* vodorovná trojice */
            if (j + 2 < C &&
                grid[i][j] && grid[i][j+1] && grid[i][j+2]) {

                int s = score(grid[i][j], grid[i][j+1], grid[i][j+2]);

                int a = grid[i][j];
                int b = grid[i][j+1];
                int c = grid[i][j+2];

                grid[i][j] = grid[i][j+1] = grid[i][j+2] = 0;

                for (int d = 0; d < depth; d++) printf("  ");
                printf("Remove H (%d,%d) +%d\n", i, j, s);
                print_grid(depth, current + s);

                dfs(current + s, depth + 1);

                grid[i][j] = a;
                grid[i][j+1] = b;
                grid[i][j+2] = c;
            }

            /* svislá trojice */
            if (i + 2 < R &&
                grid[i][j] && grid[i+1][j] && grid[i+2][j]) {

                int s = score(grid[i][j], grid[i+1][j], grid[i+2][j]);

                int a = grid[i][j];
                int b = grid[i+1][j];
                int c = grid[i+2][j];

                grid[i][j] = grid[i+1][j] = grid[i+2][j] = 0;

                for (int d = 0; d < depth; d++) printf("  ");
                printf("Remove V (%d,%d) +%d\n", i, j, s);
                print_grid(depth, current + s);

                dfs(current + s, depth + 1);

                grid[i][j] = a;
                grid[i+1][j] = b;
                grid[i+2][j] = c;
            }
        }
    }
}

int main() {
    scanf("%d %d", &R, &C);

    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            scanf("%d", &grid[i][j]);

    printf("Initial grid:\n");
    print_grid(0, 0);

    dfs(0, 0);

    printf("BEST SCORE = %d\n", best);
    return 0;
}
