#include "lz77.h"

int print_test(void* _buff, size_t _type_size, size_t _buff_count)
{
    LzNode* node = EncodeLZ77(_buff, _type_size * _buff_count);

    
    while(node)
    {
        printf("LDL : %d, %d, %c \n", node->Length, node->Distance, node->Literal);
        LzNode* next = node->Next;
        free(node);
        node = next;
    }

    return 1;
}

int main(void)
{
    char* str = "abcdabcdabcc";
    print_test(str, sizeof(char), strlen(str));
    str = "abcdabcdabc";
    print_test(str, sizeof(char), strlen(str));
    str = "abcdefaababc";
    print_test(str, sizeof(char), strlen(str));

    return 0;
}