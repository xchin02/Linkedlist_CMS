#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> // Included in original
#include <stdlib.h> // Added for general utilities
#include "include/showsummary.h"

float average_marks(student_node *head, int num_records) {
    float total_marks = 0.0f;
    student_node *average_ptr = head;
    while (average_ptr != NULL) {
        total_marks += average_ptr->student_data.Mark;
        average_ptr = average_ptr->next_student;
    }
    return total_marks / num_records;
}
//retrieve the highest and lowest marks
void get_extreme_marks(student_node *head, char *highest_student, char *lowest_student, float *max, float *min) {
    //set both highest and lowest to first student's mark
    student_node *extremes_ptr = head;
    *max = extremes_ptr->student_data.Mark;
    *min = extremes_ptr->student_data.Mark;
    strcpy(highest_student, extremes_ptr->student_data.Name);
    strcpy(lowest_student,  extremes_ptr->student_data.Name);
    //retrieve highest and lowest student's mark respectively
    while (extremes_ptr != NULL) {
        if (extremes_ptr->student_data.Mark > *max) {
            *max = extremes_ptr->student_data.Mark;
            strcpy(highest_student, extremes_ptr->student_data.Name);
        }
        else if (extremes_ptr->student_data.Mark < *min) {
            *min = extremes_ptr->student_data.Mark;
            strcpy(lowest_student, extremes_ptr->student_data.Name);
        }
        extremes_ptr = extremes_ptr->next_student;
    }
}

//just puts everything together and print summary details
void show_summary(student_node *head, int total_students) {
    char highest_student[MAX_NAME_LENGTH] = "";
    char lowest_student[MAX_NAME_LENGTH] = "";
    float max = 0;
    float min = 0;
    float average_student_marks = average_marks(head, total_students);
    get_extreme_marks(head, highest_student, lowest_student, &max, &min);
    puts("-=-=-=-=-SUMMARY STATISTICS-=-=-=-=-");
    printf("Total number of students: %d\n", total_students);
    printf("Average mark of students: %.1f\n", average_student_marks);
    printf("Highest mark among students: %s-%.1f\n", highest_student, max);
    printf("Lowest mark among students: %s-%.1f\n", lowest_student, min);
}