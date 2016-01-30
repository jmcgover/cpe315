#include <stdio.h>

#define STR_PROMPT_1 "Enter Number 1: "
#define STR_PROMPT_2 "Enter Number 2: "
#define FMT_SCAN_INT "%d"
#define FMT_SCAN_CHR " %c"
#define FMT_PRODUCT  "Product is: %d\n"
#define STR_AGAIN    "Again? "

int mymult(int multiplicand, int multiplier) {
    int product = multiplicand * multiplier;
    return product;
}

#define LSB_MASK 0x01
int multiply(int multiplicand, int multiplier) {
    int product = 0;
    while (multiplier != 0) {
        if (multiplier & LSB_MASK == 1) /* LSB = Least Significant Bit */
            product = product + multiplicand;
        multiplier = multiplier >> 1;
        multiplicand = multiplicand << 1;
    }
    return product;
}

int main(int argc, char **argv) {
    int multiplicand, multiplier, product;
    char again;
    do {
        printf(STR_PROMPT_1);
        scanf(FMT_SCAN_INT, &multiplicand);
        printf(STR_PROMPT_2);
        scanf(FMT_SCAN_INT, &multiplier);

        //product = mymult(multiplicand, multiplier);
        product = multiply(multiplicand, multiplier);
        printf(FMT_PRODUCT, product);

        printf(STR_AGAIN);
        scanf(FMT_SCAN_CHR, &again);
    } while(again == 'y');
    return 0;
}
