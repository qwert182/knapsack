#include "common.h"
#include "platform.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#define N 7

int main(int argc, char **argv) {
  struct task *task;
  elem_t sol[N];
  elem_t sol_ibarra[N];
  //int ch = -1;
  unsigned i = 0, tasks_n = 1000000;
  m_time start, end, diff;
  elem_t P_star, P_roof;
  real_t eps;
  static real_t const eps_arr[] = {REAL_T(1.0), REAL_T(0.9), REAL_T(0.8), REAL_T(0.7), REAL_T(0.6), REAL_T(0.5), REAL_T(0.4), REAL_T(0.3), REAL_T(0.2)};
  unsigned eps_i;
	(void)(argc, argv);
	task = task_create(N);

	m_time_get(&start);
	for (; i < tasks_n;) {
		++i;
		task_fill_random(task, 20);
		task_solve_01(sol, task);
		P_star = mul_vec(sol, task_get_costs(task), N);
		//for (eps = REAL_T(0.1); eps <= REAL_T(1.0); eps += REAL_T(0.09)) {
		for (eps_i = 0; eps_i < elements_in(eps_arr); ++eps_i) {
			eps = eps_arr[eps_i];
			task_ibarra1975_01(sol_ibarra, task, eps);
			P_roof = mul_vec(sol_ibarra, task_get_costs(task), task->n);
			if (!(P_roof >= P_star * (REAL_T(1.0) - eps)) && P_roof != 0) {
			//if (!((P_star - P_roof)/(real_t)P_star <= eps/3.0f)) {
				printf("error eps = %lf\n", eps);
				task_print(task);
				task_solution_print(sol, "sol", task);
				task_solution_print(sol_ibarra, "sol_ibarra", task);
				task_ibarra1975_01(sol_ibarra, task, eps);
				printf("\n");
			}
		}
	}
	m_time_get(&end);
	m_time_diff(&diff, &start, &end);
	printf("time = "M_TIME_SEC_MSEC_FMT" sec, iters = %u, time/iter = %lf usec\n", M_TIME_SEC_MSEC_ARGS(diff), i, (double)M_TIME_IN_USEC(diff)/i);

	task_delete(task);
	return 0;
}
/*
int main(int argc, char **argv) {
  struct task2 *task;
  elem_t sol[N];
  elem_t sol_ibarra[N];
  //int ch = -1;
  unsigned i = 0, tasks_n = 100000;
  unsigned start, end;
  elem_t P_star, P_roof;
  real_t eps;
  static real_t const eps_arr[] = {REAL_T(1.0), REAL_T(0.9), REAL_T(0.8), REAL_T(0.7), REAL_T(0.6), REAL_T(0.5), REAL_T(0.4), REAL_T(0.3), REAL_T(0.2)};
  unsigned eps_i;
	(void)(argc, argv);
	task = task2_create(N);

	start = GetTickCount();
	for (; i < tasks_n;) {
		++i;
		task2_fill_random(task, 30);
		task2_solve_01(sol, task);
		P_star = mul_vec(sol, task2_get_costs(task), N);
{extern elem_t task2_ibarra1975_01__P_star;
task2_ibarra1975_01__P_star = P_star;}
		//for (eps = REAL_T(0.1); eps <= REAL_T(1.0); eps += REAL_T(0.09)) {
		for (eps_i = 0; eps_i < elements_in(eps_arr); ++eps_i) {
			eps = eps_arr[eps_i];
			task2_ibarra1975_01(sol_ibarra, task, eps);
			P_roof = mul_vec(sol_ibarra, task2_get_costs(task), task->n);
			if (!(P_roof >= P_star * (REAL_T(1.0) - eps)) ) {
			//if (!((P_star - P_roof)/(real_t)P_star <= eps/3.0f)) {
				printf("error eps = %lf\n", eps);
				task2_print(task);
				task2_solution_print(sol, "sol       ", task);
				task2_solution_print(sol_ibarra, "sol_ibarra", task);
				//task2_ibarra1975_01(sol_ibarra, task, eps);
				printf("\n");
			}
		}
	}
	end = GetTickCount();
	printf("time = %u.%03u sec, iters = %u, time/iter = %lf usec\n", (end - start)/1000, (end - start)%1000, i, (double)((end - start)*1000)/i);

	task2_delete(task);
	return 0;
}
*/
