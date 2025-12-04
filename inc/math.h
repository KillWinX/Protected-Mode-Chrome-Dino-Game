#ifndef MATH_H
#define MATH_H

#define PI 3.141592653589793
#define TAU (2 * PI)

static inline double sin(double val) {
    double term = val;
    double sum  = val;

    for (int n = 1; n < 10; n++) {
        term *= -1 * val * val / ((2 * n) * (2 * n + 1));
        sum  += term;
    }
    return sum;
}

static inline double cos(double valcos) {
    double term = 1.0;
    double sum  = 1.0;

    for (int n = 1; n < 10; n++) {
        term *= -1 * valcos * valcos / ((2 * n - 1) * (2 * n));
        sum  += term;
    }
    return sum;
}

static inline double tan(double valtan) {
    double s = sin(valtan);
    double c = cos(valtan);

    if (c == 0.0) {
        return 0.0;
    } else {
        return s / c;
    }
}

static inline double wrap_angle(double x) {
    while (x > PI)  x -= TAU;
    while (x < -PI) x += TAU;
    return x;
}

static inline unsigned sqrt(unsigned long n) {
    unsigned x = n;
    unsigned y = (x+1) >> 1;

    while(y < x) {
        x = y;
        y = (x+n/x) >> 1;
    }

    return x;
}


#endif
