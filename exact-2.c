#include "common.h"
#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static inline void check(elem_t *x, elem_t *c, elem_t *sol, elem_t *const sol_cx, unsigned n) {
  elem_t cx;
	cx = mul_vec(c, x, n);
	if (cx > *sol_cx) {
		memcpy(sol, x, n * sizeof(elem_t));
		*sol_cx = cx;
	}
}

static inline unsigned increment_01(elem_t *x, unsigned from, unsigned n) {
  unsigned i = from;
	//for (i = from; i < n; ++i) {
	for (;;) {
		if (x[i] == 0) {
			++x[i]; return 0;
		}
		x[i] = 0;
		++i;
		if (i == n) return 1;
	}
}


void task2_solve_01(elem_t *sol, struct task2 *task) {
  unsigned n = task->n;
  elem_t *x, *a = task2_get_weights(task), *d = task2_get_volumes(task), *c = task2_get_costs(task);
  elem_t b = *task2_get_maxweight(task), e = *task2_get_maxvolume(task), sol_cx = 0;

  	memset(sol, 0, n * sizeof(*sol));

	x = (elem_t *)alloca(n * sizeof(elem_t));
	memset(x, 0, n * sizeof(elem_t));

	do {
		if (mul_vec(a, x, n) <= b && mul_vec(d, x, n) <= e) {
			check(x, c, sol, &sol_cx, n);
		}
	} while (increment_01(x, 0, n) == 0);
}
