#include "common.h"
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


#define USE_FILL 1
//#define TABLE_WIDE_MODE 1

struct Q {
	elem_t cmp, c, w, v;
	unsigned i;
};

static int Q_cmp_nonicreasing_profit_density(const void *A, const void *B) {
  const struct Q *a = (const struct Q*)A, *b = (const struct Q*)B;
	return b->c * a->cmp - a->c * b->cmp;
}

static void sort(struct task2 *task, elem_t *c_out, elem_t *w_out, elem_t *v_out, elem_t *cmp, unsigned *arrangement) {
  unsigned i, n = task->n;
  elem_t *c = task2_get_costs(task), *w = task2_get_weights(task), *v = task2_get_volumes(task);
  struct Q *q = (struct Q *)alloca(n * sizeof*q);

	for (i = 0; i < n; ++i) q[i].cmp = cmp[i], q[i].c = c[i], q[i].w = w[i], q[i].v = v[i], q[i].i = i;
	qsort(q, n, sizeof*q, Q_cmp_nonicreasing_profit_density);
	for (i = 0; i < n; ++i) cmp[i] = q[i].cmp, c_out[i] = q[i].c, w_out[i] = q[i].w, v_out[i] = q[i].v, arrangement[i] = q[i].i;
}


struct L {
	struct L *next;
	unsigned i;
};

struct TABLE {
	struct TABLE *next;
	struct L *L;
	elem_t C;
	elem_t W, V;
};

static inline void append_list_end(struct L **list_ptr, unsigned item_to_append) {
  struct L *list_item;
	list_item = (struct L *)malloc(sizeof(*list_item));
	list_item->i = item_to_append;
	list_item->next = NULL;
	*list_ptr = (*list_ptr)->next = list_item;
}

static inline int is_empty_t(struct TABLE *table) {
	return table->C == -1;
}

static inline void free_list(struct L *list) {(void)list;}

static inline void set_list(struct L **list_ptr, struct L *list_new, unsigned item_to_append) {
  struct L *list_item;
	if (*list_ptr != NULL) {
		free_list(*list_ptr);
	}
	list_item = (struct L *)malloc(sizeof(*list_item));
	list_item->i = item_to_append;
	list_item->next = list_new;
	*list_ptr = list_item;
}

static void FILL(struct L *small, elem_t mw, elem_t mv, elem_t *c, elem_t *w, elem_t *v, elem_t *C_out, unsigned *I_out) {
  unsigned i;
  struct L *cur_index;
  elem_t C = 0, W = 0, V = 0, tmpW, tmpV;
	for (cur_index=small,i=0; cur_index!=NULL; cur_index=cur_index->next,++i) {
		tmpW = W + w[cur_index->i];
		tmpV = V + v[cur_index->i];
		if (tmpW <= mw && tmpV <= mv) {
			W = tmpW;
			V = tmpV;
			C += c[cur_index->i];
			I_out[i] = cur_index->i;
		} else {
			W = mw; // disables consideration of the rest elements after critical one (replaces ordinary full greedy algorithm with FILL)
			I_out[i] = (unsigned)-1;
		}
	}
	*C_out = C;
}

																																				extern elem_t task2_ibarra1975_01__P_star;elem_t task2_ibarra1975_01__P_star;

void task2_ibarra1975_01(elem_t *sol, struct task2 *task, real_t eps) {
	//memset(sol, 0, task->n * sizeof(*sol));

  unsigned n = task->n, i, j;
  //elem_t *w = task2_get_weights(task), *v = task2_get_volumes(task);
  elem_t *w = alloca(n * sizeof*w), *v = alloca(n * sizeof*v);
  elem_t mw = *task2_get_maxweight(task), mv = *task2_get_maxvolume(task);
  //elem_t *c = task2_get_costs(task);
  elem_t *c = alloca(n * sizeof*c);

  struct task *task_max = task_create(n);
  elem_t *weights_max = task_get_weights(task_max);
  elem_t *task_max_sol = alloca(n*sizeof(elem_t));
  elem_t P_lower_bound;

  //struct task *task_min = task_create(n);
  //elem_t *weights_min = task_get_weights(task_min);
  //elem_t *task_min_sol = alloca(n*sizeof(elem_t));
  //elem_t P_upper_bound;

  const real_t eps2 = eps * eps;
  real_t delta;
  unsigned g;
  elem_t P_wave;

  struct TABLE *table;
  struct L *small, *small_end;
  unsigned small_size;

  unsigned *arrangement = alloca(n * sizeof*arrangement);

	// task max preparation
	{
	  elem_t *orig_w = task2_get_weights(task), *orig_v = task2_get_volumes(task);
		for (i = 0; i < n; ++i) {
		  elem_t w_i = orig_w[i] * mv, v_i = orig_v[i] * mw;
			weights_max[i] = max(w_i, v_i);
		}
	}

// Step 1
	sort(task, c, w, v, weights_max, arrangement);

// Step 2

	memcpy(task_get_costs(task_max), c, n*sizeof*c);
	*task_get_maxweight(task_max) = mw * mv;

	// task min preparation
	//for (i = 0; i < n; ++i) {
	//  elem_t w_i = w[i] * mv, v_i = v[i] * mw;
	//	weights_min[i] = min(w_i, v_i);
	//}
	//memcpy(task_get_costs(task_min), c, n*sizeof*c);
	//*task_get_maxweight(task_min) = mw * mv;

	task_solve_01(task_max_sol, task_max);
	//task_solve_01(task_min_sol, task_min);

	// CHECK: if task_min_sol is acceptable solution for the task2, then it is the optimal solution for the task2

	//P_wave = 0;
	//{
	//  elem_t tmpW, W = 0, maxW = *task_get_maxweight(task_max);
	//	for (i = 0; i < n; ++i) {
	//		P_wave += c[i];
	//		tmpW = W + weights_max[i];
	//		if (tmpW <= maxW) {
	//			W = tmpW;
	//		} else {
	//			break;
	//		}
	//	}
	//}

	P_lower_bound = mul_vec(task_get_costs(task_max), task_max_sol, n);
	//P_upper_bound = mul_vec(task_get_costs(task_min), task_min_sol, n);
	//memcpy(sol,task_max_sol, n*sizeof(elem_t));
	//for (i = 0; i < n; ++i) {
	//	sol[arrangement[i]] = task_max_sol[i];
	//}
	//task_delete(task_min);

	P_wave = 2*P_lower_bound;
	//P_wave = P_upper_bound;

	if (!(P_wave/2 <= task2_ibarra1975_01__P_star && task2_ibarra1975_01__P_star <= P_wave)) {
		__asm int 3;
	}

// Step 3
	delta = P_wave * eps2 / REAL_T(9.0);
	g = (unsigned)(REAL_T(9.0) / eps2) + 1; // +1 is for case when delta should be = P_wave * eps2 / REAL_T(9.0) = 22 * 0.90001 / 9.0 = 0.220001, but became strict 0.22

// Step 4
	table = (struct TABLE *)alloca((g+1) * sizeof*table);
	memset(&table[1], -1, g * sizeof*table);
	memset(&table[0], 0, sizeof table[0]);
	small = NULL;
	small_end = (struct L *)&small;
	small_size = 0;

// Step 5
	i = 0;

	do {
	  unsigned f_i;
	// Step 6
		if (c[i]*3 <= eps*P_wave) {
			//small += (p[i], c[i]);
			append_list_end(&small_end, i);
			++small_size;
			continue; //goto Step_8;
		}
	// Step 7
	//  Step 7.1
		f_i = (unsigned)(c[i] / delta);

	//  Step 7.2
		if (g < f_i)
			continue;
		j = g - f_i;
		for (;;) {
		  struct TABLE *cur = &table[j];
			if (!is_empty_t(cur)) {
				do {
					if (cur->W + w[i] <= mw && cur->V + v[i] <= mv) {
					  struct TABLE *upd = &table[j+f_i];
						if (is_empty_t(upd)) {
fill_upd:
							upd->next = NULL;
							set_list(&upd->L, cur->L, i);
							upd->C = cur->C + c[i];
							upd->W = cur->W + w[i];
							upd->V = cur->V + v[i];
						} else {
							do {
								if (upd->W > cur->W+w[i] || upd->V > cur->V+v[i]) {
									while (upd->next != NULL) {
										upd = upd->next;
									}
									upd->next = malloc(sizeof*upd);
									upd = upd->next;
									goto fill_upd;
								}
								upd = upd->next;
							} while (upd != NULL);
						}
						//else if (table[j+f_i].W > cur->W+w[i] /*||*/ && table[j+f_i].V > cur->V+v[i]) {
						//	if (!is_empty_t(&table[j+f_i])) {
						//		if (table[j+f_i].W < table[j].W+w[i] || table[j+f_i].V < table[j].V+v[i]) {
						//			__asm int 3;
						//		}
						//	}
						// }
					}
					cur = cur->next;
				} while (cur != NULL);
			}
			if (j == 0) break;
			--j;
		}

	// Step_8
	} while (++i < n);

// Step 9
	{
#if defined(USE_FILL) && (USE_FILL) != 0
	  unsigned *I = (unsigned *)alloca(2 * small_size * sizeof(unsigned));
	  unsigned *maxI = &I[small_size];
	  elem_t alpha = 0;
	  unsigned *tmp;
#endif
	  unsigned k;
	  struct TABLE *maxTABLE = NULL;
	  elem_t max = 0;
		for (k = 0; k <= g; ++k) {
		  struct TABLE *cur = &table[k];
			if (!is_empty_t(cur)) {
				do {
#if defined(USE_FILL) && (USE_FILL) != 0
					//FILL(small, mw*mv - max(cur->W*mv, cur->V*mw), task_get_costs(task_max), task_get_weights(task_max), &alpha, I);
					FILL(small, mw - cur->W, mv - cur->V, c, w, v, &alpha, I);
					if (cur->C + alpha > max) {
						tmp = I; I = maxI; maxI = tmp;
						max = cur->C + alpha;
						maxTABLE = cur;
					}
#else
					if (cur->C > max) {
						max = cur->C;
						maxTABLE = cur;
					}
#endif
					cur = cur->next;
				} while (cur != NULL);
			}
		}
		memset(sol, 0, n * sizeof*sol);
		if (maxTABLE != NULL) {
		  struct L *l;
			for (l = maxTABLE->L; l != NULL; l = l->next) {
				sol[arrangement[l->i]] = 1;
			}
#if defined(USE_FILL) && (USE_FILL) != 0
			for (i = 0; i < small_size; ++i) {
				if (maxI[i] != (unsigned)-1) {
					sol[arrangement[maxI[i]]] = 1;
				}
			}
#endif
		}
	}
	task_delete(task_max);
}
