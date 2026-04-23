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

void printTableLine(Header *header, Matricula *matricula, Student *student, int year, int semester)
{

    int femalesPre = 0;
    int malesPre = 0;
    int femalespos = 0;
    int malesPos = 0;
    int countStudents = 0;
    int studentsCounted[1000];

    for (int i = 0; i < header->matriculaCant; i++)
    {
        //preguntamos si el año de la stuct matricula es diferente del año que se le pasa por parametro
        //o si la stuct matricula es diferente del semestre que se le pasa por parametro(la matricula tambien se la pasa como parametro)
        //si alguno de los 2 no coincide va a la siguiente vuelta
        if (matricula[i].yearMatriculado != year || matricula[i].semester != semester)
            continue;

        for (int s = 0; s < header->studentCant; s++)
        {
            //preguntamos si el id de estudiante coicinde con el id de estudiante que esta en matricula
            if (student[s].studentID == matricula[i].studentID)
            {
                //preguntamos si el estudiante existe(si existe se incrementa studentExist y se sale del bucle)
                int studentExist = 0;
                for (int u = 0; u < countStudents; u++)
                {
                    if (studentsCounted[u] == student[s].studentID)
                    {
                        studentExist = 1;
                        break;
                    }
                }
                //si el estuiante no existe(studentExist == 0) se cuennta un estudiante
                if (studentExist == 0)
                {
                    
                    studentsCounted[countStudents] = student[s].studentID;
                    countStudents++;
                    //operacion binaria para comparar el valor del bit 7 y 6 de la bandera
                    unsigned char genGra = student[s].flag;
                    int gender = (genGra >> 7) & 1;
                    int graduate = (genGra >> 6) & 1;

                    // (gender == 1 ? females++ : males++);
                    // (graduate == 1 ? "posgrado" : "pregrado");

                    if (gender == 0 && graduate == 1)
                    {
                        malesPos++;
                    }

                    if (gender == 1 && graduate == 1)
                    {
                        femalespos++;
                    }

                    if (gender == 0 && graduate == 0)
                    {
                        malesPre++;
                    }

                    if (gender == 1 && graduate == 0)
                    {
                        femalesPre++;
                    }
                    break;
                }
            }
        }
    }
    printf("%-10d %-10d %-18d %-18d %-12d %-12d\n",
           year, semester, malesPre, femalesPre, malesPos, femalespos);
}

int main(int argc, char *argv[])
{
    //argumento para pasar el nombre del archivo en consola
    char *fileName = argv[1];

    //hacer el puntero al archivo
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

    matricula = malloc(sizeof(Matricula) * header.matriculaCant);
    fread(matricula, sizeof(Matricula), header.matriculaCant, fp);

    printf("%-10s %-10s %-18s %-18s %-12s %-12s\n",
           "Year", "Semester", "Male-Undergrad", "Female-Undergrad", "Male-Grad", "Female-Grad");

    int startYear = 2020;
    int totalYears = 10;
    for (int i = 0; i < totalYears; i++)
    {
        int currentYear = startYear + i;

        for (int j = 1; j < 5; j++)
        {
            printTableLine(&header, matricula, student, currentYear, j);
        }
                        
    }

    free(student);
    free(course);
    free(matricula);

    return 0;
}