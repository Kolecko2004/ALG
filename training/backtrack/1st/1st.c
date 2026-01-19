#include <stdio.h>
#include <math.h>

typedef struct {
    double x, y;
} Point;

double p;

double get_pythagoras(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y; 

    return sqrt(dx * dx + dy * dy);
}

double circuit(Point a, Point b, Point c) {
    return get_pythagoras(a, b) + get_pythagoras(b, c) + get_pythagoras(c, a);
}

Point point_on_line(Point a, Point b, double t) {
    Point new_point;
    new_point.x = a.x + t * (b.x - a.x);
    new_point.y = a.y + t * (b.y - a.y);

    return new_point;
}

void backtrack(Point A, Point B, Point C, long long *count, double *sum) {
    double ab = get_pythagoras(A, B);
    double bc = get_pythagoras(B, C);
    double ca = get_pythagoras(C, A);

    if (ab > bc && ab > ca) {
        Point tmp = C;
        C = B;
        B = tmp;
    } else if (bc > ca) {
        Point t = A;
        A = B;
        B = t;
    }

    double cir = circuit(A, B, C);

    if (cir <= p) {
        (*count)++;
        (*sum) += cir;
        return;
    }

    Point D = point_on_line(A, C, 1.0/3.0);
    Point E = point_on_line(A, C, 2.0/3.0);

    backtrack(A, B, D, count, sum);
    backtrack(B, C, E, count, sum);
    backtrack(B, D, E, count, sum);
}

int main () {
    Point A, B, C;

    // load input
    int res = fscanf(stdin, "%lf %lf %lf %lf %lf %lf %lf", &A.x, &A.y, &B.x, &B.y, &C.x, &C.y, &p);
    if (res != 7) {
        fprintf(stderr, "ERROR: error while loading input");
        return 100;
    }
    fprintf(stdout, "LOAD: (%g:%g) (%g:%g) (%g:%g)\n", A.x, A.y, B.x, B.y, C.x, C.y);

    long long count = 0;
    double sum = 0;

    double root = circuit(A, B, C);

    backtrack(A, B, C, &count, &sum);

    double length = 0.5 * root + 0.5 * sum;

    fprintf(stdout, "%lld %.2f\n", count, length);

    return 0;
}