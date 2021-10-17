#include "lz77.h"


int main(void)
{

    char* str = "abcdabcdabcc";
    EncodeLZ77(str, strlen(str));

    str = "abcdabcdabc"; // failed at this point
    EncodeLZ77(str, strlen(str));

    str = "abcdefaababc"; // failed at this point
    EncodeLZ77(str, strlen(str));

    return 0;
}