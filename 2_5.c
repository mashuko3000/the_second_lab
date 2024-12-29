#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#define OK 1
#define INVALID_COUNT 0
#define INVALID_EPSILON 3
#define NE_PODOYIDYOT 4

typedef struct {
    double x;
    double y;
} Point;

double cross_product(Point A, Point B, Point C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

int cross_two_lines(Point A, Point B, Point C, Point D) {
    double cp1 = cross_product(A, B, C);
    double cp2 = cross_product(A, B, D);
    double cp3 = cross_product(C, D, A);
    double cp4 = cross_product(C, D, B);
    return (cp1 * cp2 < 0) && (cp3 * cp4 < 0);
}

int polygon(int count, double epsilon, ...) {
    int i;
    int j;
    if(epsilon <= 0)
    {
        return INVALID_EPSILON;
    }
    if (count < 3) return INVALID_COUNT;
    
    va_list args;
    va_start(args, count);

    Point points[count];
    for (i = 0; i < count; i++) {
        points[i].x = va_arg(args, double);
        points[i].y = va_arg(args, double);
    }
    va_end(args);

    int direction = 0; //(1 против часовой, -1 по часовой)
    for (int i = 0; i < count; i++) {
        Point A = points[i];
        Point B = points[(i + 1) % count];
        Point C = points[(i + 2) % count];

        double cp = cross_product(A, B, C);
        if (fabs(cp) > epsilon) { 
            if (direction == 0) {
                direction = (cp > epsilon) ? 1 : -1;
            } 
            else if ((cp > epsilon && direction == -1) || (cp < -epsilon && direction == 1)) {
                return NE_PODOYIDYOT;
            }
        }
    }

    for (i = 0; i < count; i++) {
        for (j = i + 2; j < count; j++) {
            if (j == i + 1 || (i == 0 && j == count - 1)) {
                continue;
            }
            if (cross_two_lines(points[i], points[(i + 1) % count], points[j], points[(j + 1) % count])) {
                return NE_PODOYIDYOT; 
            }
        }
    }

    return OK; 
}


int main() {
    if (polygon(8, 3.0, 1.0, 1.0, 3.0, 5.0, 7.0, 9.0, 3.0, 7.0, 1.0, 4.0, 3.0, 5.0, 4.0, 6.0, 3.0)) {
        printf("Многоугольник выпуклый\n");
    } else {
        printf("Многоугольник не выпуклый\n");
    }

    if (polygon(8, 5.0, 1.0, 3.0, 3.0, 3.0, 5.0, 4.0, 7.0, 5.0, 5.0, 6.0, 7.0, 7.0, 5.0, 7.0, 3.0)) {
        printf("Многоугольник выпуклый\n");
    } else {
        printf("Многоугольник не выпуклый\n");
    }

    if (polygon(3, 1.0, 1.0, 2.0, 4.0, 5.0, 1.0)) {
        printf("Многоугольник выпуклый\n");
    } else {
        printf("Многоугольник не выпуклый\n");
    }

    return 0;
}
