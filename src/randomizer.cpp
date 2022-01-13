#include "types.h"

double RandomizeArraySingle(unsigned seed, unsigned* V, size_t n, unsigned min, unsigned max)
{
    uint64_t A = 6364136223846793005;
    unsigned B = 1;

    uint64_t prev = seed;
    uint64_t Sum = 0;

    for(unsigned int i = 0; i < n; i++)
    {
        uint64_t next = A*prev + B;
        V[i] = (next % (max - min + 1)) + min;
        prev = next;
        Sum += V[i];
        std::cout<<V[i]<<" ";
    }

    return (double)Sum/(double)n;
}

uint64_t pow(uint64_t base, uint64_t exp)
{
    uint64_t res = 1;
    for (unsigned i = 0; i < exp; i++)
    {
        res *= base;
    }
    return res;
}

uint64_t getB(unsigned exp, uint64_t A, uint64_t B)
{
    uint64_t sum = 0;
    for (unsigned i = 0; i <= exp; i++)
    {
        sum += pow(A, i);
    }

    if (sum == 0) {
        return B;
    } else {
        return B*sum;
    }
}

double RandomizeArrayShared(unsigned seed, unsigned* V, size_t n, unsigned min, unsigned max)
{
    uint64_t A = 6364136223846793005;
    uint64_t B = 1;
    unsigned T;
    uint64_t findA, findB;
    uint64_t Sum = 0;

    #pragma omp parallel shared(T, V, findA, findB)
    {
        unsigned t = (unsigned) omp_get_thread_num();
        #pragma omp single
        {
            T = (unsigned) omp_get_num_threads();
            findA = pow(A, T);
            findB = getB(T - 1, A, B);
        }
        uint64_t prev = seed;
        uint64_t elem;
        for (unsigned i = t; i < n; i += T)
        {
            if (i == t)
            {
                elem = pow(A, i + 1) * prev + getB(i, A, B);
            } else
            {
                elem = findA * prev + findB;
            }
            V[i] = (elem % (max - min + 1)) + min;
            prev = elem;
        }
    }

    for (unsigned i = 0; i < n; i++)
    {
        Sum += V[i];
    }

    return (double)Sum/(double)n;
}