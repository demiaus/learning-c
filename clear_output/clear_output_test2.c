#include <stdio.h>
#include <unistd.h>

int main(void) {

        unsigned secs = 10;
        unsigned rows = 20;

        for (unsigned i = 0; i < secs; ++i) {
                for (unsigned j = 0; j < rows; ++j) {
                        printf("\033[A\033[2K");
                }
                for (unsigned j = 0; j < rows; ++j) {
                        printf("testing testing %d\n", i);
                }
                sleep(1);
        }

        return 0;
}
