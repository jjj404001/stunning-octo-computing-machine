#include "lz77.h"

int print_and_free_test(void* _buff, size_t _type_size, size_t _buff_count)
{
    LzLinkedlist linked_list = EncodeLZ77(_buff, _type_size * _buff_count);
    LzNode* node = linked_list.Head;
    uint8_t* result = (uint8_t*)malloc(linked_list.SizeInByte);

    uint64_t src_index = 0;

    printf("-----------------------------------------------------\n");
    while(node)
    {
        printf("LDL : %d, %d, %c \n", node->Length, node->Distance, node->Literal);

        if(node->Length > 0)
        {
            memcpy(&result[src_index], &result[src_index - node->Distance], node->Length);
            result[src_index + node->Length] = node->Literal;
            src_index += node->Length + 1;
        }
        else
            result[src_index++] = node->Literal;

        LzNode* next = node->Next;
        free(node);
        node = next;
    }
    uint64_t i = 0;
    for(i = 0; i < linked_list.SizeInByte; i++)
        printf("%c", result[i]);
    printf("\n");

    printf("=====================================================\n");

    free(result);

    return 1;
}

int main(void)
{
    char* str = "abcdabcdabcc";
    //print_and_free_test(str, sizeof(char), strlen(str));
    str = "abcdabcdabc";
    print_and_free_test(str, sizeof(char), strlen(str));
    str = "abcdefaababc";
    print_and_free_test(str, sizeof(char), strlen(str));
    str = "aabcdeabcd";
    print_and_free_test(str, sizeof(char), strlen(str));

    return 0;
}