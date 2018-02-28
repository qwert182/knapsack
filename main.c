#include "common.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <Windows.h>

#if 0

#define EPS REAL_T(0.4)

static void test01_1(void) {
  #define P {15,17,18,19,20}
  #define C { 4, 6, 7, 9,10}
  static TASK_DECL(_task, 5, 15, C, P);
	elem_t sol[5], sol_ibarra[5];
	static elem_t const rsol[] = {0,1,0,1,0};
	solve_01(sol, (struct task *)&_task);
	if (memcmp(sol, rsol, sizeof rsol)) fprintf(stderr, "err\n");
	task_print((struct task *)&_task);
	solution_print(sol, "sol", (struct task *)&_task);
	ibarra1975_01(sol_ibarra, (struct task *)&_task, EPS);
	solution_print(sol_ibarra, "sol_ibarra", (struct task *)&_task);
  #undef C
  #undef P
}

static void test01_2(void) {
  #define P {24,14,16,17,18}
  #define C { 3, 4, 6, 7, 8}
  static TASK_DECL(_task, 5, 12, C, P);
	elem_t sol[5], sol_ibarra[5];
	static elem_t const rsol[] = {1,0,0,0,1};
	solve_01(sol, (struct task *)&_task);
	if (memcmp(sol, rsol, sizeof rsol)) fprintf(stderr, "err\n");
	task_print((struct task *)&_task);
	solution_print(sol, "sol", (struct task *)&_task);
	ibarra1975_01(sol_ibarra, (struct task *)&_task, EPS);
	solution_print(sol_ibarra, "sol_ibarra", (struct task *)&_task);
  #undef C
  #undef P
}

static void test01_3(void) {
  #define P {4,1,2,1,3}
  #define C {3,4,6,7,8}
  static TASK_DECL(_task, 5, 12, C, P);
	elem_t sol[5], sol_ibarra[5];
	static elem_t const rsol[] = {1,0,0,0,1};
	solve_01(sol, (struct task *)&_task);
	if (memcmp(sol, rsol, sizeof rsol)) fprintf(stderr, "err\n");
	task_print((struct task *)&_task);
	solution_print(sol, "sol", (struct task *)&_task);
	ibarra1975_01(sol_ibarra, (struct task *)&_task, EPS);
	solution_print(sol_ibarra, "sol_ibarra", (struct task *)&_task);
  #undef C
  #undef P
}

static void test01_4(void) {
  #define P {4,1,2,1,3}
  #define C {3,4,6,7,8}
  static TASK_DECL(_task, 5, 3, C, P);
	elem_t sol[5], sol_ibarra[5];
	static elem_t const rsol[] = {1,0,0,0,0};
	solve_01(sol, (struct task *)&_task);
	if (memcmp(sol, rsol, sizeof rsol)) fprintf(stderr, "err\n");
	task_print((struct task *)&_task);
	solution_print(sol, "sol", (struct task *)&_task);
	ibarra1975_01(sol_ibarra, (struct task *)&_task, EPS);
	solution_print(sol_ibarra, "sol_ibarra", (struct task *)&_task);
  #undef C
  #undef P
}

static void test01_5(void) {
  #define P {2, 16, 11, 18, 20}
  #define C {4,  7,  9,  4,  8}
  static TASK_DECL(_task, 5, 19, C, P);
	elem_t sol[5], sol_ibarra[5];
	static elem_t const rsol[] = {0, 1, 0, 1, 1};
	solve_01(sol, (struct task *)&_task);
	if (memcmp(sol, rsol, sizeof rsol)) fprintf(stderr, "err\n");
	task_print((struct task *)&_task);
	solution_print(sol, "sol", (struct task *)&_task);
	ibarra1975_01(sol_ibarra, (struct task *)&_task, EPS);
	solution_print(sol_ibarra, "sol_ibarra", (struct task *)&_task);
  #undef C
  #undef P
}


static void test01_6(void) {
  #define P {3, 4, 20, 2, 11}
  #define C {2, 5, 7, 10, 7}
  static TASK_DECL(_task, 5, 6, C, P);
	elem_t sol[5], sol_ibarra[5];
	static elem_t const rsol[] = {0, 1, 0, 0, 0};
	solve_01(sol, (struct task *)&_task);
	if (memcmp(sol, rsol, sizeof rsol)) fprintf(stderr, "err\n");
	task_print((struct task *)&_task);
	solution_print(sol, "sol", (struct task *)&_task);
	ibarra1975_01(sol_ibarra, (struct task *)&_task, EPS);
	solution_print(sol_ibarra, "sol_ibarra", (struct task *)&_task);
  #undef C
  #undef P
}

// this test violates assumption that every c_i <= M
static void test01_7(void) {
  #define P {13, 16, 1, 2, 17}
  #define C {10, 6, 1, 4, 6}
  static TASK_DECL(_task, 5, 4, C, P);
	elem_t sol[5], sol_ibarra[5];
	static elem_t const rsol[] = {0, 0, 0, 1, 0};
	solve_01(sol, (struct task *)&_task);
	if (memcmp(sol, rsol, sizeof rsol)) fprintf(stderr, "err\n");
	task_print((struct task *)&_task);
	solution_print(sol, "sol", (struct task *)&_task);
	ibarra1975_01(sol_ibarra, (struct task *)&_task, EPS);
	solution_print(sol_ibarra, "sol_ibarra", (struct task *)&_task);
  #undef C
  #undef P
}

// this test produces solution which have an error = (19-15)/19 > 0.2 in case SMALL isn't used by ibarra1975_01
static void test01_8(void) {
  #define P {5, 15, 2, 13, 2}
  #define C {15, 5, 2, 16, 6}
  static TASK_DECL(_task, 5, 17, C, P);
	elem_t sol[5], sol_ibarra[5];
	static elem_t const rsol[] = {0, 0, 0, 1, 0};
	solve_01(sol, (struct task *)&_task);
	if (memcmp(sol, rsol, sizeof rsol)) fprintf(stderr, "err\n");
	task_print((struct task *)&_task);
	solution_print(sol, "sol", (struct task *)&_task);
	ibarra1975_01(sol_ibarra, (struct task *)&_task, REAL_T(0.2));
	solution_print(sol_ibarra, "sol_ibarra", (struct task *)&_task);
  #undef C
  #undef P
}



int main() {
	//test01_1();
	//test01_2();
	//test01_3();
	//test01_4();
	//test01_5();
	//test01_6();
	//test01_7();
	test01_8();
}


#else


#define N 10

int main(int argc, char **argv) {
  struct task *task;
  elem_t sol[N];
  elem_t sol_ibarra[N];
  //int ch = -1;
  unsigned i = 0, tasks_n = 10;
  unsigned start, end;
  elem_t P_star, P_roof;
  real_t eps;
  static real_t const eps_arr[] = {REAL_T(1.0), REAL_T(0.9), REAL_T(0.8), REAL_T(0.7), REAL_T(0.6), REAL_T(0.5), REAL_T(0.4), REAL_T(0.3), REAL_T(0.2)};
  unsigned eps_i;
	(void)(argc, argv);
	task = task_create(N);
	//scanf("%u", &tasks_n);
	//{
	//  char must_be_not_written;
	//	if (argc < 2 || sscanf(argv[1], "%u%c", &tasks_n, &must_be_not_written) != 1) {
	//		fprintf(stderr, "ERROR: wrong arguments\n");
	//		//exit(1);
	//	}
	//}

	start = GetTickCount();
	for (; i < tasks_n;) {
		//if (i >= tasks_n) break;
		++i;
		task_fill_random(task, 20);
		solve_01(sol, task);
		P_star = mul_vec(sol, task_get_costs(task), N);
		//for (eps = REAL_T(0.1); eps <= REAL_T(1.0); eps += REAL_T(0.09)) {
		for (eps_i = 0; eps_i < sizeof(eps_arr)/sizeof(*eps_arr); ++eps_i) {
			eps = eps_arr[eps_i];
			ibarra1975_01(sol_ibarra, task, eps);
			P_roof = mul_vec(sol_ibarra, task_get_costs(task), task->n);
			if (1 || !(P_roof >= P_star * (REAL_T(1.0) - eps)) && P_roof != 0) {
			//if (!((P_star - P_roof)/(real_t)P_star <= eps/3.0f)) {
				printf("error eps = %lf\n", eps);
				task_print(task);
				solution_print(sol, "sol", task);
				solution_print(sol_ibarra, "sol_ibarra", task);
				ibarra1975_01(sol_ibarra, task, eps);
				printf("\n");
			}
		}
	}
	end = GetTickCount();
	printf("time = %u.%03u sec, iters = %u, time/iter = %lf usec\n", (end - start)/1000, (end - start)%1000, i, (double)((end - start)*1000)/i);

	task_delete(task);
	return 0;
}


#endif
