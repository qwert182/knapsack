#include <stddef.h>

#if defined (_MSC_VER) && (_MSC_VER <= 2020)
#define inline __inline
#endif

#define offset_of(s,f) ((unsigned)&((s*)0)->f)

typedef int elem_t;

typedef float real_t;
#define REAL_T(x) (x##f)

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
elem_t mul_vec(elem_t *a, elem_t *b, unsigned n);
void solution_print(elem_t *sol, const char *name, struct task *task);


void solve_01(elem_t *sol, struct task *task);
void ibarra1975_01(elem_t *sol, struct task *task, real_t eps);
