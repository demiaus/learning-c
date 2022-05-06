#include <stdio.h>
#include <unistd.h>

int main(void) {

        unsigned secs = 10;

        for (unsigned i = 0; i < secs; ++i) {
                printf("\033[A\033[2K");
                printf("testing testing %d\n", i);
                sleep(1);
        }

        return 0;
}
