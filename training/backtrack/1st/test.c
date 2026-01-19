#include <stdio.h>
#include <math.h>

typedef struct {
    double x, y;
} Point;

double p;

double dist(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

/* obvod */
double perimeter(Point a, Point b, Point c) {
    return dist(a,b) + dist(b,c) + dist(c,a);
}

/* bod na úsečce */
Point lerp(Point a, Point b, double t) {
    Point r;
    r.x = a.x + t*(b.x - a.x);
    r.y = a.y + t*(b.y - a.y);
    return r;
}

/* rekurze */
void backtrack(Point A, Point B, Point C,
               long long *count, double *sum)
{
    double ab = dist(A,B);
    double bc = dist(B,C);
    double ca = dist(C,A);

    /* najdi nejdelší stranu = AC */
    if (ab > bc && ab > ca) {
        Point t = C; C = B; B = t;
    } else if (bc > ca) {
        Point t = A; A = B; B = t;
    }

    double per = perimeter(A,B,C);

    /* STOP PODMÍNKA (KRITICKÉ) */
    if (per <= p || dist(A,C) < 1e-6) {
        (*count)++;
        (*sum) += per;
        return;
    }

    /* D, E na AC */
    Point D = lerp(A, C, 1.0/3.0);
    Point E = lerp(A, C, 2.0/3.0);

    backtrack(A, B, D, count, sum);  // ABD
    backtrack(B, C, E, count, sum);  // BCE
    backtrack(B, D, E, count, sum);  // BDE
}

int main(void) {
    Point A,B,C;

    if (scanf("%lf %lf %lf %lf %lf %lf %lf",
        &A.x,&A.y,&B.x,&B.y,&C.x,&C.y,&p) != 7)
        return 1;

    long long count = 0;
    double sum = 0.0;

    double root = perimeter(A,B,C);

    backtrack(A,B,C,&count,&sum);

    double length = 0.5*root + 0.5*sum;

    printf("%lld %.2f\n", count, length);
    return 0;
}
