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




/*static void solve(elem_t *sol, struct task *task) {
  unsigned i, n = task->n;
  elem_t *x, *a = task_get_weights(task), *c = task_get_costs(task);
  elem_t b_m_ax, b = *task_get_maxweight(task), sol_cx = 0;

  	memset(sol, 0, n * sizeof(*sol));

	x = (elem_t *)alloca(n * sizeof(elem_t));
	memset(&x[1], 0, (n-1) * sizeof(elem_t));

	for (;;) {
		i = 1;
		x[0] = 0;

		do {
			b_m_ax = b - mul_vec(a, x, n);
			if (b_m_ax < 0) {
				if (i == n-1) return;
				x[i] = 0;
				++x[++i];
			}
		} while (b_m_ax < 0);

		check(x, c, sol, &sol_cx, n);

		x[0] = (int)(b_m_ax / a[0]);
		//assert(mul_vec(a, x) <= b);
		while (!(mul_vec(a, x, n) <= b)) --x[0];
		assert(x[0] >= 0);

		check(x, c, sol, &sol_cx, n);

		++x[1];
	}
}*/

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


static void solve01_checker(elem_t *sol, struct task *task) {
  unsigned n = task->n;
  elem_t *x, *a = task_get_weights(task), *c = task_get_costs(task);
  elem_t b = *task_get_maxweight(task), sol_cx = 0;

  	memset(sol, 0, n * sizeof(*sol));

	x = (elem_t *)alloca(n * sizeof(elem_t));
	memset(x, 0, n * sizeof(elem_t));

	do {
		if (mul_vec(a, x, n) <= b) {
			check(x, c, sol, &sol_cx, n);
		}
	} while (increment_01(x, 0, n) == 0);
}


void task_solve_01(elem_t *sol, struct task *task) {
  unsigned i, n = task->n;
  elem_t *x, *a = task_get_weights(task), *c = task_get_costs(task);
  elem_t b_m_ax, b = *task_get_maxweight(task), sol_cx = 0;

  	memset(sol, 0, n * sizeof(*sol));

	x = (elem_t *)alloca(n * sizeof(elem_t));
	memset(&x[1], 0, (n-1) * sizeof(elem_t));

	for (;;) {
		i = 1;
		x[0] = 0;

		do {
			b_m_ax = b - mul_vec(a, x, n);
			if (b_m_ax < 0) {
				if (i == n-1) {
					goto check;
				}
				x[i] = 0;
				if (increment_01(x, ++i, n)) //++x[++i];
					goto check;
			}
		} while (b_m_ax < 0);

		check(x, c, sol, &sol_cx, n);

		x[0] = (int)(b_m_ax >= a[0]);
		//assert(mul_vec(a, x) <= b);
		while (!(mul_vec(a, x, n) <= b)) --x[0];
		assert(x[0] >= 0);

		check(x, c, sol, &sol_cx, n);

		if (increment_01(x, 1, n)) //++x[1];
			goto check;
	}

check: return;
	//{
	//  elem_t *sol_check = (elem_t *)alloca(n * sizeof(elem_t));
	//  elem_t sol_check_cx;
	//	solve01_checker(sol_check, task);
	//	sol_check_cx = mul_vec(c, sol_check, n);
	//	if (sol_cx != sol_check_cx) {
	//		fprintf(stderr, "error\n"); //__asm int 3
	//	}
	//}
}
