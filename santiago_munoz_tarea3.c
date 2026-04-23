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
    __int32_t age;
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
    __int32_t semester;

} Matricula;
#pragma pack(pop)


int main()
{

    FILE *fp = fopen("sample_data.bin", "rb");

    Header header;
    Student *student;
    Course *course;
    Matricula *matricula;

    fread(&header, sizeof(Header), 1, fp);

    student = malloc(sizeof(Student) * header.studentCant);
    fread(student, sizeof(Student), header.studentCant, fp);

    course = malloc(sizeof(Course) * header.courseCant);
    fread(course, sizeof(Course), header.courseCant, fp);

    matricula = malloc(sizeof(Matricula) * header.matriculaCant);
    fread(matricula, sizeof(Matricula), header.matriculaCant, fp);

    int idStudents[1000];
    int count = 0;

        int femalesPre = 0;
        int malesPre = 0;
        int femalespos = 0;
        int malesPos = 0;


    printf("year semester male undergrad female undergrad male grad female grad\n");
    for (int i = 0; i < header.matriculaCant; i++)
    {
        

        for (int s = 0; s < header.studentCant; s++)
        {

            if (student[s].studentID == matricula[i].studentID)
            {

                if (matricula[i].yearMatriculado == 2020 && matricula[i].semester == 1)
                {
                    unsigned char genGra = student[s].flag;
                    int gender = (genGra >> 7) & 1;
                    int graduate = (genGra >> 6) & 1;

                    // (gender == 1 ? females++ : males++);
                    // (graduate == 1 ? "posgrado" : "pregrado");

                    if(gender == 0 && graduate == 1){
                        malesPos++;
                    }

                    if(gender == 1 && graduate == 1){
                        femalespos++;
                    }

                    if(gender == 0 && graduate == 0){
                        malesPre++;
                    }

                    if(gender == 1 && graduate == 0){
                        femalesPre++;
                    }

                    break;
                }
            }
        }
    }
printf("2020 1 %d %d %d %d\n", malesPre, femalesPre, malesPos, femalespos);



    return 0;
}