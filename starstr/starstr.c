/*
 * Testing and demonstrating the use of '*' in context of reading and writing formatted strings
 * No error checking though
 */

#include <stdio.h>
#include <time.h>

int main(void)
{
        unsigned day, month, year, pad, tmp;
        char date_str[64];
        char *filename;
        FILE *f;
        time_t t;
        struct tm *tm;

        filename = "test.txt";
        t = time(NULL);
        tm = localtime(&t);

        strftime(date_str, sizeof(date_str), "%F", tm);
        sscanf(date_str, "%u-%u-%u", &year, &month, &day);
        printf("\nSystem gave date %02u.%02u.%04u\n", day, month, year);

        printf("Writing date to file %s\n", filename);
        f = fopen(filename, "w");
        fprintf(f, "%u.%u.%u\n", day, month, year);
        printf("Zeroing date variables to %02u.%02u.%04u and closing file\n", day = 0, month = 0, year = 0);
        fclose(f);

        f = fopen(filename, "r");

        // Reading incomplete lines works...
        fscanf(f, "%u.%u", &day, &month);
        printf("Read from file %s\nThe date is %0u.%0u. of unknown year.\n", filename, day, month);

        rewind(f);

        // But if you need to skip reading some format characters before the one you are interested in, use '*' after '%'
        fscanf(f, "%*u.%*u.%u", &year);
        printf("Actually, the year is %u, I checked.\n", year);
        fclose(f);

        printf("\nSometimes you want to pad the output strings conditionally.\n");
        printf("Maybe you foolishly assumed the year will always be a four-digit number.\n");

        printf("\n%-5s : %4u\n%-5s : %4u\n%-5s : %4u\n\n", "Day", day, "Month", month, "Year", year);

        printf("But just in %u years your programs output will be aligned ugli-ly\n", 10000 - year);
        printf("That is called... TECHNICAL DEBT!!!\n");
        printf("Just look at this monstrosity...\n\n");
        printf("%-5s : %4u\n%-5s : %4u\n%-5s : %4u\n\n", "Day", day, "Month", month, "Year", 10000);

        printf("\nBut if you count the length of our input and use '*', you can always print it very nicelily\n");

        while (year < 1000000000) {

                tmp = year;
                pad = 1;

                while (tmp /= 10) {
                        pad++;
                }

                printf("\n%-5s : %*u\n%-5s : %*u\n%-5s : %*u\n", "Day", pad, day, "Month", pad, month, "Year", pad, year);

                year *= 10;
        }

        printf("\nYou can also specify precision of floats with '.*':\n");
        printf("%d-precision: %.*f\n", 0, 0, 69.420);
        printf("%d-precision: %.*f\n\n", 3, 3, 69.420);


        return 0;
}
