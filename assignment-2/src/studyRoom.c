/*  
      #######################################################
      #                                                     #
      #           Assignment 2 - hy345                      # 
      #           Giogos Geramoutsos, @csd3927              #
      #                                                     #
      #######################################################                                                           
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>



#define MAX_CAPACITY 8
#define MAX_STUDENTS 40
#define MIN_STUDENTS 20
#define Red     "\e[0;31m"
#define Green   "\e[0;32m"
#define Yellow  "\e[0;33m"
#define Blue    "\e[0;34m"
#define Purple  "\e[0;35m"
#define Cyan    "\e[0;36m"
#define Reset   "\e[0;37m"           



pthread_mutex_t stdRm;                                      /* A mutex that ensures that only 1 thread is entering the critical section*/
pthread_cond_t isFull;                                      /* A cond that signals all threads that waitinig that the study hall is empty again */



typedef struct Student                                      /* Student Struct */ 
{
    int id;                                                 /* ID of student */
    int index;                                              /* index for storing its ID */
    struct Student* next;

}student;



student*  sH = NULL;                                        /* Header pointer of study room */
student*  qH = NULL;                                        /* Header pointer of queue room */



int flagForPrint                =  0;                       /* A simple flag that checks just for the first 8 students if they have finished */
int students_inside             =  0;                       /* Counts how many students are in the study hall */
int waitin_students             =  0;                       /* Counts how many students are waiting in the waiting hall */
int studyRoom_Index             =  0;                       /* Index of the array studyRoom_IDs */
int queueRoom_Index             =  0;                       /* Index of the array queueRoom_IDs */
int studyRoom_IDs[MAX_CAPACITY] = {0};                      /* An array that stores thes id's of active students (just for print) */
int queueRoom_IDs[MAX_STUDENTS] = {0};                      /* An array that stores thes id's of students that waiting outside (just for print) */



void printStudyRoom(void)
{
    int i;

    printf(Cyan"Study Room:\t"Reset);

    for(i=0 ; i<MAX_CAPACITY ; i++)
    {
        if(studyRoom_IDs[i] != 0)
        {
            printf(Reset"|");
            printf(Cyan"\e[0;36m%d"Reset,studyRoom_IDs[i]);
        }
        else
        {
            printf(Reset"| ");
        }    
    }
    printf("|\n");
}



void printQueueRoom(void)
{
    int i;

    printf(Yellow"Queue Room:\t"Reset);

    for(i=0 ; i<MAX_STUDENTS ; i++)
    {
        if(queueRoom_IDs[i] != 0)
        {
            printf("|"Yellow);
            printf("%d"Reset,queueRoom_IDs[i]);
        }
        else
        {
            printf("| ");
        }
    }

    if(waitin_students == 0)
    {
        printf("| ");
    }

    printf("|\n\n");
}



void enteringStudyRoom(int IdToInsert)
{
    student* new_student = (student*)malloc(sizeof(student));

    /* Set new student */
    new_student -> id       = IdToInsert;
    new_student -> index    = studyRoom_Index;

    /* Store its id to array of ids*/
    studyRoom_IDs[studyRoom_Index] = new_student->id;
    studyRoom_Index++;
    
    if(!sH)
    {
        new_student -> next = NULL;
        sH = new_student;
    }
    else 
    {
        new_student -> next = sH;
        sH = new_student;
    }
    students_inside++;
}



int leavingStudyRoom(int IdToRemove)
{
    student*    tmp;
    student*    tmp2;
    int         idRemoved;

    tmp = sH;

    if((sH->id) == IdToRemove)
    {
        idRemoved = tmp -> id;
        studyRoom_IDs[tmp->index] = 0;

        tmp = sH;
        sH  = sH -> next;
        free(tmp);
    }
    else
    {
        while(tmp)
        {
            if((tmp->id) == IdToRemove)
            {
                idRemoved = tmp -> id;
                studyRoom_IDs[tmp->index] = 0;

                tmp2 -> next = tmp -> next;

                free(tmp);
                break;

            }
            else 
            {
                tmp2 = tmp;
                tmp  = tmp -> next;
            }
        }
    }

    students_inside--;

    return idRemoved;
}



void enteringQueueRoom(int IdToInsert)
{
    student* new_student = (student*)malloc(sizeof(student));

    /* Set new student */
    new_student -> id = IdToInsert;
    new_student -> index = queueRoom_Index;

    queueRoom_IDs[queueRoom_Index] = new_student -> id;
    queueRoom_Index++;

    if(!qH)
    {
        new_student -> next = NULL;
        qH = new_student;
    }
    else
    {
        new_student -> next = qH;
        qH = new_student;
    }
    waitin_students++;
}



int leavingQueueRoom(void)
{
    student*    tmp;
    student*    tmp2;
    int         idRemoved;

    tmp = qH;

    if(!qH)
    {
        return -1;
    }
    else if(!(tmp -> next))
    {
        idRemoved = tmp -> id;
        queueRoom_IDs[tmp->index] = 0;

        qH = NULL;
    }
    else 
    {
        while(tmp -> next)
        {
            tmp2 = tmp;
            tmp = tmp -> next;
        }

        idRemoved = tmp -> id;
        queueRoom_IDs[tmp->index] = 0;
        tmp2 -> next = NULL;
    }

    waitin_students--;

    return idRemoved;
}



void myPrints(int colorise, int id, int studytime)
{
    switch(colorise)
    {
        case 1  :
                    printf(Reset "Student ");
                    printf(Green "%d"Reset,id);
                    printf(" was born\n\n"); 
                    break;
        case 2  :
                    printf(Reset "Student ");
                    printf(Red "%d" Reset,id);
                    printf(" cannot enter the study room, it is full!\n\n");
                    break;
        case 3  :
                    printf(Reset "Student ");
                    printf(Green "%d" Reset,id);
                    printf(" can now enter the study room!\n\n");
                    break;
        case 4  :
                    printf(Reset "Student ");
                    printf(Purple "%d" Reset,id);
                    printf(" exited the study room after studying for %d secs.\n\n",studytime);
                    break;
        default :
                    break;
    }
}



void* student_thread(void* arg) {

    int student_id = *((int*)arg);
    int hasFinish;

    

    pthread_mutex_lock(&stdRm);  
    
    myPrints(1,student_id,0);                                     

        /*  If there are 8 students inside :
                1.Enter the waiting Queue 
                2. Release lock and wait for the wake_up signal                                             
        */

    while (students_inside  >= MAX_CAPACITY)      
        {                                          
            enteringQueueRoom(student_id);
            myPrints(2,student_id,0);
            printStudyRoom();
            printQueueRoom();
            pthread_cond_wait(&isFull, &stdRm);
        }

            /*
                If the student can enter the study room:
                    1. Insert it's id to the list
                    2. Store it's id to the array 
                    3. Print the Study Hall
                    4. Print the Queue Hall
                    *5. Check the flag to update printing
            */

    if(flagForPrint == 1)
    {
        int student_dequeued = leavingQueueRoom(); 

            if(student_dequeued != -1)
            {
                myPrints(3,student_dequeued,0);
            }
    }

    
    enteringStudyRoom(student_id);
    printStudyRoom();
    printQueueRoom();

        /*
            When the student has finish registering,
                1. Unlock the mutex
                2. Do your study 
        */

    pthread_mutex_unlock(&stdRm);

    int studyTime = rand() % 11 + 5;
    sleep(studyTime);  

            /* 
                When the student has finish its study 
                    1. Lock the mutex to ensure that only one thread has access to the critical section 
                    2. Print how much time the execution last
                    3. Print the Study Room
                    4. Print the Queue Room
                    5. Before unlocking the mutex, check if the study room its empty to send a signal to the students are waiting outside
            */

    pthread_mutex_lock(&stdRm);

    myPrints(4,student_id,studyTime);

    hasFinish = leavingStudyRoom(student_id);

    printStudyRoom();
    printQueueRoom();

    if (students_inside  == 0 && waitin_students  > 0) 
    {
       int j            = 0;
       studyRoom_Index  = 0;
       flagForPrint     = 1;

       while(j < MAX_CAPACITY)
       {
            pthread_cond_signal(&isFull);
            sleep(1);
            j++;
       }       
    }
    
    pthread_mutex_unlock(&stdRm);

    return NULL;
}



void free_all(void)
{
    while(sH) 
    {
        struct Student* tmp;
        
        tmp = sH;
        sH  = sH -> next;
        free(tmp);
    }
    
    while(qH) 
    {
        struct Student* tmp;
        
        tmp = qH;
        qH  = qH -> next;
        free(tmp);
    }   
}



int main()
{
    pthread_mutex_init(&stdRm,NULL);                                                                        /* Initalise Mutex */
    pthread_cond_init(&isFull,NULL);                                                                        /* Initialise Condition */

    srand(time(NULL));
    
    int N;                                                                                                 /* Total Students */  

    printf("\nEnter the total number of students: \n");
    scanf("%d", &N);
    printf("\n");

    if(N < MIN_STUDENTS || N > MAX_STUDENTS+1)
    {
        perror("Students must be between 20 and 40!\n");
        exit(-1);    
    } 

    pthread_t   students[N];                                                                                /* Total Threads to be created */
    int         iDs[N];                                                                                     /* Total Id's to be stored */

    for (int i = 0; i < N; i++) 
    {                                                                                                       /* Create Threads */
        iDs[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &iDs[i]);
    }

    for (int i = 0; i < N; i++)             
    {                                                                                                       /* Join Threads */
        pthread_join(students[i], NULL);
    }

    pthread_mutex_destroy(&stdRm);                                                                          /* Destroy Mutex */
    pthread_cond_destroy(&isFull);                                                                          /* Destroy Conditinion */

    free_all();                                                                                             /* Free the memory */
    
    return 0;
}
