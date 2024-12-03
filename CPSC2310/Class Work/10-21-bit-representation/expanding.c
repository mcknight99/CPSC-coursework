#include <stdint.h>
#include <stdio.h>

void print_bits(uint16_t x)
{
    for (int i = 15; i >= 0; i--)
    {
        printf("%d", (x >> i) & 1);
    }
    printf("\n");
}

void print_bits_signed(int16_t x)
{
    for (int i = 15; i >= 0; i--)
    {
        printf("%d", (x >> i) & 1);
    }
    printf("\n");
}


int main()
{
    // this is unsigned, so the most significant bit is just a bit
    // when the zero extension happens, it just adds 0s to the left
    uint8_t a = 0b10000000;
    uint16_t b = a;

    printf("a: ");
    print_bits(a);

    printf("b: ");
    print_bits(b);

    // this is signed, so the most significant bit is the sign bit
    // when the signed extension happens, it adds 1s to the new bits on the left if the sign bit is 1
    int8_t c = 0b10000000;
    int16_t d = c;

    printf("c: ");
    print_bits_signed(c);

    printf("d: ");
    print_bits_signed(d);

    int8_t e = 0b01000000;
    int16_t f = e;

    printf("e: ");
    print_bits_signed(e);
    
    printf("f: ");
    print_bits_signed(f);
}