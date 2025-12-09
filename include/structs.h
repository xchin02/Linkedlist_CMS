#ifndef STRUCTS_H
#define STRUCTS_H
#include "constants.h"

// Database structure: ID, Name, Programme, Mark
// typedef creates an alias for the struct type, helps to keep the code short and clear so there is no need to type struct StudentRecords anymore
typedef struct {
    int ID;
    char Name[MAX_NAME_LENGTH]; // String of 50 characters
    char Programme[MAX_PROGRAMME_LENGTH]; // String of 50 characters
    float Mark;
} StudentRecords; // Name of struct

typedef struct student {
    StudentRecords student_data;
    struct student *next_student;
} student_node;

#endif