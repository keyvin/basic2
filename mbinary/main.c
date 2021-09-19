#include <stdio.h>
#include <stdlib.h>

typedef struct UNPACKED_FLOAT {
    uint8_t exponent;
    bool is_negative;
    uint8_t mantessa[4]; //byte 4 is the underflow/buffer byte used for rounding.
} unpacked_f;

typedef struct FAC {
    uint8_t[4];
}

//stored little endian
bool overflow;

//passed in with the sign bit in mantessa replaced with 1
inline void increment3(uint8_t *t) {
    uint16_t scratch = 0;
    //set true to increment the first
    overflow = true;
    for (int i = 2; i>0; i++) {
        scratch = 0;
        if (overflow) {
            scratch = *(t+i);
            scratch++;
        }
        if (scratch > 255) {
            overflow = true;
        }
        else {
            overflow = false;
        }
    }
    //overflow set true if the increment resulted in overflow
}

void unpack (uint32_t *t, unpacked_f *result) {
    //will be stored in reverse order
    uint8_t *p = (uint8_t *) t;
    if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) {
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
            target->mantessa[0] = target->mantessa[0] & 7F;
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
    if (scratch &0x0100 ==0) borrow = 1;
    o1->mantessa[2] = (uint8_t) scratch;
    scratch = o2->mantessa[1] | 0x0100;
    if (borrow) {
        scratch--;
        borrow = 0;
    }
    scratch -= o2->mantessa[1]
    o1->mantessa[1] = (uint8_t) scratch;
    if (scratch & 0x0100 ==0) borrow =1;
    scratch = 0;
    scratch = o2->mantessa[0] |0x0100;
    if (borrow){
        scratch--;
        borrow = 0;
    }
    scratch-= o1->mantessa[0];
    if (scratch & 0x0100 ==0) borrow = 1;
    o1->mantessa[0] = (uint8_t) scratch;
    //result was negative. Negate mantessa
    //TODO - sign bit?
    //in 8080 basic, the sign bit is supposed to match the FAC(o2).
    //we will keep the sign in o1 instead
    if(borrow)
    {
        o1->is_negative = !o2->is_negative;
        negate_f(o1);
    }

}

//o1 will have the underflow byte set. o1 is value in registers in 8080
//o1 is where the result is stored
void add(unpacked_f *o1, unpacked_f *o2) {
    //should already be aligned. There is no carry check in C.
    //use bigger uint and check the high bit for carry
    uint8_t carry =0;
    uint8_t carry_carry = 0;
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
        overflow = true;
    o1->mantessa[0] = (uint8_t) scratch1;
    if (overflow) {
        overflow = false;
        uint8_t old_exp = o1->exponent;
        o1->exponent+=1;
        shift_right(1,o1);
        if (o1->exponent < old_exp) { //overflow in exponent
            overflow = true;
        }
    }
    return;
}

//TODO - how to decide which goes into FAC if ord of negative is higher than ord of positive
uint8_t compare(unpacked_f o1, unpacked_f o2) {
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

//normalize
void normalize(unpacked_f o1);


//round
void round(unpacked_f o1);

//zero the exponent
inline void zero(uint32_t *t) {
    uint8_t *p = (uint8_t *) t;
    *p = 0;
}

int main()
{
    printf("Hello world!\n");
    return 0;
}
