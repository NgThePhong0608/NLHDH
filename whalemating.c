/*
 * Copyright (c) 2001, 2002, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Driver code is in kern/tests/synchprobs.c We will
 * replace that file. This file is yours to modify as you see fit.
 *
 * You should implement your solution to the whalemating problem below.
 */

#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>

/*
 * Called by the driver during initialization.
 */
struct semaphore *male_sem;
struct semaphore *female_sem;
struct semaphore *match_sem;

struct lock *common_lock;
// struct semaphore * common_sem;
struct cv *common_cv;

int sem_count = 3;
void whalemating_init()
{
	male_sem = sem_create("man sem", 1);
	female_sem = sem_create("female sem", 1);
	match_sem = sem_create("match sem", 1);

	common_lock = lock_create("common lock");
	common_cv = cv_create("common cv");
	return;
}

/*
 * Called by the driver during teardown.
 */

void whalemating_cleanup()
{
	sem_destroy(male_sem);
	sem_destroy(female_sem);
	sem_destroy(match_sem);

	lock_destroy(common_lock);
	cv_destroy(common_cv);
	return;
}

void male(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling male_start and male_end when
	 * appropriate.
	 */
	male_start(index);
	// Implement this function
	// P(male_sem);
	lock_acquire(common_lock);
	sem_count--;
	if (sem_count != 0)
		cv_wait(common_cv, common_lock);
	else
		sem_count = 3;
	cv_broadcast(common_cv, common_lock);
	lock_release(common_lock);
	male_end(index);
	// V(male_sem);
	return;
}

void female(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling female_start and female_end when
	 * appropriate.
	 */
	female_start(index);
	// Implement this function
	// P(female_sem);
	lock_acquire(common_lock);
	sem_count--;
	if (sem_count != 0)
		cv_wait(common_cv, common_lock);
	else
		sem_count = 3;
	cv_broadcast(common_cv, common_lock);
	lock_release(common_lock);
	female_end(index);
	// V(female_sem);
	return;
}

void matchmaker(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling matchmaker_start and matchmaker_end
	 * when appropriate.
	 */
	matchmaker_start(index);
	// P(match_sem);
	lock_acquire(common_lock);
	sem_count--;
	if (sem_count != 0)
		cv_wait(common_cv, common_lock);
	else
		sem_count = 3;
	cv_broadcast(common_cv, common_lock);
	lock_release(common_lock);
	matchmaker_end(index);
	// V(match_sem);
	return;
}
