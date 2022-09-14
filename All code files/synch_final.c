/*
 * Synchronization primitives.
 * See synch.h for specifications of the functions.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>

////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *namearg, int initial_count)
{
	struct semaphore *sem;

	assert(initial_count >= 0);

	sem = kmalloc(sizeof(struct semaphore));
	if (sem == NULL) {
		return NULL;
	}

	sem->name = kstrdup(namearg);
	if (sem->name == NULL) {
		kfree(sem);
		return NULL;
	}

	sem->count = initial_count;
	return sem;
}

void
sem_destroy(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	spl = splhigh();
	assert(thread_hassleepers(sem)==0);
	splx(spl);

	/*
	 * Note: while someone could theoretically start sleeping on
	 * the semaphore after the above test but before we free it,
	 * if they're going to do that, they can just as easily wait
	 * a bit and start sleeping on the semaphore after it's been
	 * freed. Consequently, there's not a whole lot of point in 
	 * including the kfrees in the splhigh block, so we don't.
	 */

	kfree(sem->name);
	kfree(sem);
}

void 
P(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the P without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();
	while (sem->count==0) {
		thread_sleep(sem);
	}
	assert(sem->count>0);
	sem->count--;
	splx(spl);
}

void
V(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	spl = splhigh();
	sem->count++;
	assert(sem->count>0);
	thread_wakeup(sem);
	splx(spl);
}

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
	assert( lock->lock_count==0 || (lock->lock_count && lock->lock_owner == curthread) );  // the lock is free or no owner holds the lock
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

////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
	struct cv *cv;

	cv = kmalloc(sizeof(struct cv));
	if (cv == NULL) {
		return NULL;
	}

	cv->name = kstrdup(name);
	if (cv->name==NULL) {
		kfree(cv);
		return NULL;
	}
	
	// add stuff here as needed
	
	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	// add stuff here as needed
	
	kfree(cv->name);
	kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}
