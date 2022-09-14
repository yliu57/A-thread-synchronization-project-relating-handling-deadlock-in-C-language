// os161-1.11 kern/include/synch.h
struct lock {
	char *name;
	volatile int lock_count;  //689 
	volatile struct lock lock_owner; //690
};

struct lock *lock_create(const char *name);
void         lock_acquire(struct lock *);
void         lock_release(struct lock *);
int          lock_do_i_hold(struct lock *);
void         lock_destroy(struct lock *);




////////////////////////////////////////////////////////////
//
// Lock.

struct lock *
lock_create(const char *name)
{
	struct lock *lock;

	lock = kmalloc(sizeof(struct lock));
	if (lock == NULL) {
		return NULL;
	}

	lock->name = kstrdup(name);
	if (lock->name == NULL) {
		kfree(lock);
		return NULL;
	}
	
	// add stuff here as needed
	lock->lock_count = 0;         //  initialize the count to 0 (no thread holds it)
 	lock->lock_owner = NULL; 	  // intialize the owner to NULL
	
	return lock;
}

void
lock_destroy(struct lock *lock)
{
	assert(lock != NULL);

	// add stuff here as needed
	while( lock->lk_count==1 && lock->lk_owner != NULL); //Wait utill count is 0 and owner is NULL
	assert(lock->lk_count==0 && lock->lk_owner == NULL); //Confirm that count is 0 and owner is NULL
	
	kfree(lock->name);
	kfree(lock->lock_count);	// free the count
	kfree(lock->lock_owner);	// free the owner
	kfree(lock);
}

void
lock_acquire(struct lock *lock)
{
	int spl;
	assert(lock != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the lock_acquire without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();
	while ( lock->lock_count==1 && lock->lock_owner != curthread) { 		// the lock is hold by other threads
		thread_sleep(lock);
	}
	assert( lock->lock_count==0 || (lock->lock_count && lock->lock_owner == curthread) );  // the lock is free or no owner hold the lock
	lock->lock_count = 1;								
	lock->lock_owner = curthread;						// the lock count and owner status change. The lock is hold by the current thread now.
	splx(spl);
	
}

void
lock_release(struct lock *lock)
{
	int spl;
	assert(lock != NULL);
	spl = splhigh();
	
	// reset the count and clear the owner
	lock->lock_count = 0;								
	lock->lock_owner = NULL;
	
	// confirm that the count is reset and the owner is cleared
	assert(lock->lock_count==0 && lock->lock_owner == NULL);
	
	thread_wakeup(lock);
	splx(spl);

}

int
lock_do_i_hold(struct lock *lock)
{
	assert(lock != NULL);
	if(lock->lock_owner == curthread){
		return 1;
	}
	return 0;
}

