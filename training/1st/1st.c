#include <stdio.h>

double pointAX, pointAY, pointBX, pointBY, pointCX, pointCY, p;

int load_input() {
    fscanf(stdin, "%le %le %le %le %le %le %le", &pointAX, &pointAY, &pointBX, &pointBY, &pointCX, &pointCY, &p);

    return 0;
}

int free_all() {

    return 0;
}

int main () {
    load_input();

    free_all();
    return 0;
}