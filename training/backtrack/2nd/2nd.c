#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_M 100
#define MAX_N 100
#define MAX_PATH (MAX_M * MAX_N * 2)
#define HASH_SIZE 200003  // Prime number for better distribution

typedef struct {
    int x, y;
    char from; // L = left, T = top, S = start
    char path[MAX_PATH];
    int letters;
    int changes_of_dir;
} Robot;

typedef struct PathNode {
    char path[MAX_PATH];
    int count;
    struct PathNode *next;
} PathNode;

typedef struct {
    char path[MAX_PATH];
    int count;
} PathResult;

PathNode *hash_table[HASH_SIZE];
PathResult *results = NULL;
int num_results = 0;
int max_results = 110000;

int M, N, k_changes;
char game_plan[MAX_M][MAX_N];

// Simple hash function
unsigned long hash_string(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

// Add or increment path count in hash table
void add_path(const char *path) {
    unsigned long hash = hash_string(path);
    PathNode *node = hash_table[hash];
    
    // Search for existing path in chain
    while (node != NULL) {
        if (strcmp(node->path, path) == 0) {
            node->count++;
            return;
        }
        node = node->next;
    }
    
    // Path not found, create new node
    PathNode *new_node = (PathNode*)malloc(sizeof(PathNode));
    strcpy(new_node->path, path);
    new_node->count = 1;
    new_node->next = hash_table[hash];
    hash_table[hash] = new_node;
}

int path_finding(Robot robot);

void load_input() {
    scanf("%d %d %d\n", &M, &N, &k_changes);
    for (int idx = 0; idx < M; idx++) {
        for (int idy = 0; idy < N; idy++) {
            scanf(" %c", &game_plan[idx][idy]);
        }
    }
}

void send_to(Robot robot, char direction, int num) {
    // Add character(s) to path (if not on starting position)
    if (robot.x != 0 || robot.y != 0) {
        for (int idx = 0; idx < num; idx++) {
            robot.path[robot.letters] = game_plan[robot.x][robot.y];
            robot.letters++;
        }
    }

    robot.from = direction;

    if (direction == 'T') {
        robot.x++;
    } else {
        robot.y++;
    }

    if (num == 2) {
        robot.changes_of_dir++;
    }
    
    path_finding(robot);
}

int path_finding(Robot robot) {
    // Check if we reached destination
    if (robot.x == M - 1 && robot.y == N - 1) {
        if (robot.changes_of_dir == k_changes) {
            // Don't add the last character - just null-terminate
            robot.path[robot.letters] = '\0';
            add_path(robot.path);
        }
        return 0;
    }
    
    if (robot.from == 'S') {
        // Starting position - can go down or right (no change counted)
        if (robot.x + 1 < M) {
            send_to(robot, 'T', 1);
        }
        if (robot.y + 1 < N) {
            send_to(robot, 'L', 1);
        }
    } else if (robot.from == 'L') {
        // Came from left - can continue right (1 char) or change to down (2 chars)
        if (robot.x + 1 < M && robot.changes_of_dir < k_changes) {
            send_to(robot, 'T', 2);
        }
        if (robot.y + 1 < N) {
            send_to(robot, 'L', 1);
        }
    } else if (robot.from == 'T') {
        // Came from top - can continue down (1 char) or change to right (2 chars)
        if (robot.y + 1 < N && robot.changes_of_dir < k_changes) {
            send_to(robot, 'L', 2);
        }
        if (robot.x + 1 < M) {
            send_to(robot, 'T', 1);
        }
    }

    return 0;
}

int compare_results(const void *a, const void *b) {
    return strcmp(((PathResult*)a)->path, ((PathResult*)b)->path);
}

void collect_results() {
    // Count total unique paths
    num_results = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        PathNode *node = hash_table[i];
        while (node != NULL) {
            num_results++;
            node = node->next;
        }
    }
    
    // Allocate results array
    results = (PathResult*)malloc(num_results * sizeof(PathResult));
    
    // Copy from hash table to results array
    int idx = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        PathNode *node = hash_table[i];
        while (node != NULL) {
            strcpy(results[idx].path, node->path);
            results[idx].count = node->count;
            idx++;
            node = node->next;
        }
    }
}

void free_hash_table() {
    for (int i = 0; i < HASH_SIZE; i++) {
        PathNode *node = hash_table[i];
        while (node != NULL) {
            PathNode *temp = node;
            node = node->next;
            free(temp);
        }
        hash_table[i] = NULL;
    }
}

int main() {
    // Initialize hash table
    for (int i = 0; i < HASH_SIZE; i++) {
        hash_table[i] = NULL;
    }

    load_input();

    Robot robot;
    robot.letters = 0;
    robot.changes_of_dir = 0;
    robot.x = 0;
    robot.y = 0;
    robot.from = 'S';  // Start position
    robot.path[0] = '\0';
    
    path_finding(robot);

    // Collect results from hash table
    collect_results();

    // Find maximum frequency
    int max_count = 0;
    for (int i = 0; i < num_results; i++) {
        if (results[i].count > max_count) {
            max_count = results[i].count;
        }
    }

    // Sort paths alphabetically
    qsort(results, num_results, sizeof(PathResult), compare_results);

    // Print all paths with maximum frequency
    for (int i = 0; i < num_results; i++) {
        if (results[i].count == max_count) {
            printf("%s\n", results[i].path);
        }
    }

    free(results);
    free_hash_table();
    return 0;
}