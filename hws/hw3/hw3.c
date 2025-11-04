#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int num_pearls, max_pearl = 0;
char *pearls = NULL;

void load_input() {
    scanf("%d", &num_pearls);
    
    pearls = malloc(num_pearls * sizeof(char));
    if (pearls == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(101);
    }

    for (int idx = 0; idx < num_pearls; idx++) {
        scanf(" %c", &pearls[idx]);
    }
}

int depth_and_update_diameter(int idx, char *arr, char before_colour) {
    if (idx >= num_pearls || arr[idx] == '2') { // pokud je to leaf vrati to 1
        // fprintf(stdout, "im leaf returening 1 - idx: %d\n", idx);
        return 1;
    }

    // Idx for the next node
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    int left_depth = 0, right_depth = 0;

    // tchuba aktualne nastava ze to nzjisti pokud 3 koralky jsou stejne ale jdou pres koren stromu 
    if (!(before_colour == arr[idx] && arr[idx] == arr[2 * idx + 1] && before_colour == arr[2 * idx + 1])) {
        left_depth = depth_and_update_diameter(left, arr, arr[idx]);
    }

    if (!(before_colour == arr[idx] && arr[idx] == arr[2 * idx + 2] && before_colour == arr[2 * idx + 2])) {
        right_depth = depth_and_update_diameter(right, arr, arr[idx]);
    }
    
    int path_length = left_depth + right_depth + 1;
    if (path_length > max_pearl) {
        max_pearl = path_length;
    }

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

int find_max_pearls() {
    max_pearl = 0;
    if (num_pearls == 0) {
        return 0;
    }

    if (pearls[0] == pearls[1] && pearls[0] == pearls[2]) {
        fprintf(stdout, "special\n");
        depth_and_update_diameter(1, pearls, -1);
        depth_and_update_diameter(2, pearls, -1);
        return max_pearl;
    }

    depth_and_update_diameter(0, pearls, -1);
    return max_pearl;
}

int main() {
    load_input();

    for (int i = 0; i < num_pearls; i++) {
        printf("pearl[%d] = %c\n", i, pearls[i]);
    }

    fprintf(stdout, "max: %d\n", find_max_pearls());

    free(pearls);
    return 0;
}
