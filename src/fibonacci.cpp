#include "types.h"

unsigned fibonacci (unsigned n)
{
    if (n <= 2)
    {
        return 1;
    }

    unsigned x1, x2;
#pragma omp task
    {
        x1 = fibonacci(n - 1);
    };

#pragma omp task
    {
        x2 = fibonacci(n - 2);
    };

#pragma omp taskwait
    return x1 + x2;
}