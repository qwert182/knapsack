#include "common.h"
#include "platform.h"
#include <stdio.h>
//#include <memory.h>
//#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// mode = static-tests {
//   .solver : solver
// }
// mode = random-tasks {
//   .N : int
//   .solver : solver {
//     .parameters : parameters
//   }
//   .parameter-space : parameter-space {
//   }
// }

#define OPTION_MODE "mode"
#define OPTION_CONSTRAINTS "constraints"
#define OPTION_TYPE "type"
#define OPTION_SOLVER "solver"
#define OPTION_HELP "help"
struct raw_options {
  const char *mode, *constraints, *type, *solver;
};

#define CHECK_ARG_FOR_OPTION(arg, name, value) \
	if (strncmp(arg, "--"name"=", sizeof("--"name"=")-1) == 0) { \
		value = arg + sizeof("--"name"=") - 1; \
	}

#define MODE_STATIC_TESTS "static-tests"
#define MODE_RANDOM_TASKS "random-tasks"

static void show_usage(FILE *f) {
	fprintf(f,
	           "knapsack-test [options]\n"
	           "  --"OPTION_MODE"="MODE_STATIC_TESTS"|"MODE_RANDOM_TASKS"\n"
	           "  --"OPTION_CONSTRAINTS"=1|2\n"
	           "  --"OPTION_TYPE"=bin|int\n"
	           "  --"OPTION_SOLVER"=exact|ibarra75\n"
	);
}

static int read_options(int argc, char **argv, struct raw_options *opts) {
  int i;
	for (i = 1; i < argc; ++i) {
		CHECK_ARG_FOR_OPTION(argv[i], OPTION_MODE, opts->mode)
		else CHECK_ARG_FOR_OPTION(argv[i], OPTION_CONSTRAINTS, opts->constraints)
		else CHECK_ARG_FOR_OPTION(argv[i], OPTION_TYPE, opts->type)
		else CHECK_ARG_FOR_OPTION(argv[i], OPTION_SOLVER, opts->solver)
		else if (strcmp(argv[i], "--"OPTION_HELP) == 0) {
			show_usage(stderr);
			return 1;
		} else {
			fprintf(stderr, "ERROR: unknown option '%s'\n", argv[i]);
			show_usage(stderr);
			return 1;
		}
	}
	return 0;
}

struct when_to_stop {
	unsigned tasks_n;
	unsigned time_in_sec;
};

struct random_parameters {
	elem_t max_value;
};

struct exact_solver_params {
	void (*run)(elem_t *sol, struct task *task);
};

struct solver_parameters_context {
	struct solver_parameter * (*next)(struct solver_parameters_context *);
};

struct solver_params {
	void (*init_params_context)(struct solver_parameters_context **);
	void (*run)(elem_t *sol, struct task *task, struct solver_parameter *);
	int (*check_solution)(elem_t sol_val_approx, elem_t sol_val_exact, struct solver_parameter *solver_param);
};

struct runner_params {
	unsigned N;
	struct when_to_stop stop_on;
	struct random_parameters random;
	struct exact_solver_params exact_solver;
	struct solver_params solver;
};

struct ibarra1975_parameters {
	real_t eps;
};

struct ibarra1975_parameters_context {
	struct solver_parameters_context parent;
	struct ibarra1975_parameters params;
	int eps_i;
};

struct solver_parameter *stupid_epsilon_iterator_next(struct solver_parameters_context *in_ctx) {
  struct ibarra1975_parameters_context *ctx = (void*)in_ctx;
	if (ctx->eps_i > 0) {
		ctx->params.eps = (real_t)ctx->eps_i / REAL_T(10.0);
		--ctx->eps_i;
		return (struct solver_parameter *)&ctx->params;
	} else {
		return NULL;
	}
}

static void stupid_epsilon_iterator_init_context(struct solver_parameters_context **out_ctx) {
  struct ibarra1975_parameters_context *ctx;
	ctx = malloc(sizeof(*ctx));
	ctx->parent.next = stupid_epsilon_iterator_next;
	ctx->eps_i = 10;
	*out_ctx = &ctx->parent;
}

static void ibarra1975_run(elem_t *sol, struct task *task, struct solver_parameter *in_param) {
  struct ibarra1975_parameters *param = (void*)in_param;
	task_ibarra1975_01(sol, task, param->eps);
}

static int ibarra1975_check_solution(elem_t sol_val_approx, elem_t sol_val_exact, struct solver_parameter *in_param) {
  struct ibarra1975_parameters *param = (void*)in_param;
	return sol_val_approx >= sol_val_exact * (REAL_T(1.0) - param->eps);
}

static int run_random_tasks(struct runner_params *params) {
  struct task *task;
  elem_t *sol_exact = malloc(params->N * sizeof*sol_exact);
  elem_t *sol_approx = malloc(params->N * sizeof*sol_approx);
  //int ch = -1;
  unsigned tasks_ran;
  m_time time_start, time_end, time_diff;
  elem_t sol_val_exact, sol_val_approx;
  struct solver_parameters_context *solver_params_context;
  struct solver_parameter *solver_param;
	task = task_create(params->N);

	tasks_ran = 0;
	m_time_get(&time_start);
	while (tasks_ran < params->stop_on.tasks_n) {
		task_fill_random(task, params->random.max_value);
		params->exact_solver.run(sol_exact, task);
		sol_val_exact = mul_vec(sol_exact, task_get_costs(task), params->N);

		params->solver.init_params_context(&solver_params_context);
		while ((solver_param = solver_params_context->next(solver_params_context)) != NULL) {
			params->solver.run(sol_approx, task, solver_param);
			sol_val_approx = mul_vec(sol_approx, task_get_costs(task), params->N);
			if (!params->solver.check_solution(sol_val_approx, sol_val_exact, solver_param)) {
				//printf("error eps = %lf\n", eps);
				task_print(task);
				task_solution_print(sol_exact, "sol_exact", task);
				task_solution_print(sol_approx, "sol_approx", task);
				printf("\n");
			}
		}
		free(solver_params_context);

		++tasks_ran;
	}
	m_time_get(&time_end);
	m_time_diff(&time_diff, &time_start, &time_end);
	printf("time = "M_TIME_SEC_MSEC_FMT" sec, iters = %u, time/iter = %lf usec\n", M_TIME_SEC_MSEC_ARGS(time_diff), tasks_ran, M_TIME_IN_USEC_DBL(time_diff)/tasks_ran);

	task_delete(task);
	free(sol_approx);
	free(sol_exact);
	return 0;
}

int main(int argc, char **argv) {
  struct raw_options raw_opts = {0};
  struct runner_params params = {5, {1000000,0}, {20}, {task_solve_01}, {stupid_epsilon_iterator_init_context, ibarra1975_run, ibarra1975_check_solution}};
	// general logic
	// read options
	if (read_options(argc, argv, &raw_opts) != 0) return 1;

	return run_random_tasks(&params);

	// select operating mode
	//   static tests | tests on random tasks: {for check, for performance} | solve problem from stdin/file (need?)
	// select problem dimensions
	//   1 constraint | 2 constraints
	// select problem type
	//   {0/1} (for ibarra75) | unbounded (for kohli92) | linear (need?)
	// select solver
	//   greedy | kohli92 | ibarra75 (eps) | exact | glpk (probably has eps) | lp_solve ?
	//   and its options (like eps, ...)
	// what about testing many eps ?
	// do it in general mean (call abstract methods)

	//printf("mode=%s, constraints=%s, type=%s, solver=%s\n", mode, constraints, type, solver);
/*
	if (raw_opts.mode == NULL) {
		fprintf(stderr, "ERROR: "OPTION_MODE" is not set\n");
		return 1;
	} else if (strcmp(raw_opts.mode, MODE_STATIC_TESTS) == 0) {
	} else if (strcmp(raw_opts.mode, MODE_RANDOM_TASKS) == 0) {
	} else {
		fprintf(stderr, "ERROR: unknown "OPTION_MODE"='%s'\n", raw_opts.mode);
		return 1;
	}
*/
}
