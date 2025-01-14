#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "html.h"

int correct_name(const char * string, int count) {
    if (string[count] != ' ') {
        for (int i = 0; i < count && string[count] != ' ' && string[count] != '=' && string[count] != '>'; ++i) {
            if (!isalpha(string[count])) {
                printf("Incorrect name tag or name attribute\n");
                return EXIT_FAILURE;
            } count++;
        }
    } else {
        printf("Incorrect name tag\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

html * html_decoder(const char * string, int * size) {
    size_t size_tag = 1; int count = 0;
    html * tag_ptr = (html*) calloc(size_tag, sizeof(html));
    if (NULL == tag_ptr) {
        printf("Allocation problem\n");
        return 0;
    }

    char data[SIZE] = {'\0'};
    char value[SIZE_VALUE] = {'\0'};

    while (string[count] == ' ')
        count++;

    if (string[count] !='<') {
        printf("String is null or incorrect\n");
        strcpy(tag_ptr->name, "0\0"); return tag_ptr;
    }

    if (string[count+1] != '/') {
        if (correct_name(string, count + 1)) {
            strcpy(tag_ptr->name, "0\0"); return tag_ptr;
        }
    } else {
        if (correct_name(string, count + 2)) {
            strcpy(tag_ptr->name, "0\0"); return tag_ptr;
        }
    }
    int j = 0;
    for (int flag = 0; string[count] != ' ' && string[count] != '>' && string[count] != '\0'; ++count) {
        if (!flag) {
            flag = 1;
            if (string[count+1] == '/') {
                strcpy(tag_ptr->value, "close\0"); count++;
            } else
                strcpy(tag_ptr->value, "open\0");
            continue;
        }
        data[j++] = string[count];
    }

    strcpy(tag_ptr->name, data);
    tag_ptr->name[j] = '\0';

    (*size)++;
    if (tag_ptr[0].value[0] == 'c' || string[count] == '>' || tag_ptr[0].value[0] == '\0')
        return tag_ptr;

    while (string[count] != '>') {
        memset(data, '\0', SIZE);
        memset(value, '\0', SIZE_VALUE);

        html *new_tag = (html*) calloc(++size_tag, sizeof(html));
        if (NULL == new_tag) {
            printf("Allocation problem\n");
            free(tag_ptr);
            return 0;
        }

        memcpy(new_tag, tag_ptr, (size_tag) * sizeof(html));

        while (string[count] == ' ')
            count++;

        if (correct_name(string, count)) {
            strcpy(new_tag[0].name, "0"); return new_tag;
        }

        for (int w = 0; string[count] != ' ' && (string[count] != '=' && string[count] != '>'); ++w)
            data[w] = string[count++];

        while (string[count] == ' ')
            count++;

        if (string[count] == '=' || string[count] == '/') {
            while (string[count+1] == ' ')
                count++;

            if (string[++count] == '/') {
                for (int r = 0; string[count] != ' ' && string[count] != '>'; ++r)
                    value[r] = string[count++]; // проверка в случае action=/search
            } else {
                count++;
                for (int w = 0; string[count] != '\"'; ++w)
                    value[w] = string[count++];
            }
            count++;
        }
        strcpy(new_tag[size_tag-1].name, data);
        strcpy(new_tag[size_tag-1].value, value);
        memcpy(tag_ptr, new_tag, (size_tag) * sizeof(html));
        (*size)++;
        free(new_tag);
    }
    return tag_ptr;
}