#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/d_params.h>
#include <asm/uaccess.h>
#include <asm/current.h>


asmlinkage long sys_get_task_params(struct d_params *params)
{
    struct task_struct  *getT;
    char                *groupName;
    int                 *memberId;

    printk("|   Giorgos Geramoutsos     |   3927    |   get_task_params     |\n");

    if(!params)
    {
        return EINVAL;
    }
    
    if(!access_ok(VERIFY_WRITE,params,sizeof(struct d_params *)))
    {
        return EINVAL;
    }

    getT = get_current();

    groupName = &params -> group_name;
    memberId = &params -> member_id;

    if(copy_to_user(groupName,&getT->group_name,sizeof(int)))
    {
        return EINVAL;
    }

    if(copy_to_user(memberId,&getT->member_id,sizeof(int)))
    {
        return EINVAL;
    }

    printk("|   Group Name : %c     |   Member Id : %d  |\n",params->group_name,params->member_id);

    return 0;
}
