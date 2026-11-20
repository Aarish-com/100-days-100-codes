#include <stdio.h>

#define MAX_NAME 50

struct Student {
    int    roll;
    char   name[MAX_NAME];
    float  marks;
};

/* Returns the student with the highest marks */
struct Student getTopStudent(struct Student arr[], int n)
{
    struct Student top = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i].marks > top.marks) {
            top = arr[i];
        }
    }
    return top;   /* structure returned by value */
}

int main(void)
{
    int n;
    printf("Enter number of students: ");
    scanf("%d", &n);

    struct Student students[n];

    for (int i = 0; i < n; i++) {
        printf("\nStudent %d\n", i + 1);
        printf("Roll no: ");
        scanf("%d", &students[i].roll);
        printf("Name: ");
        scanf(" %49[^\n]", students[i].name);
        printf("Marks: ");
        scanf("%f", &students[i].marks);
    }

    struct Student topper = getTopStudent(students, n);

    printf("\n--- Top Student ---\n");
    printf("Roll no: %d\n", topper.roll);
    printf("Name   : %s\n", topper.name);
    printf("Marks  : %.2f\n", topper.marks);

    return 0;
}
