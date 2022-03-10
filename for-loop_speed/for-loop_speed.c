#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/time.h>

/*
 * Trying to look for a difference in speed using the postfix in-/decrement vs.
 * prefix in-/decrement
 */

int main(void)
{
        int i, j, n;
        struct timeval tval_before_1, tval_after_1, tval_result_1;
        struct timeval tval_before_2, tval_after_2, tval_result_2;
        struct timeval tval_diff;
        n = 10;

        while (--n) {
                gettimeofday(&tval_before_1, NULL);
                for (i = 0; i < 1000000000; i++)
                        ;
                for (; i > 0; i--)
                        ;
                gettimeofday(&tval_after_1, NULL);

                gettimeofday(&tval_before_2, NULL);
                for (j = 0; j < 1000000000; ++j)
                        ;
                for (; j > 0; --j)
                        ;
                gettimeofday(&tval_after_2, NULL);

                timersub(&tval_after_1,  &tval_before_1, &tval_result_1);
                timersub(&tval_after_2,  &tval_before_2, &tval_result_2);
                timersub(&tval_result_1, &tval_result_2, &tval_diff);

                // printf("Postfix: %ld.%06ld\n",   (long int)tval_result_1.tv_sec, (long int)tval_result_1.tv_usec);
                // printf("Prefix:  %ld.%06ld\n\n", (long int)tval_result_2.tv_sec, (long int)tval_result_2.tv_usec);
        }
        printf("%d %d\n", i, j);
        printf("Postfix - prefix: %ld.%06ld\n", (long int)tval_diff.tv_sec, (long int)tval_diff.tv_usec);
}
