#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "shelltypes.h"

int execute(struct command *cmd);

void launch_job(struct job *j);

void init_shell();

#endif
