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
        char    groupName;
        int     memberId;
        int     returned_Value;

        groupName = 'A';
        memberId = 3;

        returned_Value = setParams(groupName,memberId);

        if(returned_Value == 0)
        {
            printf(YEL"\nTrap to kernel level\n"WHT);
            printf(GRN"Arguments given  | Group Name : %c, | Member Id : %d|\n"WHT,groupName,memberId);
            printf(YEL"Back to user level\n\n"WHT);
        }
        else
        {   
            printf(YEL"\nTrap to kernel level\n"WHT);
            printf(RED"Wrong Arguments given !\n"WHT);
            printf(YEL"Back to user level\n\n"WHT);
        }
        
        groupName = 'A';
        memberId = -1;

        returned_Value = setParams(groupName,memberId);

        if(returned_Value == 0)
        {
            printf(YEL"\nTrap to kernel level\n"WHT);
            printf(GRN"Arguments given  | Group Name : %c, | Member Id : %d|\n"WHT,groupName,memberId);
            printf(YEL"Back to user level\n\n"WHT);
        }
        else
        {   
            printf(YEL"\nTrap to kernel level\n"WHT);
            printf(RED"Wrong Arguments given ( Member Id : -1 ) !\n"WHT);
            printf(YEL"Back to user level\n\n"WHT);
        }

        groupName = 'B';
        memberId = 5;

        returned_Value = setParams(groupName,memberId);

        if(returned_Value == 0)
        {
            printf(YEL"\nTrap to kernel level\n"WHT);
            printf(GRN"Arguments given  | Group Name : %c, | Member Id : %d|\n"WHT,groupName,memberId);
            printf(YEL"Back to user level\n\n"WHT);
        }
        else
        {   
            printf(YEL"\nTrap to kernel level\n"WHT);
            printf(RED"Wrong Arguments given !\n"WHT);
            printf(YEL"Back to user level\n\n"WHT);
        }
        
        groupName = 'c';
        memberId = 3;

        returned_Value = setParams(groupName,memberId);

        if(returned_Value == 0)
        {
            printf(YEL"\nTrap to kernel level\n"WHT);
            printf(GRN"Arguments given  | Group Name : %c, | Member Id : %d|\n"WHT,groupName,memberId);
            printf(YEL"Back to user level\n\n"WHT);
        }
        else
        {   
            printf(YEL"\nTrap to kernel level\n"WHT);
            printf(RED"Wrong Arguments given ( Group Member : c ) !\n"WHT);
            printf(YEL"Back to user level\n\n"WHT);
        }

        return 0;
}
