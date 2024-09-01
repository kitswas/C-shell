#ifndef SHELL_STATE_H
#define SHELL_STATE_H

#include "shelltypes.h"

struct job *get_job(pid_t pgid);

void add_jobs_to_store(struct job *j);

struct job *get_job_list();

void list_jobs();

void remove_job_from_store(pid_t pgid);

#endif
