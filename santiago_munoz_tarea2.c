#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct Header
{
    // es posible usar _int16_t, sin embargo usare short para 2 bytes
    //  es posible usar un tipo de datos con una cantidad especifica de bytes como _int16_t o _uint32_t para mas presicion
    short magicString;
    int studentCant;
    int courseCant;
    int matriculaCant;
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
    __int32_t courseID;
    char name[32];
    __int32_t hourNumber;

} Course;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct Matricula
{
    int studentID;
    int courseID;
    int yearMatriculado;

} Matricula;
#pragma pack(pop)

int main(int argc, char *argv[])
{

    char *fileName = argv[1];
    FILE *fp = fopen(fileName, "rb");

    Header header;
    Student *student;
    Course *course;
    Matricula *matricula;

    fread(&header, sizeof(Header), 1, fp);

    student = malloc(sizeof(Student) * header.studentCant);
    fread(student, sizeof(Student), header.studentCant, fp);

    course = malloc(sizeof(Course) * header.courseCant);
    fread(course, sizeof(Course), header.courseCant, fp);

    // for(int i = 0; i < header.courseCant; i++){
    //     printf("%d %.32s\n",course[i].courseID, course[i].name);
    // }

    matricula = malloc(sizeof(Matricula) * header.matriculaCant);
    fread(matricula, sizeof(Matricula), header.matriculaCant, fp);

    printf("\n%-23.23s %-6s\n", "course", "average Age");
    for (int i = 0; i < header.courseCant; i++)
    {
        int sumAges = 0;
        int studentSumPerCourse = 0;

        for (int j = 0; j < header.matriculaCant; j++)
        {
            if (course[i].courseID != matricula[j].courseID)
            {
                continue;
            }

            for (int s = 0; s < header.studentCant; s++)
            {

                if (student[s].studentID == matricula[j].studentID)
                {
                    sumAges += student[s].age;
                    studentSumPerCourse++;
                    break;
                }
            }
        }
        float promedio = (float)sumAges / studentSumPerCourse;
        printf("%d %-23.23s %.2f\n", course[i].courseID, course[i].name, promedio);
    }

    free(student);
    free(course);
    free(matricula);

    return 0;
}