#include "types.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "perf.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int status;

  if(argint(0, &status) < 0)
    return -1;
  exit(status);
  return 0;  // not reached
}

int
sys_wait(void)
{
  int *status;

  if(argptr(0, (char**) &status, sizeof(int*)) < 0)
    return -1;
  
  return wait(status);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_schedp(void)
{
  int sched_policy_id;

  if(argint(0, &sched_policy_id) < 0)
    return -1;
  return schedp(sched_policy_id);
}

int
sys_priority(void)
{
  int pr;

  if(argint(0, &pr) < 0)
    return -1;
  
  return priority(pr);
}

int
sys_wait_stat(void)
{
  int* status;
  struct perf * p;

  if(argptr(0, (char**) &status, sizeof(int*)) < 0 || argptr(1, (char**) &p, sizeof(int*)) < 0)
    return -1;
  
  return wait_stat(status, p);
}

int
sys_signal(void)
{
  int signum;
  sighandler_t handler;
  
  if(argint(0, &signum) < 0 || argptr(1, (char**) &handler, sizeof(sighandler_t)) < 0)
    return -1;
  
  // failure in case of signal was sent that is out of range.
  if(signum < 0 || signum >= NUMSIG)
    return -1;
  
  sighandler_t temp = proc->signalHandler[signum];
  proc->signalHandler[signum] = handler;
  return (int) temp;
}

int 
sys_sigsend(void)
{
  int pid, signum;

  if(argint(0, &pid) < 0 || argint(1, &signum) < 0)
    return -1;
  
  // failure in case of signal was sent that is out of range.
  if(signum < 0 || signum >= NUMSIG)
    return -1;
  
  return sigsend(pid, signum);
}


int 
sys_sigreturn(void)
{
  return sigreturn();
}