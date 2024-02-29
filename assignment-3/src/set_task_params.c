#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/syscalls.h>
#include <linux/d_params.h>


asmlinkage long sys_set_task_params(char group_name, int member_id)
{
    struct task_struct  *taskSet;
    
    printk("|   Giorgos Geramoutsos |   3927    |   set_task_params   |\n");

    if(!(group_name >= 'A' && group_name <= 'Z') || member_id <= 0)
    {
        printk("Invalid arguments");
        return EINVAL;
    }

    taskSet = get_current();
    taskSet -> group_name = group_name;
    taskSet -> member_id = member_id;

    printk("|   Group Name: %c  |   Member Id: %d   |\n", taskSet->group_name, taskSet->member_id);

    return 0;
}
