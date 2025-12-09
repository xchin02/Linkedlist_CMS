#ifndef VIEWSUMMARY_H
#define VIEWSUMMARY_H
#include "constants.h"
#include "structs.h"

float average_marks(student_node *head, int num_records);
void get_extreme_marks(student_node *head, char *highest_student, char *lowest_student, float *max, float *min);
void show_summary(student_node *students_records, int total_students);

#endif
