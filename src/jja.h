#ifndef JJA
#define JJA

#define TH_PER_BLOCK 128
#define TH_PER_BLOCK_MINUS_ONE 127

// for block representation
#define BLOCK_SIZE 8
#define BLOCK_SIZE_M1 7

#define ffloat double
#define parse_ffloat(val) strtod(val,  NULL)
#define sin_ffloat(val) sin(val)
#define cos_ffloat(val) cos(val)

#endif

