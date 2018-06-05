#include "common.h"
#include <glpk.h>
#include <stdio.h>
#include <assert.h>

void glpk_solve_01(elem_t *sol, struct task *task) {
  glp_prob *p;
  //glp_smcp lparm;
  glp_iocp iparm;
  //int ia[1+1000];
  int ja[1+10], i;
  double ar[1+10];
  elem_t *a = task_get_weights(task), *c = task_get_costs(task);
  elem_t b = *task_get_maxweight(task);
  int err;

	//task_solve_01(sol, task);

	p = glp_create_prob();
	//glp_init_smcp(&lparm);
	//lparm.msg_lev = GLP_MSG_ERR;
	glp_init_iocp(&iparm);
	iparm.presolve = GLP_ON;
	iparm.msg_lev = GLP_MSG_ERR;
	//glp_set_prob_name(p, "glpk_solve_01");
	glp_set_obj_dir(p, GLP_MAX);
	glp_add_rows(p, 1);
	//glp_set_row_name(p, 1, "p");
	glp_set_row_bnds(p, 1, GLP_UP, 0.0, b);
	glp_add_cols(p, task->n);
	for (i = 1; i <= task->n; ++i) {
		//glp_set_col_name(p, i, NULL);
		//glp_set_col_bnds(p, i, GLP_LO, 0.0, 0.0);
		glp_set_col_kind(p, i, GLP_BV);
		glp_set_obj_coef(p, i, c[i-1]);
	}

	for (i = 1; i <= task->n; ++i) {
		//ia[i] = 1;
		ja[i] = i, ar[i] = a[i-1];
	}

	//glp_load_matrix(p, task->n, ia, ja, ar);
	glp_set_mat_row(p, 1, task->n, ja, ar);
	//err = glp_simplex(p, &lparm);
	//assert(err == 0);
	err = glp_intopt(p, &iparm);
	assert(err == 0);
	err = glp_mip_status(p);
	assert(err == GLP_OPT);
	//z = glp_get_obj_val(p);
	for (i = 1; i <= task->n; ++i) {
		//sol[i-1] = glp_get_col_prim(p, i);
		sol[i-1] = glp_mip_col_val(p, i);
		//assert(sol[i-1] == glp_mip_col_val(p, i));
	}
	glp_delete_prob(p);
}
