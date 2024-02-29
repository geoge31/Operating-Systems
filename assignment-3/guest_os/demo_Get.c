#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"



int main()
{
        struct d_params dm;
        char            random_groupName;
        int             returned_Value;
        int             random_memberId;
        int             i;
        int             upperOrLower;
        
        srand(time(NULL));

        returned_Value = 0;

    
        for(i=0; i<2; i++)
        {
                upperOrLower = rand() % 2;

                if(upperOrLower)
                {
                        random_groupName = rand() % 26 + 'A';
                        random_memberId = rand() % 41 - 20;

                        returned_Value = setParams(random_groupName,random_memberId);

                        if(returned_Value == 0)
                        {
                                printf(YEL"\nTrap to kernel level\n"WHT);
                                printf("Arguments given  | Group Name : %c, | Member Id : %d|\n", random_groupName, random_memberId);
                                printf("Calling : get_task_params expecting to return 0 | Returned Value : %d\n", getParams(&dm));
                                printf(CYN"Expecting the values --> Group Name : %c | Member Id : %d |\n"WHT, random_groupName, random_memberId);
                                printf(GRN"Returned values --> Group Name : %c | Member Id : %d |\n"WHT, dm.group_name, dm.member_id);
                                printf(YEL"Back to user level\n\n"WHT);
                                }
                        else 
                        {
                                printf(YEL"\nTrap to kernel level\n"WHT);
                                printf("Arguments given | Group Name : %c | Member Id : %d |\n", random_groupName, random_memberId);
                                printf("Calling : get_task_params expecting to return 0 | Returned Value : %d\n", getParams(&dm));
                                printf(YEL"Back to user level\n\n"WHT);
                        }
                        
                }
                else
                {
                        random_groupName = rand() % 26 + 'a';
                        random_memberId = rand() % 41 - 20;
                        
                        returned_Value = setParams(random_groupName,random_memberId);

                       if(returned_Value == 0)
                        {
                                printf(YEL"\nTrap to kernel level\n"WHT);
                                printf("Arguments given  | Group Name : %c, | Member Id : %d|\n", random_groupName, random_memberId);
                                printf("Calling : get_task_params expecting to return 0 | Returned Value : %d\n", getParams(&dm));
                                printf(CYN"Expecting the values --> Group Name : %c | Member Id : %d |\n"WHT, random_groupName, random_memberId);
                                printf(GRN"Returned values --> Group Name : %c | Member Id : %d |\n"WHT, dm.group_name, dm.member_id);
                                printf(YEL"Back to user level\n\n"WHT);
                                }
                        else 
                        {
                                printf(YEL"\nTrap to kernel level\n"WHT);
                                printf("Arguments given | Group Name : %c | Member Id : %d |\n", random_groupName, random_memberId);
                                printf("Calling : get_task_params expecting to return 0 | Returned Value : %d\n", getParams(&dm));
                                printf(YEL"Back to user level\n\n"WHT);
                        }
                }
        }


        return 0;
}
