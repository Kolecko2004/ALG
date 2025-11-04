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

int depth_and_update_diameter(int idx, char *arr) {
    if (idx >= num_pearls || arr[idx] == '2') // pokud je to leaf vrati to 1
        return 1;

    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    int left_depth = 0, right_depth = 0;

    if (left < num_pearls)
        left_depth = depth_and_update_diameter(left, arr);
    if (right < num_pearls)
        right_depth = depth_and_update_diameter(right, arr);
    
    int path_length = left_depth + right_depth + 1;

    if (path_length > max_pearl) {
        max_pearl = path_length;
    }

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

int find_max_pearls() {
    max_pearl = 0;
    if (num_pearls == 0)
        return 0;

    depth_and_update_diameter(0, pearls);
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
