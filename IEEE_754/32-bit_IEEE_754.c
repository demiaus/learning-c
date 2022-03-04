#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// stdint.h was introduced in C99 to have constant size integer types.
#include <math.h>
// math.h has to be explicitly linked during compilation with a flag -lm for
// historical reasons

/*
   This program demonstrates how floats are stored in memory
   and how they are calculated
*/

int main(void)
{
        // float is a 32-bit IEEE 754 single precision floating point number.
        // It has 1-bit for the sign, 8-bit for exponent, 23-bit (but
        // implicitly 24-bits) for the significand. It has 6 decimal digits of
        // precision.

        printf("\nThis program will explore how floating point numbers work.\n\n");

        // Best 3-digit rational approximation of pi is 355/113.
        // Let's double that to make `tau` and use that as our example.
        float divident  = 355;
        float divisor   = 113;
        float pi_float  = divident / divisor;
        float tau_float = pi_float * 2;
        float new_float;
        float fraction;

        int32_t  exponent_tmp_int;
        int32_t  jumps;
        int32_t  offset;
        int32_t  tau_int = tau_float;
        int32_t  tmp_int;
        int32_t  true_binary;
        int8_t   i;
        int8_t   j;
        int8_t   sign;
        int8_t   example_range_top;
        int8_t   example_range_bottom;
        uint32_t encoded_significand;
        uint32_t new_int;
        uint32_t tau_float_bits;
        uint8_t  dot_index;
        uint8_t  encoded_exponent;
        uint8_t  example_bits;
        uint8_t  example_n;
        uint8_t  exponent_bit_cnt;
        uint8_t  one_index;
        uint8_t  sign_bit_int;


        char tmp_char;
        char orig_float_str[33]       = { 0 };
        char orig_sign_bit_str[2]     = { 0 };
        char orig_exponent_str[9]     = { 0 };
        char orig_significand_str[24] = { 0 };
        char new_float_str[33]        = { 0 };
        char new_sign_bit_str[2]      = { 0 };
        char new_exponent_str[9]      = { 0 };
        char new_significand_str[24]  = { 0 };
        char tmp_str[34]              = { 0 };
        char tmp_bit_char;
        char *tmp_ptr = orig_sign_bit_str;

        // Using an "evil bit hack", one step in the source code of Quake III
        // Arena's `Fast inverse square root` algorithm. This saves the actual
        // bits of the float representation into a 32-bit integer
        tau_float_bits = *(uint32_t *) &tau_float;

        printf("Best 3-digit rational approximation of pi is %.0f / %.0f.\n", divident, divisor);
        printf("Let's double that (%f) to make a more interesting binary number `tau` (%f) and use that as our example.\n", pi_float, tau_float);
        printf("Binary form IEEE 754 single precision floating point representation of that is:\n");


        // Store the bits into orig_float_str
        // and substrings into orig_sign_bit_str,
        //                     orig_exponent_str, and
        //                     orig_significand_str
        i = 32;
        j =  0;
        while (i-- > 0 ) {
                tmp_ptr[j++] = orig_float_str[31-i] = ((tau_float_bits & (1 << i)) >> i) + '0';
                if (i == 31) {
                        tmp_ptr = orig_exponent_str;
                        j = 0;
                } else if (i == 23) {
                        tmp_ptr = orig_significand_str;
                        j = 0;
                }
        }

        printf("\n%s\nor separated into fields\n", orig_float_str);
        printf("%s %s %s\n", orig_sign_bit_str, orig_exponent_str, orig_significand_str);

        printf("\nwhere the three fields are:\n\n");

        printf("%-28s : %-s\n", "Sign bit",                     orig_sign_bit_str);
        printf("%-28s : %-s\n", "8-bit exponent (encoded)",     orig_exponent_str);
        printf("%-28s : %-s\n", "23-bit significand (encoded)", orig_significand_str);

        printf("\n(Note that while the significand has a 24 bit value, it only has 23 bits stored, as the 24th bit is `1` by definition)\n");

        sign_bit_int        = tau_float_bits >> 31;
        encoded_exponent    = tau_float_bits >> 23;
        encoded_significand = tau_float_bits << 9 >> 9;

        printf("\nValues in decimal form:\n");
        printf("%-21s : %u\n", "Sign bit",              sign_bit_int);
        printf("%-21s : %u\n", "Exponent (encoded)",    encoded_exponent);
        printf("%-21s : %u\n", "Significand (encoded)", encoded_significand);

        printf("\nIEEE 754 encodes the values in a way, where the lowest value is all zeros\n");
        printf("and the range favours positive numbers (has one more of those.\n)");
        example_bits         =  4;
        example_range_bottom = -7;
        example_range_top    =  8;
        printf("As an example, %u-bit numbers ranging from %d to %d, would be encoded like this:\n\n", example_bits, example_range_bottom, example_range_top);

        offset = -example_range_bottom;
        for (i = 0-offset; i <= offset+1; ++i) {
                true_binary = i + offset;
                printf("%2d ", i);
                for (j = 3; j >= 0; --j) {
                        printf("%c", true_binary & 1 << j ? '1' : '0');
                }
                printf("\n");
        }

        example_n = 3;
        printf("\nTo encode unsigned value to follow that schema, one has to add the number of negative values in the schema to the unsigned value\n");
        printf("For example, to encode %u, normally in binary: ", example_n);
        for (j = 3; j >= 0; --j) {
                printf("%d", (example_n & 1 << j) >> j);
        }

        printf("\none would add %d, resulting in denary %d, which in binary would be ", offset, offset + example_n);
        for (j = 3; j >= 0; --j) {
                printf("%d", ((example_n + offset) & 1 << j) >> j);
        }
        printf(", our encoded value.\n");


        exponent_tmp_int = (int32_t)powf(2, 8 - 1) - 1;
        exponent_bit_cnt = 8;
        printf("\nIn this system, encoded n-bit exponent value has an `exponent bias` of 2^(n-1) - 1\n");
        printf("In our case with %u bits that is 2^(%u-1) =  %d\n", exponent_bit_cnt, exponent_bit_cnt, exponent_tmp_int);

        printf("\nLet's try to reproduce the IEEE 754 bit pattern of %f `by hand`, as if on paper\n", tau_float);

        tau_int = tau_float;
        printf("This is the whole number portion of the number: %d\n", tau_int);
        printf("To turn it into binary by hand, you would divide by 2, keep note of the remainder, and finally write the remainders down in reverse order\n\n");

        tmp_int = tau_int;
        while (tau_int > 0) {
                printf("%d / 2 = %d, remainder %d\n", tau_int, tau_int / 2, tau_int % 2);
                tau_int /= 2;
        }
        printf("In reverse, correct order: ");

        tau_int = tmp_int;
        i       = 32;
        j       =  0;

        // Skip leading zeros
        while ((tau_int & 1 << --i) == 0);
        ++i;

        // Save tau binary to string
        while (i--) {
                tmp_bit_char = tau_int & 1 << i ? '1' : '0';
                tmp_str[j++] = tmp_bit_char;
        }
        printf("%s\n\n", tmp_str);

        fraction = tau_float - tau_int;
        printf("Next we have to convert the fractional part `%f` into binary.\n", fraction);
        printf("It follows a similar pattern, but instead of dividing, you multiply by 2 and keep note of the emerging whole numbers.\n");
        printf("You stop when the result is `1` or repeating\n\n");

        tmp_str[j++] = '.';
        i = 32;
        while (fraction && i--) {
                printf("%f * 2 = ", fraction);
                fraction *= 2;
                tmp_str[j++] = tmp_bit_char = (fraction >= 1) + '0';
                printf("%f, whole part %c\n", fraction, tmp_bit_char);
                fraction -= fraction >= 1;
        }

        printf("\nThis time you read the result from top to bottom\n");

        printf("So the entire number in true binary looks like this\n");
        printf("\n%s\n\n", tmp_str);

        printf("Now we need to normalize the significand - that is; move the binary point so, that only one non-zero number is on the left side of it\n");
        printf("Keep count of how many times you float the point to the left\n");
        printf("(Use negative values for rightwise movement)\n\n");

        one_index = 0;
        dot_index = 0;

        while (tmp_str[one_index] != '1')
                ++one_index;
        while (tmp_str[dot_index] != '.')
                ++dot_index;

        jumps = dot_index - one_index - 1;

        sign = jumps >= 0 ? 1 : -1;
        while (dot_index != one_index + sign) {
                tmp_char = tmp_str[dot_index-sign];
                tmp_str[dot_index-sign] = tmp_str[dot_index];
                tmp_str[dot_index] = tmp_char;
                dot_index -= sign;
                printf("%s\n", tmp_str);
        }
        printf("\n(%d jump%s)\n", jumps, jumps > 1 ? "s" : "");

        printf("\nWe then get rid of the leftmost `1` before the binary point\n");
        printf("and fill the end with zeros\n");

        tmp_str[one_index] = '.';
        i = dot_index;
        while (tmp_str[++i] != '\0') {
                tmp_str[i-1] = tmp_str[i];
        }
        tmp_str[22] = '0';
        printf("\n%s\n", tmp_str);

        i = 0;
        while (tmp_str[++i] != '\0') {
                tmp_str[i-1] = tmp_str[i];
        }
        tmp_str[22] = '0';

        printf("%s\n\n", tmp_str);

        printf("Next we need to un-bias the biased exponent (%d) by adding the amount of jumps (%d) to it:\n",
                        exponent_tmp_int, jumps);
        printf("\n%d + %d = ", exponent_tmp_int, jumps);
        exponent_tmp_int += jumps;
        printf("%d,\n", exponent_tmp_int);

        printf("in binary ");

        i = 8;
        while (i--) {
                printf("%d", (exponent_tmp_int & 1 << i) >> i);
        }
        puts("");

        printf("Now, let's try to assemble the bits into a 32-bit string.\n\n");

        new_sign_bit_str[0] = tau_float < 0 ? '1' : '0';
        i = 8;
        while (i--) {
                new_exponent_str[i] = ((exponent_tmp_int & 1 << i) >> i) + '0';
        }

        i = 0;
        while (tmp_str[i] != '\0') {
                new_significand_str[i] = tmp_str[i];
                ++i;
        }

        printf("%11s : %s\n", "Sign bit",    new_sign_bit_str);
        printf("%11s : %s\n", "Exponent",    new_exponent_str);
        printf("%11s : %s\n", "Significand", new_significand_str);
        printf("\nAll together now:\n\n");

        sprintf(new_float_str, "%s%s%s", new_sign_bit_str, new_exponent_str, new_significand_str);

        printf("%s\n\n", new_float_str);

        printf("Let's try to first plug that into an unsigned int and then cast that to float\n\n");

        i = 32;
        new_int = 0;
        while (i--) {
                new_int += (new_float_str[31 - i] - '0') << i;
        }

        printf("The unsigned int value is %u\n", new_int);
        printf("In binary: ");
        i = 32;
        while (i--) {
                printf("%u", (new_int & 1 << i) >> i);
        }
        puts("");

        printf("Now let's cast it into a float\n\n");

        new_float = *(float *) &new_int;

        printf("%f\n", new_float);

        return 0;
}
