/*
 * Illustrate how values are initialized in arrays
 */

#include <stdio.h>

int array3[20];

void print_arr(char *comments[3], char *statement, int arr[20])
{
        printf("%s\n", comments[0]);
        printf("%s\n", comments[1]);
        printf("%s\n", statement);
        printf("[");

        for (int i = 0; i < 20; ++i) {
                printf("%d", arr[i]);
                if (i < 19) {
                        printf(", ");
                }
        }

        printf("]\n");
        printf("%s\n\n", comments[2]);
}

int main()
{
        int array1[20];
        int array2[20] = {[10] = 616};
        int array4[20] = {[0 ... 10] = -1};
        int *arrays[] = {array1, array2, array3, array4};

        char *comments1[] = {"// Uninitialized local array",
                                "// Inside main()",
                                "// ^^ Garbage..."};

        char *comments2[] = {"// Partly initialized local array",
                                "// Inside main()",
                                "// ^^ Along the explicit initialization, others are implicitly initialized to 0"};

        char *comments3[] = {"// Uninitialized global array",
                                "// Outside of main()",
                                "// Global declared arrays (and variables) are implicitly initialized to 0"};

        char *comments4[] = {"// Very naughty non-standard way to initialize a range. ISO C forbids it, although newer compilers support it (without pedantic-flag set)",
                                "// Can be inside or outside main()",
                                "// ^^ Range is initialized to given value, others implicitly to 0"};

        char **comments[] = {comments1, comments2, comments3, comments4};

        char *statements[] = {"int array1[20];",
                                "int array2[20] = {[10] = 616};",
                                "int array3[20];",
                                "int array4[20] = {[0 ... 10] = -1};"};


        for (int i = 0; i < 4; ++i) {
                print_arr(comments[i], statements[i], arrays[i]);
        }

        return 0;
}
