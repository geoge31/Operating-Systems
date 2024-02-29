HY345 
Assignment 4 - 2023
Γιώργος Γεραμούτσος , csd3927

~ Μια θεωρητική υλοποίηση που σκέφτηκα είναι η εξής: 
	1. Δημιουργούμε τις διεργασίες μέσω ενός demo και κάνοντας spin τις εναλάσσουμε
	2. Κάθε φορά που δημιουργούμε μια διεργασία τσεκάρουμε το γκρουπ της και ανανεώνουμε το fairness του group της.
	3. Κάνουμε ενημέρωση των νέων χρόνων για τις διεργασίες λόγω της νέας διεργασίας που ανήκουν σε αυτό το γκρουπ.
	4. Μέσω του δέντρου επιλέγουμε την επόμενη διεργασία που είναι να τρέξει.
	(Εφόσον υλοποιούμε CFS θα επιλέγουμε πάντα την Mostleft και θα κάνουμε εναλλαγές μεταφέροντας τις διεργασίες)
	5. Όταν τελειώσει ο χρόνος που είναι ορισμένος για κάθε διεργασία κάνουμε εναλλαγή.
	6. Όταν η διεργασία ολοκληρωθεί αφαιρείται από το γκρουπ και γίνεται ανανέωση στους χρόνους των group και των διεργασιών ξανά.

	~Κάποιες πιθανές συναρτήσεις ή τροποποιημένες υπάρχουσες είναι παρακάτω.


~ Θα χρειαζόμασταν μια συνάρτηση για να υλοποιήσουμε το fairness
	
	//Αν έχουμε μόνο ένα γκρουπ παίρνει όλο το ποσοστό της CPU
	
	static unsigned int calculate_fairness(struct task_struct *task, int num_of_groups) 
	{
    
	    int processes_in_group = get_number_of_processes_in_group(rq, p->group_id);
    
	    if (processes_in_group == 0) 
	    {
	        processes_in_group = 1;
	    }

	    return 100 / (num_groups * processes_in_group);

	}

~ Άρα αυτομάτως θα θέλαμε μια συνάρτηση να υπολογίζει πόσες διεργασίες είναι σε κάθε group 
	Ένας τρόπος θα ήταν να χρησιμοποιήσουμε το struct rq και να κάνουμε traverse την λίστα για να βρούμε τις διεργασίες που ανήκουν στο γκρουπ
	Μια πιθανή υλοποίηση : 
		
		int get_number_of_processes_in_group(struct rq *rq, int group_id) 
		{
		    
		    list_for_each_entry(cfs_rq, &rq->leaf_cfs_rq_list, leaf_cfs_rq_list) {
		        // checkaroume an einai to zitoumeno group
		        if (cfs_rq->your_group_identifier == group_id) {
		            // Apla auksanoume ton metrhth 
		            count += cfs_rq->nr_running;
		        }
		    }
		    return count;  // Number of processes in the group
		}


~ Μία συνάρτηση να υλοποιεί τον CFS για τον χρόνο εκτέλεσης 
	
	void update_cfs_scheduler(struct task_struct *p, int num_of_groups)
	{
		int execution_time = calculate_fairness(p,num_of_groups);

		// Εδώ θα πρέπει να ενημερώσουμε το τυχόν πεδία της διεργασίας με βάση τον χρόνο εκτέλεσης που της αναλογεί και ό,τι άλλες παραμετροποιήσεις χρειάζονται 

		update_process(p, execution_time);
	}


~ Ενημέρωση της νέας  διεργασίας
	
	void update_process(struct task *p, int execution_time)
	{
		// Απ' ότι κατάλαβα αυτό χρειάζεται για να ενημερώσουμε την προτεραιότητα της διεργασίας καθώς έχει το default priority και προσθέτουμε και τον χρόνο εκτέλεσης 
		p->prio = BASE_PRIORITY + execution_time;
	}


~ Τροποιώντας την συνάρτηση schedule που είναι η καρδιά του kernel θα διαλέγουμε ποιά διεργασία θα τρέξει μετά, καλώντας και την συνάρτηση pick_next_task()
	
	void schedule(void) 
	{
	    struct task_struct *prev, *next;
	    prev = current;

	    // Βρίσκουμε την επόμενη διεργασία που χρειάζεται 
	    next = pick_next_task(); 

	    // Κάνουμε εναλλαγή της τρέχουσας με την επόμενη που πρέπει να τρέξει
	    context_switch(prev, next);  
}


~ Συνάρτηση που επιλέγει το επόμενο task 

	struct task_struct *pick_next_task(void) 
	{
	    // Εδώ θα πρέπει να υλοποιείται η λογική που επιλέγει ποια διεργασία θα τρέξει μετά
	    // Πιθανώς ελέγχοντας το redblack δέντρο των διεργασιών και επιλέγοντας το mostleft κόμβο του δέντρου σύμφωνα και με το σχεδιάγραμμα του φροντιστηρίου 

	   	...

	    return next;
	}


~ Συνάρτηση για την εναλλαγή διεργασιών 

	void context_switch(struct task_struct *prev, struct task_struct *next)
	{
	    // Εδώ θα υλοποιείται η εναλλαγή διεργασιών
	    // για παράδειγμα κάπως:
	    switch(prev, next);

		// Περατέρω τροποποιήσεις για την προηγούμενη και την επόμενη διεργασία
	    // Πχ virtual run time 
	}
