#include "common.h"
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct Q {
	elem_t p, c;
	unsigned i;
};

static int Q_cmp_nonicreasing_profit_density(const void *A, const void *B) {
  const struct Q *a = (const struct Q*)A, *b = (const struct Q*)B;
	return b->p * a->c - a->p * b->c;
}

static void sort(struct task *task, elem_t *p_out, elem_t *c_out, unsigned *arrangement) {
  unsigned i, n = task->n;
  elem_t *p = task_get_costs(task), *c = task_get_weights(task);
  struct Q *q = (struct Q *)alloca(n * sizeof*q);

	for (i = 0; i < n; ++i) q[i].p = p[i], q[i].c = c[i], q[i].i = i;
	qsort(q, n, sizeof*q, Q_cmp_nonicreasing_profit_density);
	for (i = 0; i < n; ++i) p_out[i] = q[i].p, c_out[i] = q[i].c, arrangement[i] = q[i].i;
}

struct L {
	struct L *next;
	unsigned i;
};

struct TABLE {
	struct L *L;
	elem_t P;
	elem_t W;
};

static inline int is_empty_t(struct TABLE *table) {
	return table->P == -1;
}

static inline void set_list(struct L **list_ptr, struct L *list_new, unsigned item_to_append, struct L *list_item) {
	list_item->i = item_to_append;
	list_item->next = list_new;
	*list_ptr = list_item;
}

/*static inline void append_list(struct L **list_ptr, unsigned item_to_append) {
  struct L *list_item;
	list_item = (struct L *)malloc(sizeof(*list_item));
	list_item->i = item_to_append;
	list_item->next = *list_ptr;
	*list_ptr = list_item;
}*/

static inline void append_list_end(struct L **list_ptr, unsigned item_to_append, struct L *list_item) {
	list_item->i = item_to_append;
	list_item->next = NULL;
	*list_ptr = (*list_ptr)->next = list_item;
}

static void FILL(struct L *small, elem_t M, elem_t *p, elem_t *c, elem_t *P_out, unsigned *I_out) {
  unsigned i;
  struct L *cur_index;
  elem_t P = 0, C = 0, tmp;
	for (cur_index=small,i=0; cur_index!=NULL; cur_index=cur_index->next,++i) {
		tmp = C + c[cur_index->i];
		if (tmp <= M) {
			C = tmp;
			P += p[cur_index->i];
			I_out[i] = cur_index->i;
		} else {
			I_out[i] = (unsigned)-1;
		}
	}
	*P_out = P;
}

#define USE_FILL 1

void task_ibarra1975_01(elem_t *sol, struct task *task, real_t eps) {
  unsigned n = task->n;
  elem_t M = *task_get_maxweight(task);
  void *buf = alloca(2*n * sizeof(elem_t) + n * sizeof(unsigned));
  elem_t *p = (elem_t *)buf + 0, *c = (elem_t *)buf + n;
  unsigned *arrangement = (unsigned *)((elem_t *)buf + 2*n);
  unsigned i;
  elem_t C;
  elem_t P_wave;
  const real_t eps2 = eps * eps;
  real_t delta;
  unsigned g;
  struct TABLE *table;
  struct L *small, *small_end;
  unsigned small_size;
  unsigned j;

	//x = (elem_t *)alloca(n * sizeof(elem_t));
	//memset(&x[1], 0, (n-1) * sizeof(elem_t));

// Step 1
	sort(task, p, c, arrangement);
  	//memset(sol, 0, n * sizeof(*sol));

// Step 2
	C = 0; P_wave = 0;
	for (i = 0; i < n; ++i) {
	  elem_t tmp = C + c[i];
		P_wave += p[i];
		if (tmp > M)
			break;
		C = tmp;
	}
	if (i == n || C == M) {
		for (j = 0; j < i; ++j) sol[arrangement[j]] = 1;
		for (j = i; j < n; ++j) sol[arrangement[j]] = 0;
		return;
	}

	//P_wave = P_wave - p[i] + p[i]*(M-C)/c[i];
	P_wave += 50;

// Step 3
	delta = P_wave * eps2 / REAL_T(9.0);
	g = (unsigned)(REAL_T(9.0) / eps2) + 1;

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
		if (p[i]*3 <= eps*P_wave) {
			//small += (p[i], c[i]);
			append_list_end(&small_end, i, (struct L *)alloca(sizeof(struct L)));
			++small_size;
			continue; //goto Step_8;
		}

	// Step 7
	//  Step 7.1
		f_i = (unsigned)(p[i] / delta);

	//  Step 7.2
		if (g < f_i)
			continue;
		j = g - f_i;
		for (;;) {
			if (!is_empty_t(&table[j]) && table[j].W + c[i] <= M) {
				if (is_empty_t(&table[j+f_i]) || table[j+f_i].W > table[j].W+c[i]) {
					set_list(&table[j+f_i].L, table[j].L, i, (struct L *)alloca(sizeof(struct L)));
					table[j+f_i].P = table[j].P + p[i];
					table[j+f_i].W = table[j].W + c[i];
				}
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
	  unsigned k,maxk = (unsigned)-1;
	  elem_t max = 0;
		for (k = 0; k <= g; ++k) {
			if (!is_empty_t(&table[k])) {
#if defined(USE_FILL) && (USE_FILL) != 0
				FILL(small, M - table[k].W, p, c, &alpha, I);
				if (table[k].P + alpha > max) {
					tmp = I; I = maxI; maxI = tmp;
					max = table[k].P + alpha;
					maxk = k;
				}
#else
				if (table[k].P > max) {
					max = table[k].P;
					maxk = k;
				}
#endif
			}
		}
		memset(sol, 0, n * sizeof*sol);
		if (maxk != (unsigned)-1) {
		  struct L *l;
			for (l = table[maxk].L; l != NULL; l = l->next) {
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
}
