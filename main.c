#include "lz77.h"

int print_and_free_test(void* _buff, size_t _type_size, size_t _buff_count)
{
    LzLinkedlist linked_list = EncodeLZ77(_buff, _type_size * _buff_count);
    LzNode* node = linked_list.Head;
    uint8_t* result = (uint8_t*)malloc(linked_list.SizeInByte +1);

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
    result[i] = '\0';
    printf("\n");
    if(!strcmp((const char*)_buff, (const char*)result))
        printf("Pass\n");
    else
        printf("Failed\n");
    printf("=====================================================\n");

    free(result);

    return 1;
}

int compare_and_free_test(void* _buff, size_t _type_size, size_t _buff_count)
{
    LzLinkedlist linked_list = EncodeLZ77(_buff, _type_size * _buff_count);
    LzNode* node = linked_list.Head;
    uint8_t* result = (uint8_t*)malloc(linked_list.SizeInByte +1);
    uint64_t src_index = 0;
    printf("-----------------------------------------------------\n");
    while(node)
    {
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
    result[linked_list.SizeInByte] = '\0';
    if(!strcmp((const char*)_buff, (const char*)result))
        printf("Pass\n");
    else
        printf("Failed\n");

    printf("=====================================================\n");
    free(result);

    return 1;
}

int main(void)
{
    char* str = "abcdabcdabcc";
    //compare_and_free_test(str, sizeof(char), strlen(str));
    str = "abcdabcdabc";
    print_and_free_test(str, sizeof(char), strlen(str));
    str = "abcdefaababc";
    //compare_and_free_test(str, sizeof(char), strlen(str));
    str = "aabcdeabcd";
    print_and_free_test(str, sizeof(char), strlen(str));

    fflush(stdout);

    FILE* file = fopen("MetalRoughSpheres.glb", "rb");
    fseek(file, 0L, SEEK_END);
    uint64_t file_size = ftell(file);
    rewind(file);

    uint8_t* file_buff = (uint8_t*)malloc(file_size);
    //compare_and_free_test(file_buff, sizeof(uint8_t), file_size);

    free(file_buff);
    fclose(file);

    return 0;
}