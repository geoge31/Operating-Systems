#################################################################

Assignment 3 -HY345
README
Giorgos Geramoutsos @csd3927

#################################################################

-- Αρχεία που τροποποιήθηκαν -- 

Στον φάκελο src υπάρχουν τα παρακάτω αρχεία :

    - unistd_32.h (linux-2.6.38.1/arch/x86/include/asm/unistd_32.h) :
        Προστέθηκαν τα δύο νέα system calls στις γραμμές 350,351
        #define __NR_set_task_params    342
        #define __NR_get_task_params    343

    - syscall_table_32.S (linux-2.6.38.1/arch/x86/kernel/syscall_table_32.S) :
        Προστέθηκαν τα νέα function pointer στο τέλος
        .long sys_set_task_params	/* 342 */
        .long sys_get_task_params	/* 343 */

    - syscalls.h (linux-2.6.38.1/include/asm-generic/syscalls.h) :
        Προστέθηκαν τα system calls 
        #ifndef sys_set_task_params
        asmlinkage long sys_set_task_params(char group_name, int member_id);
        #endif

        #ifndef sys_get_task_params
        asmlinkage long sys_get_task_params(struct d_params * params);
        #endif

    - sched.h (linux-2.6.38.1/include/linux/sched.h) :
        Προστέθηκαν τα πεδία group_name και member_id στις γραμμές 1202,1203
        char group_name;
        int member_id;

    - d_params.h (linux-2.6.38.1/include/linux/d_params.h) :
        Δημιουργήθηκε το αρχείο d_params.h όπου περιέχει το struct

    - set_task_params.c (linux-2.6.38.1/kernel/set_task_params.c) :
        Δημιουργήθηκε το αρχείο set_task_params.c όπυ υλοποιείται το system call set_task_parameters
    
    - get_task_params.c (linux-2.6.38.1/kernel/get_task_params.c) :
        Δημιουργήθηκε το αρχείο set_task_params.c όπυ υλοποιείται το system call get_task_parameters

    - Makefile (linux-2.6.38.1/kernel/Makefile) :
        Προστέθηκαν οι δύο επιπλέον εντολές για το Make
        obj-y += set_task_params.o
        obj-y += get_task_params.o
    
    - bzImage (linux-2.6.38.1/arch/x86/boot/bzImage) :
        Το ανανεωμένο bzImage

Στον φάκελο guest_os υπάρχουν τα παρακάτω αρχεία :

    - unistd.h (/usr/include/unistd.h) : 
        Προστέθηκε το struct d_params με τα πεδία του
        Προστέθηκαν τα macro για τα system calls

    - demo_Set.c:
        Ένα demo για το system call set_task_params

    - demo_Get.c : 
        Ένα demo για το system call get_task_params

    - Makefile : 
        Ένα makefile για την εκτέλεση των  demo_Set και demo_Get. 



