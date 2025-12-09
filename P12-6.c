#include <stdio.h>  // Standard input/output
#include <stdlib.h> // Conversions, memory and exit control
#include <string.h> // Comparing and copying strings
#include <ctype.h>  // Input validation/character conversion
#include <stdbool.h>
#include "include/showsummary.h"

student_node *create_student_node(StudentRecords individual_student);

bool isValidID(int id)
{
    if (id > LOWEST_ID && id < HIGHEST_ID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Function to open file
void openFile(StudentRecords *student_records, int *studentCount, student_node **head)
{                                              // File pointer to store file data like filename, location, mode and current position of the file
    FILE *open_ptr = fopen(DB_FILE_NAME, "r"); // Open file in read mode
    char line[MAX_FILE_LINE_LENGTH];           // Buffer to store each line of the file
    if (open_ptr == NULL)
    {
        printf("Database file not found.\n"); // If file does not exists, then print error message
        return;
    }

    *studentCount = 0; // Reset counter to 0 in case the file is opened multiple times, so when the user enters the same command again, it will reset and no records will duplicate

    if (open_ptr != NULL)
    {
        while (fgets(line, sizeof(line), open_ptr))
        {
            if (!isdigit(*line))
            { // Ignores lines in database that are not numerical, we want to get student id number, meaning a valid student record
                continue;
            }
            else
            { // first character is a digit indicate its a student ID number so its a student record to create individual nodes for them
                sscanf(line, "%d\t%[^\t]\t%[^\t]\t%f", &student_records[*studentCount].ID,
                       student_records[*studentCount].Name, student_records[*studentCount].Programme,
                       &student_records[*studentCount].Mark);
                // creates student nodes for existing student records in text file
                // printf("%-10d %-20s %-30s %-10.1f", student_records[*studentCount].ID, student_records[*studentCount].Name, student_records[*studentCount].Programme, student_records[*studentCount].Mark);
                student_node *existing_student = create_student_node(student_records[*studentCount]);
                if (existing_student == NULL)
                { // if malloc does fail
                    printf("Memory allocation failed.\n");
                    return;
                }

                // link into list
                if (*head == NULL)
                {
                    // first node
                    *head = existing_student;
                }
                else
                {
                    // find the last node(tail) and append new_student to linkedlist
                    student_node *current_ptr = *head;
                    while (current_ptr->next_student != NULL)
                    {
                        current_ptr = current_ptr->next_student; // traverse the linkedlist until last node(tail), where next_student points to nothing/NULL
                    }
                    current_ptr->next_student = existing_student; // set last node(tail) pointer to point to new_student node
                }
            }
            (*studentCount)++;
        }
    }
    fclose(open_ptr); // Close file after reading
    printf("%s successfully opened.\n", DB_FILE_NAME);
}

// Function to show all records
void showAll(int *studentCount, student_node *head)
{
    if (*studentCount == 0)
    {
        printf("No records found.\n"); // If record is empty, then print error message
        return;
    }
    printf("%-10s %-20s %-30s %-10s\n", "ID", "Name", "Programme", "Mark");
    // for (int i = 0; i < studentCount; i++) { // Tracks how many records are loaded and uses studentCount as the limit
    //     printf("%-10d %-20s %-30s %-10.1f\n", students[i].ID, students[i].Name, students[i].Programme, students[i].Mark); // Print according to ID, Name, Programme, Mark
    // };
    student_node *print_student_node = head; // creates a pointer that points to address of first node of linkedlist
    while (print_student_node != NULL)
    {
        // prints out student records for all nodes
        printf("%-10d %-20s %-30s %-10.1f\n", print_student_node->student_data.ID,
               print_student_node->student_data.Name, print_student_node->student_data.Programme,
               print_student_node->student_data.Mark);
        print_student_node = print_student_node->next_student; // print_student_node is now the next node for printing
    }
}

// Function to insert new record
void insertData(char *userInput, int *studentCount, student_node **head)
{
    int id = 0;
    const char *ptr_Name = "Name=";
    const char *ptr_Prog = "Programme=";
    float mark = 0;
    StudentRecords insert_student[MAX_STUDENTS];

    // points after first instance of = in string
    char *start_ID = strchr(userInput, '=') + 1;

    // convert id string to integer
    id = atoi(start_ID);

    // to check if ID already exists in linked list
    student_node *current = *head;
    while (current != NULL)
    {
        if (current->student_data.ID == id)
        {
            printf("CMS: The record with ID=%d already exists.\n", id);
            return; // no insert, regardless of whether Name/Programme/Mark is provided because id alr exists
        }
        current = current->next_student;
    }

    char *start_Mark = strstr(userInput, "Mark=");
    char *start_name = strstr(userInput, ptr_Name);
    char *start_programme = strstr(userInput, ptr_Prog);

    // user only types ID that does exist but no programme/name/mark fields provided
    if (start_name == NULL || start_programme == NULL || start_Mark == NULL)
    {
        printf("CMS: Please enter in proper format E.g. INSERT ID=<id> Name=<name> Programme=<programme name> Mark=<mark>\n", id);
        return;
    }
    else
    {
        // retrieve student's name between Name= and Programme=
        char *end_name, *end_programme;
        char *target_string = NULL;
        if (start_name != NULL)
        {
            start_name += strlen(ptr_Name); // start_name now points to actual name
            if (end_name = strstr(userInput, ptr_Prog))
            {
                target_string = (char *)malloc(end_name - start_name + 1);
                if (target_string == NULL)
                {
                    puts("CMS: Memory allocation failed");
                    return;
                }
                memcpy(target_string, start_name, end_name - start_name);
                target_string[end_name - start_name] = '\0';
            }
        }
        // store new name into temporary array
        strncpy(insert_student[0].Name, target_string, MAX_NAME_LENGTH);
        // clear string for storing programme string
        target_string[0] = '\0';

        // retrieve student's name between Programme= and Mark=
        if (start_programme != NULL)
        {
            start_programme += strlen(ptr_Prog); // start_programme now points to actual programme
            // reallocate the memory for storing the longer programme string
            target_string = realloc(target_string, start_Mark - start_programme + 1);
            if (target_string == NULL)
            {
                puts("CMS: Memory allocation failed");
                return;
            }
            memcpy(target_string, start_programme, start_Mark - start_programme);
            target_string[start_Mark - start_programme] = '\0';
        }
        // store new programme into a temporary array
        strncpy(insert_student[0].Programme, target_string, MAX_PROGRAMME_LENGTH);
        free(target_string);

        // convert mark string to float
        mark = atof(strstr(start_Mark, "=") + 1);

        // store id and mark into temporary array
        insert_student[0].ID = id;
        insert_student[0].Mark = mark;

        // create a new student_node for storing into linkedlist
        student_node *new_student = create_student_node(insert_student[0]);
        // linkedlist is empty and then insert new student node as head of linkedlist
        if (*head == NULL)
        {
            *head = new_student;
        }
        // traverse linkedlist until find last node then insert
        else
        {
            student_node *insert_ptr = *head;
            while (insert_ptr->next_student != NULL)
            {
                insert_ptr = insert_ptr->next_student;
            }
            insert_ptr->next_student = new_student;
        }
    }
    (*studentCount)++; // Add 1 to studentCount after storing the data
    printf("CMS: New record with ID=%d successfully created.\n", id);
}

// Function to search for records
void queryData(char *userInput, student_node *head)
{
    int queryID = 0;
    bool query_found = false;
    // to retrieve the userID from the format stated
    queryID = atoi(strchr(userInput, '=') + 1);
    if (queryID == 0)
    {
        printf("CMS: Invalid format. Use QUERY ID=NUMBER.\n");
    }
    else
    {
        if (isValidID(queryID) == false)
        {
            printf("CMS: Invalid ID. Please enter a 7-digit number.\n");
        }

        student_node *query_pointer = head; // points to first node of linkedlist
        while (query_pointer != NULL)
        {
            // if node has an ID matching the queryID
            if (query_pointer->student_data.ID == queryID)
            {
                printf("CMS: The record with ID=%d is found in the data table.\n", queryID);
                printf("%-10s %-20s %-30s %-10s\n", "ID", "Name", "Programme", "Mark");
                printf("%-10d %-20s %-30s %-10.1f\n", query_pointer->student_data.ID, query_pointer->student_data.Name,
                       query_pointer->student_data.Programme, query_pointer->student_data.Mark);
                query_found = true;
                return;
            }
            else
            {
                query_pointer = query_pointer->next_student;
            }
        }
        if (query_found == false)
        {
            printf("CMS: The record with ID=%d does not exists.\n", queryID);
            return;
        }
        else
        {
            query_found = false; // this is to reset to original value for future queries
        }
    }
}
// Function to update records
void updateData(char *userInput, student_node *head)
{
    // local variables needed for the functions
    int updateID = 0;
    float newMark = 0.0f;
    bool id_found = false;
    char newProgramme[MAX_PROGRAMME_LENGTH];
    student_node *update_pointer = head;

    // %*[ \t] --> %* means: read but DON'T store (discard)
    //[ \t] means: match any combination of spaces and/or tabs
    // %49[^\n] - %49 means: read up to 49 characters (leaves room for null terminator) and [^\n] means: read any character EXCEPT newline

    if (sscanf(userInput, "UPDATE ID=%d%*[ \t]Programme=%49[^\n]", &updateID, &newProgramme) == 2)
    {
        if (isValidID(updateID) == false)
        {
            printf("CMS: Invalid ID. Please enter a 7-digit number.\n");
            return;
        }
        else
        {
            while (update_pointer != NULL)
            {
                // if node has an ID matching the updateID
                if (update_pointer->student_data.ID == updateID)
                { // updates the programme of selected id
                    printf("CMS: The record with ID=%d is successfully updated as shown below\n", updateID);
                    printf("%-10s %-20s %-30s %-10s\n", "ID", "Name", "Programme", "Mark");
                    strncpy(update_pointer->student_data.Programme, newProgramme, MAX_PROGRAMME_LENGTH - 1);
                    printf("%-10d %-20s %-30s %-10.1f\n", update_pointer->student_data.ID, update_pointer->student_data.Name,
                           update_pointer->student_data.Programme, update_pointer->student_data.Mark);
                    id_found = true;
                    return;
                }
                else
                {
                    update_pointer = update_pointer->next_student;
                }
            }
        }
        if (id_found == false)
        {
            printf("CMS: The record with ID=%d does not exist.\n", updateID);
            return;
        }
    }

    else if (sscanf(userInput, "UPDATE ID=%d%*[ \t]Mark=%fO[^\n]", &updateID, &newMark) == 2)
    {
        // check for valid 7 digit number
        if (isValidID(updateID) == false)
        {
            printf("CMS: Invalid ID. Please enter a 7-digit number.\n");
            return;
        }
        if (newMark < MIN_MARK || newMark > MAX_MARK)
        {
            printf("CMS: Marks must be between 0 and 100.\n");
            return;
        }
        // update the marks for the student
        while (update_pointer != NULL)
        {
            // if node has an ID matching the updateID
            if (update_pointer->student_data.ID == updateID)
            {
                printf("CMS: The record with ID=%d is successfully updated as shown below\n", updateID);
                printf("%-10s %-20s %-30s %-10s\n", "ID", "Name", "Programme", "Mark");
                update_pointer->student_data.Mark = newMark;
                printf("%-10d %-20s %-30s %-10.1f\n", update_pointer->student_data.ID, update_pointer->student_data.Name,
                       update_pointer->student_data.Programme, update_pointer->student_data.Mark);
                id_found = true;
                break;
            }
            else
            {
                update_pointer = update_pointer->next_student;
            }
        }
        if (id_found == false)
        {
            printf("CMS: The record with ID=%d does not exists.\n", updateID);
        }
    }
    else
    {
        // if user never enter the valid fields
        printf("CMS: Please enter a valid command UPDATE ID=<validID> Programme=<Name> or UPDATE ID=<validID> Marks:<marks>\n");
    }
    if (id_found == true)
    {
        id_found = false; // this is to reset to original value for future queries
    }
}

// Function to delete records
void deleteData(char *userInput, int *student_count, student_node **head)
{
    // local variable needed for delete function
    int deleteID = 0;

    // this pointer is used for traversing the nodes, it kinda acts like a cursor
    student_node *delete_ptr = *head;

    student_node *previous_ptr = NULL;
    /* this will point to the node before delete_ptr is pointing to currently.
    its currently set to NULL, so that if lets say delete_ptr is pointing to first node of linkedlist.
    it means that there is no node before that, thus NULL*/

    // retrieve the ID requested from the user
    if (sscanf(userInput, "DELETE ID=%d", &deleteID) == 1)
    {
        if (isValidID(deleteID) == false)
        {
            printf("CMS: Invalid ID. Please enter a 7-digit number.\n");
            return;
        }

        // traverses the linked list, when it finds the correct ID it will stop the loop
        while (delete_ptr != NULL && delete_ptr->student_data.ID != deleteID)
        {
            previous_ptr = delete_ptr;
            delete_ptr = delete_ptr->next_student;
        }
        // in the case where linkedlist does not contain the id
        if (delete_ptr == NULL)
        {
            // Not found
            printf("CMS: The record with ID=%d does not exist.\n", deleteID);
            return;
        }

        printf("CMS: Are you sure you want to delete record with ID=%d? Type 'Y' to Confirm or type 'N' to cancel\n", deleteID);
        printf("User: ");
        char userConfirm = 'A';
        // gets the users options
        scanf("%c", &userConfirm);
        while (getchar() != '\n' && getchar() != EOF); //clear buffer

        // if users type N (indicates No), then cancel deletion and return main loop
        if (toupper((unsigned char)userConfirm) == 'N')
        {
            printf("CMS: The deletion is cancelled.\n");
            return;
        }
        // if user options is Y (indicates yes), then delete record
        else if (toupper((unsigned char)userConfirm) == 'Y')
        {
            if (previous_ptr == NULL)
            { // if previous node is NULL it means that there is no node before delete_ptr indicating its the head of linkedlist
                // deleting the head node by setting it as next node in linked list
                *head = delete_ptr->next_student;
            }
            else
            {
                // deleting a middle or last node
                previous_ptr->next_student = delete_ptr->next_student;
            }
            printf("CMS: The record with ID=%d is successfully deleted\n", deleteID);
            (*student_count)--;
            return;
        }
    }
    else
    {
        printf("Please enter a valid format: DELETE ID=<validID>\n");
    }
}

// Function to save file
void saveFile(student_node *head)
{
    FILE *f = fopen(DB_FILE_NAME, "w"); // Open file in write mode
    student_node *save_pointer = head;
    if (f == NULL)
    {
        printf("Failed to save the database file.\n"); // If unable to save file, then print error message
        return;
    }
    // fprintf() is used to prints the contents to file
    fprintf(f, "Database Name: CMS_xchin02\nAuthors: xchin02\n\n"); // Telling fprintf() to write the contents to f
    fprintf(f, "Table Name: StudentRecords\n");
    fprintf(f, "ID\tName\tProgramme\tMark\n");
    while (save_pointer != NULL)
    {
        fprintf(f, "%d\t%s\t%s\t%.1f\n", save_pointer->student_data.ID, save_pointer->student_data.Name, save_pointer->student_data.Programme, save_pointer->student_data.Mark);
        save_pointer = save_pointer->next_student;
    }
    fclose(f); // Close file after saving
    printf("File successfully saved.\n");
}

// creates a student node where its size dynamically stretches according to student_data size
student_node *create_student_node(StudentRecords individual_student)
{
    student_node *new_student = (student_node *)malloc(sizeof(student_node));
    if (new_student == NULL)
    {
        return NULL;
    }
    new_student->student_data = individual_student;
    new_student->next_student = NULL;
    return new_student;
}

int main()
{
    puts("=======Class Management System=======");
    puts("Available commands: OPEN, SHOW ALL, INSERT, QUERY, UPDATE, DELETE, SAVE, EXIT");

    char userInput[USER_INPUT_LENGTH];
    student_node *head = NULL;
    StudentRecords students[MAX_STUDENTS];
    bool openCheck = false;
    int studentCount = 0;
    while (1)
    {
        // Infinite loop
        printf("User: ");
        fgets(userInput, sizeof(userInput), stdin);

        userInput[strcspn(userInput, "\n")] = '\0';

        if (strncasecmp(userInput, "OPEN", strlen("OPEN")) == 0)
        { // If userInput matches with OPEN, then run the open file function
            openFile(students, &studentCount, &head);
            openCheck = true;
        }
        else if (strncasecmp(userInput, "SHOW ALL", strlen("SHOW ALL")) == 0)
        { 
            if (openCheck == false)
            {
                printf("No records found. Please open a file using the OPEN command first.\n"); // If user forgot to open a file first
            }
            else
            {
                showAll(&studentCount, head);
            }
        }
        else if (strncasecmp(userInput, "INSERT ID=", strlen("INSERT ID=")) == 0)
        { 
            if (openCheck == false)
            {
                printf("No records found. Please open a file using the OPEN command first.\n"); // If user forgot to open a file first
            }
            else 
            {
                insertData(userInput, &studentCount, &head);
            }
        }
        else if (strncasecmp(userInput, "SAVE", strlen("SAVE")) == 0)
        { 
            saveFile(head);
        }
        else if (strncasecmp(userInput, "EXIT", strlen("EXIT")) == 0)
        { 
            return 0;
        }
        else if (strncasecmp(userInput, "QUERY ID=", strlen("QUERY ID=")) == 0)
        { 
            if (openCheck == false)
            {
                printf("No records found. Please open a file using the OPEN command first.\n"); // If user forgot to open a file first
            }
            else
            {
                queryData(userInput, head);
            }
        }
        else if (strncasecmp(userInput, "UPDATE ID=", strlen("UPDATE ID=")) == 0)
        {
            if (openCheck == false)
            {
                printf("No records found. Please open a file using the OPEN command first.\n"); // If user forgot to open a file first
            }
            else 
            {
                updateData(userInput, head);
            }
        }
        else if (strncasecmp(userInput, "DELETE ID=", strlen("DELETE ID=")) == 0)
        {
            if (openCheck == false)
            {
                printf("No records found. Please open a file using the OPEN command first.\n"); // If user forgot to open a file first
            }
            else 
            {
                deleteData(userInput, &studentCount, &head);
            }
        }
        else if (strncasecmp(userInput, "SHOW SUMMARY", strlen("SHOW SUMMARY")) == 0) 
        {
            if (openCheck == false)
            {
                printf("No records found. Please open a file using the OPEN command first.\n"); // If user forgot to open a file first
            }
            else 
            {
                show_summary(head, studentCount);
            }
        }
        else
        {
            printf("Invalid command.\n");
        }
    }
}