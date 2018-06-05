#include <stddef.h>

#ifndef max
#define max(a,b) ((a)>=(b) ? (a) : (b))
#endif

#if defined (_MSC_VER) && (_MSC_VER <= 2020)
#define inline __inline
#endif

#define offset_of(s,f) ((size_t)&((s*)0)->f)
#define elements_in(arr) (sizeof(arr)/sizeof*(arr))

typedef int elem_t;

typedef float real_t;
#define REAL_T(x) (x##f)


elem_t mul_vec(elem_t *a, elem_t *b, unsigned n);




// ordinary knapsack problem

#define sizeof_task_fields (offset_of(struct task, data))
struct task {
  unsigned n;
  char data[64];
};

static inline elem_t * task_get_maxweight(struct task *task) {
	return (elem_t *)((size_t)&task->data);
}
static inline elem_t * task_get_weights(struct task *task) {
	return (elem_t *)((size_t)&task->data + (1)*sizeof(elem_t));
}
static inline elem_t * task_get_costs(struct task *task) {
	return (elem_t *)((size_t)&task->data + (1+task->n)*sizeof(elem_t));
}

#define TASK_DECL(name, n, M, c, p) struct { \
	unsigned _n; \
	elem_t _M, _c[n], _p[n]; \
	} name = {n, M, c, p}


struct task * task_create(unsigned n);
void task_delete(struct task *task);
void task_fill_random(struct task *task, elem_t max);
void task_print(struct task *task);
void task_solution_print(elem_t *sol, const char *name, struct task *task);


void task_solve_01(elem_t *sol, struct task *task);
void task_ibarra1975_01(elem_t *sol, struct task *task, real_t eps);
void glpk_solve_01(elem_t *sol, struct task *task);




// knapsack with 2 constraints

#define sizeof_task2_fields (offset_of(struct task2, data))
struct task2 {
  unsigned n;
  char data[192];
};

static inline elem_t * task2_get_maxweight(struct task2 *task) {
	return (elem_t *)(size_t)&task->data;
}
static inline elem_t * task2_get_maxvolume(struct task2 *task) {
	return (elem_t *)((size_t)&task->data + (1)*sizeof(elem_t));
}
static inline elem_t * task2_get_weights(struct task2 *task) {
	return (elem_t *)((size_t)&task->data + (2)*sizeof(elem_t));
}
static inline elem_t * task2_get_volumes(struct task2 *task) {
	return (elem_t *)((size_t)&task->data + (2+task->n)*sizeof(elem_t));
}
static inline elem_t * task2_get_costs(struct task2 *task) {
	return (elem_t *)((size_t)&task->data + (2+task->n*2)*sizeof(elem_t));
}

#define TASK2_DECL(name, n, W, V, w, v, c) struct { \
	unsigned _n; \
	elem_t _W, _V, _w[n], _v[n], _c[n]; \
	} name = {n, W, V, w, v, c}


struct task2 * task2_create(unsigned n);
void task2_delete(struct task2 *task);
void task2_fill_random(struct task2 *task, elem_t max);
void task2_print(struct task2 *task);
void task2_solution_print(elem_t *sol, const char *name, struct task2 *task);


void task2_solve_01(elem_t *sol, struct task2 *task);
void task2_ibarra1975_01(elem_t *sol, struct task2 *task, real_t eps);
