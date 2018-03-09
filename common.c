#include "common.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

struct task * task_create(unsigned n) {
  struct task *task;
	task = (struct task *)malloc(sizeof_task_fields + (2*n+1)*sizeof(elem_t));
	task->n = n;
	return task;
}

void task_delete(struct task *task) { free(task); }

void task_fill_random(struct task *task, elem_t max) {
  unsigned n = task->n, i;
  elem_t *costs = task_get_costs(task), *weights = task_get_weights(task);
  elem_t maxweight = rand() * max / RAND_MAX + 1;
	*task_get_maxweight(task) = maxweight;
	for (i = 0; i < n; ++i) {
		weights[i] = rand() * maxweight / RAND_MAX + 1;
		costs[i] = rand() * max / RAND_MAX + 1;
	}
}

void task_print(struct task *task) {
  unsigned n = task->n, i;
  elem_t *costs = task_get_costs(task), *weights = task_get_weights(task);
	printf("task n=%u\n", n);
	printf(" costs = (%u", costs[0]);
	for (i = 1; i < n; ++i) printf(", %u", costs[i]);
	printf(")\n");
	printf(" weights = (%u", weights[0]);
	for (i = 1; i < n; ++i) printf(", %u", weights[i]);
	printf(") <= %u\n", *task_get_maxweight(task));
	printf(" c/w ratio = (%lf", (double)costs[0]/weights[0]);
	for (i = 1; i < n; ++i) printf(", %lf", (double)costs[i]/weights[i]);
	printf(")\n");
}

void task_solution_print(elem_t *sol, const char *name, struct task *task) {
  unsigned i, n = task->n;
	printf(" %s = (%u", name, sol[0]);
	for (i = 1; i < n; ++i) printf(", %u", sol[i]);
	printf(") --> %u\n", mul_vec(sol, task_get_costs(task), n));
}







struct task2 * task2_create(unsigned n) {
  struct task2 *task;
	task = (struct task2 *)malloc(sizeof_task2_fields + (3*n+2)*sizeof(elem_t));
	task->n = n;
	return task;
}

void task2_delete(struct task2 *task) { free(task); }

void task2_fill_random(struct task2 *task, elem_t max) {
  unsigned n = task->n, i;
  elem_t *costs = task2_get_costs(task), *weights = task2_get_weights(task), *volumes = task2_get_volumes(task);
  elem_t maxweight = rand() * max / RAND_MAX + 1;
  elem_t maxvolume = rand() * max / RAND_MAX + 1;
	*task2_get_maxweight(task) = maxweight;
	*task2_get_maxvolume(task) = maxvolume;
	for (i = 0; i < n; ++i) {
		weights[i] = rand() * maxweight / RAND_MAX + 1;
		volumes[i] = rand() * maxvolume / RAND_MAX + 1;
		costs[i] = rand() * max / RAND_MAX + 1;
	}
}

void task2_print(struct task2 *task) {
  unsigned n = task->n, i;
  elem_t *costs = task2_get_costs(task), *weights = task2_get_weights(task), *volumes = task2_get_volumes(task);
	printf("task n=%u\n", n);
	printf(" costs      = (%2u", costs[0]);
	for (i = 1; i < n; ++i) printf(",%2u", costs[i]);
	printf(")\n");
	printf(" weights    = (%2u", weights[0]);
	for (i = 1; i < n; ++i) printf(",%2u", weights[i]);
	printf(") <= %2u\n", *task2_get_maxweight(task));
	printf(" volumes    = (%2u", volumes[0]);
	for (i = 1; i < n; ++i) printf(",%2u", volumes[i]);
	printf(") <= %u\n", *task2_get_maxvolume(task));
}

//void arr_print(elem_t *a, const char *name, unsigned n) {}

void task2_solution_print(elem_t *sol, const char *name, struct task2 *task) {
  unsigned i, n = task->n;
	printf(" %s = (%2u", name, sol[0]);
	for (i = 1; i < n; ++i) printf(",%2u", sol[i]);
	printf(") --> %u\n", mul_vec(sol, task2_get_costs(task), n));
}








elem_t mul_vec(elem_t *a, elem_t *b, unsigned n) {
  elem_t res = 0;
  unsigned i;
	for (i = 0; i < n; ++i) {
		res += a[i] * b[i];
	}
	return res;
}

