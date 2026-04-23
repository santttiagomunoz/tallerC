#include <stdio.h>
#include <stdlib.h>


#pragma pack(push, 1)
typedef struct Header
{

    short magicString;
    int cantStudents;
    int cantCourses;
    int cantMatriculas;

} Header;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct Student
{

    int studentID;
    __uint8_t flag;
    char name[23];
    int age;

} Student;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct Course
{

    int courseID;
    char name[(sizeof(int) * 8)];
    int hoursumber;

} Course;

#pragma pack(push, 1)
typedef struct Matricula
{

    int stutudentID;
    int courseID;
    int yearMatriculado;

} Matricula;
#pragma pack(pop)


int main(int argc, char *argv[])
{

     char *file_name = argv[1];
     int minAge = atoi(argv[2]);
     int maxAge = atoi(argv[3]);

    FILE *fp = fopen(file_name, "rb");
    Student *student;
    Header header;
    Course course;
    Matricula Matricula;

    if (fp == NULL)
        return 1;

    size_t headerLeido = fread(&header, sizeof(Header), 1, fp);
    if (headerLeido != 0)
    {
        printf("%x, %d, %d, %d\n", header.magicString, header.cantStudents, header.cantCourses, header.cantMatriculas);
    }

    student = malloc(sizeof(Student) * 1000);
    fread(student, sizeof(Student), 1000, fp);

    printf("\n%-23.23s %-6s %-6s\n", "Name", "Age", "Gender");
    for (int i = 0; i < 1000; i++)
    {
        if (student[i].age >= minAge && student[i].age <= maxAge)
        {
            unsigned char b = student[i].flag;

            int gender = (b >> 7) & 1;
            int graduate = (b >> 6) & 1;

            printf("%-23.23s %-6d %s\n", student[i].name, student[i].age, (gender == 1 ? "femenino" : "masculino"));
        }
    }

    fclose(fp);    
    free(student);
    return 0;
}