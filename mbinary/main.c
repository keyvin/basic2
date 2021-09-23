#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>


typedef struct UNPACKED_FLOAT {
    uint8_t exponent;
    uint8_t sign;
    uint8_t mantessa[4]; //byte 4 is the underflow/buffer byte used for rounding.
} unpacked_f;

typedef uint32_t packed_f;
//stored little endian
uint8_t overflow;
//unpacked
void increment(unpacked_f *o1) {
    uint16_t scratch = 0;
    //set true to increment the first
    overflow = 1;
    for (int i = 2; i>0; i++) {
        scratch = 0;
        if (overflow) {
            scratch = o1->mantessa[i];
            scratch++;
        }
        if (scratch & 0x0100) {
            overflow = 1;
        }
        else {
            overflow = 0;
        }
    }
    //overflow set true if the increment resulted in overflow
}

void unpack (uint32_t t, unpacked_f *result) {
    //will be stored in reverse order

    uint8_t *p = &t;
    if (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__) {
        result->exponent = *(p+3);
        result->sign = *(p+2)&0x80;
        //implicit 1
        result->mantessa[0] = *(p+2)|0x80;
        result->mantessa[1] = *(p+1);
        result->mantessa[2] = *(p);
        result->mantessa[3] = 0;
    }
    else {
        result->exponent = *p;
        result->sign = *(p+1)&0x80;
        result->mantessa[0] = *(p+1)|0x80;
        result->mantessa[1] = *(p+2);
        result->mantessa[2] = *(p+3);
        result->mantessa[3]= 0;
    }

}

packed_f pack(unpacked_f *o1) {
    packed_f a = 0;
    packed_f result = 0;
    a = o1->exponent;
    a = a <<24;
    //positive is zero, so set if negative, clear if positive
    result = a;
    if (o1->sign)
        o1->mantessa[0]=o1->mantessa[0] | 0x80;
    else
        o1->mantessa[0]=o1->mantessa[0] & 0x7f;
    a=0;
    a = o1->mantessa[0];
    a = a << 16;
    result = result | a;
    a = 0;
    a = o1->mantessa[1];
    a = a << 8;
    result = result | a;
    a = 0;
    a = o1->mantessa[2];
    result = result | a;
    return result;
}


//used for addition and subtraction
void shift_right(uint8_t right_shift, unpacked_f *target) {
    uint8_t low_bit_true = 0;
    uint8_t high_bit_true = 0;
    uint8_t high_unset_set[] = {0x7F,0xFF} ;
    //clear underflow
    target->mantessa[3] =0;

    for (int rs = right_shift; rs > 0; rs--) {
            low_bit_true = target->mantessa[0] &0x01;
            target->mantessa[0] =  target->mantessa[0] >> 1;
            target->mantessa[0] = target->mantessa[0] & 0x7F;
            high_bit_true = low_bit_true;
            low_bit_true = target->mantessa[1] & 0x01;
            target->mantessa[1]  = (target->mantessa[1] >> 1) & high_unset_set[high_bit_true];
            high_bit_true = low_bit_true;
            low_bit_true = target->mantessa[2] & 0x01;
            target->mantessa[2] = (target->mantessa[2] >> 1) & high_unset_set[high_bit_true];
            target->mantessa[3] = (target->mantessa[3] >> 1) & high_unset_set[low_bit_true];
    }
    return;
}

void negate_f(unpacked_f *o1) {
    o1->mantessa[3] = 0-o1->mantessa[3];
    o1->mantessa[2] = 0-o1->mantessa[2];
    o1->mantessa[1] = 0-o1->mantessa[1];
    o1->mantessa[0] = 0-o1->mantessa[0];
    return;

}
//o1 is value in registers in 8080
void subtract(unpacked_f *o1, unpacked_f *o2) {
    //negate overflow byte (subtract from 0)
    //results stored in o1
    uint8_t borrow = o1->mantessa[3] & 80;
    uint16_t scratch;
    o1->mantessa[3] = 0-o1->mantessa[3];
    scratch = o2->mantessa[2] | 0x0100;
    if (borrow) {
        scratch--;
        borrow = 0;
    }
    scratch -= o1->mantessa[2];
    if ((scratch &0x0100) ==0) borrow = 1;
    o1->mantessa[2] = (uint8_t) scratch;
    scratch = o2->mantessa[1] | 0x0100;
    if (borrow) {
        scratch--;
        borrow = 0;
    }
    scratch -= o2->mantessa[1];
    o1->mantessa[1] = (uint8_t) scratch;
    if ((scratch & 0x0100) ==0) borrow =1;
    scratch = 0;
    scratch = o2->mantessa[0] |0x0100;
    if (borrow){
        scratch--;
        borrow = 0;
    }
    scratch-= o1->mantessa[0];
    if ((scratch & 0x0100) ==0) borrow = 1;
    o1->mantessa[0] = (uint8_t) scratch;
    //result was negative. Negate mantessa
    //TODO - sign bit?
    //in 8080 basic, the sign bit is supposed to match the FAC(o2).
    //we will keep the sign in o1 instead
    if(borrow)
    {
        o1->sign = !o2->sign;
        negate_f(o1);
    }

}

//o1 will have the underflow byte set. o1 is value in registers in 8080
//o1 is where the result is stored
void add(unpacked_f *o1, unpacked_f *o2) {
    //should already be aligned. There is no carry check in C.
    //use bigger uint and check the high bit for carry
    uint16_t carry =0;

    uint16_t scratch1, scratch2 = 0;

    scratch1 = o1->mantessa[2];
    scratch2 = o2->mantessa[2];
    scratch1 += scratch2;
    o1->mantessa[2] = (uint8_t) scratch1;
    carry = scratch1 & 0x0100;
    scratch1 = scratch2 = 0x0000;
    scratch1 = o1->mantessa[1];
    scratch2 = o2->mantessa[1];
    if (carry) {
        scratch1 += 1;
    }
    scratch1 += scratch2;
    o1->mantessa[1] = (uint8_t) scratch1;
    carry = (scratch1 & 0x0300) >>8;
    scratch1 = scratch2 = 0x0000;
    scratch1 = o1->mantessa[0];
    scratch2 = o2->mantessa[0];
    scratch1 +=carry;
    scratch1 += scratch2;
    if (scratch1 & 0x0300)
        overflow = 1;
    o1->mantessa[0] = (uint8_t) scratch1;
    if (overflow) {
        overflow = 0;
        uint8_t old_exp = o1->exponent;
        o1->exponent+=1;
        o1->mantessa[0] = o1->mantessa[0]|0x80;
        shift_right(1,o1);
        if (o1->exponent < old_exp) { //overflow in exponent
            overflow = 1;
        }
    }
    return;
}

//TODO - how to decide which goes into "FAC" if ord of negative is higher than ord of positive
uint8_t compare(unpacked_f *o1, unpacked_f *o2) {
    if (o1->exponent > o2->exponent)
        return 0;
    else if (o1->exponent < o2->exponent)
        return 1;
    if (o1->mantessa[0] > o2->mantessa[0])
        return 0;
    else if (o1->mantessa[0] < o2->mantessa[0] )
        return 1;
    if (o1->mantessa[1] > o2->mantessa[1])
        return 0;
    else if (o1->mantessa[1] < o2->mantessa[2])
        return 1;
    if (o1->mantessa[2] > o2->mantessa[2])
        return 0 ;
    else if (o1->mantessa[2] < o2->mantessa[2])
        return 1;
    return 3;

}

void shift_left(unpacked_f *o1)
{
    uint8_t high_bit = 0;
    high_bit = o1->mantessa[3] & 0x80;
    o1->mantessa[3] = o1->mantessa[3] << 1;
    for (int i = 2; i >=0; i--){
        if (high_bit) {
            high_bit = o1->mantessa[i] & 0x80;
            o1->mantessa[i] = o1->mantessa[i] << 1;
            o1->mantessa[i] = o1->mantessa[i] | 0x01;
        }
        else {
            high_bit = o1->mantessa[i] & 0x80;
            o1->mantessa[i] = o1->mantessa[i] << 1;
            o1->mantessa[i] = o1->mantessa[i] | 0x00;
        }
    }

}
//normalize
void normalize(unpacked_f *o1) {
//get a one into the MSB of the mantessa. Shift from underflow byte
    uint8_t shift_count = 0;
    uint8_t zero = 1;
    //normal?
    if (o1->mantessa[0] & 0x80) {
            return;
    }
    //zero?
    for (uint8_t i=0; i<4;i++) {
        if (o1->mantessa[i]!=0)
            zero = 0;
        else break;
    }
    if (zero) {
        o1->exponent = 0;
        return;
    }
    //already checked for zero

    while(1) {
        shift_count++;
        shift_left(o1);
        if (o1->mantessa[0] & 0x80)
            break;
    }
    if (shift_count >= o1->exponent) {
        o1->exponent = 0;
    }
    else{
        o1->exponent = o1->exponent - shift_count;
    }

}


//round
void round_f(unpacked_f *o1) {
    uint16_t scratch = o1->exponent;
    if (o1->mantessa[3] & 0x80) {
        increment(o1);
        if (overflow) {
            overflow=0;
            shift_right(1,o1);
            o1->mantessa[0] = o1->mantessa[0] | 0x80;
            scratch++;
            o1->exponent = (uint8_t)scratch;
            if (scratch &0x0100)
                overflow = 1;
        }

    }
}

//zero the exponent
inline void zero(uint32_t *t) {
    uint8_t *p = (uint8_t *) t;
    *p = 0;
}

//when this is called, it definitely matters which of these was in the "fac"
packed_f add_f(packed_f f1, packed_f f2) {
    unpacked_f o1, o2;
    unpacked_f *registers, *fac;
    unpack(f1, &o1);
    unpack(f2, &o2);
    if (o1.exponent > o2.exponent) {
        registers = &o2;
        fac = &o1;
    }
    else {
        registers = &o1;
        fac = &o2;
    }
    //align
    uint8_t shift = fac->exponent - registers->exponent;
    if (shift) shift_right(shift, registers);
    //once aligned, exponent of the registers is the same as the FAC.
    registers->exponent = fac->exponent;
    if (o1.sign != o2.sign) {
        subtract(registers, fac);
    }
    else {
        add(registers, fac);
    }

    normalize(registers);
    round_f(registers);
    return pack(registers);
}

void print_binary(int number)
{
    if (number) {
        print_binary(number >> 1);
        putc((number & 1) ? '1' : '0', stdout);

    }https://wwwCP/M.mouser.co.uk/datasheet/2/450/z8s180ps-28828.pdf
}

int main()
{
    uint32_t p1 = 0x8380F100;
    uint32_t p2 = 0x83001000;
    printf("%x", add_f(p1,p2));
    printf("Hello world!\n");
    return 0;
}
