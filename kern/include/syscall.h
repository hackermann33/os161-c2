/*
 * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _SYSCALL_H_
#define _SYSCALL_H_


#include <cdefs.h> /* for __DEAD */
#include "opt-syscalls.h"
#include "opt-fork.h"
#include "opt-file.h"
#include "opt-c2.h"
#include <stat.h>

struct trapframe; /* from <machine/trapframe.h> */

struct proc; /* from somewhere */

/*
 * The system call dispatcher.
 */

void syscall(struct trapframe *tf);

/*
 * Support functions.
 */

/* Helper for fork(). You write this. */
void enter_forked_process(struct trapframe *tf);

/* Enter user mode. Does not return. */
__DEAD void enter_new_process(int argc, userptr_t argv, userptr_t env,
		       vaddr_t stackptr, vaddr_t entrypoint);


/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys___time(userptr_t user_seconds, userptr_t user_nanoseconds);
#if OPT_SYSCALLS
#if OPT_FILE
//struct openfile;
struct openfile {
  struct vnode *vn;
  off_t offset;	
  int flags;
  unsigned int countRef;
  struct lock* filelock;
};

void openfileIncrRefCount(struct openfile *of);
int sys_open(userptr_t path, int openflags, mode_t mode, int *errp);
int sys_close(int fd, int *err);
int sys_lseek( int fd, off_t offset, int whence, int64_t *retval );
#endif

#if OPT_C2
int sys_write(int fd, userptr_t buf_ptr, size_t size, int *err);
int sys_read(int fd, userptr_t buf_ptr, size_t size, int *err);
void sys__exit(int status);
int sys_waitpid(pid_t pid, userptr_t statusp, int options, int valid_anyway, int *err);
pid_t sys_getpid(void);
pid_t sys_getpid2(struct proc* p);
#if OPT_FORK
int sys_fork(struct trapframe *ctf, pid_t *retval);
#endif
int sys_execv(char *progname, char **args, int *err);
int sys_fstat (int fd, struct stat *buf);
int sys___getcwd(userptr_t buf, size_t size, int *retval);
int sys_chdir(userptr_t dir);
int sys_mkdir(userptr_t dir, mode_t mode);
int sys_rmdir(userptr_t dir);
int sys_getdirentry(int fd, userptr_t buf, size_t buflen);

int sys_dup2(int oldfd,int newfd, int *retval);

#endif

#endif

#endif /* _SYSCALL_H_ */
