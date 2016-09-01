#include "types.h"
#include "stat.h"
#include "user.h"


void A(int a){
  printf(1,"pid %d function A got %d\n",getpid(), a);
}

void B(int b){
  printf(1,"pid %d function B got %d\n",getpid(), b);
}

int flag = 0;
void C(int c){
  printf(1,"pid %d function C got %d\n",getpid(),c);
  flag = 1;
}
void D(int d){
  printf(1,"pid %d function D got %d\n",getpid(),d);
}
void E(int e){
  printf(1,"pid %d function E got %d\n",getpid(),e);
  printf(1,"bye bye\n");
  exit(0);
}  

int main (int argc, char *argv[]){
  int i;
  int cpid;
  sighandler_t temp;
  int ppid = getpid();
  // should print default handler.
  sigsend(ppid,0);

  signal(0,A);
  // now should print the A message.
  sigsend(ppid,0);
  
  temp = signal(0,B);
  // should print the A message.
  temp(0);
  // should print the B message.
  sigsend(ppid,0);
  




  // testing signal sending between different process:
  signal(1,C);
  cpid = fork();
  
  if(cpid){ // this is the parent
    printf(1,"parent is sending signal to child\n");
    // should only accour once
    sigsend(cpid,1);
    sigsend(cpid,1);
    while(!flag);
    printf(1,"parent got signal from child\n");
    wait(0);
  }
  else { // this is the child
    while(!flag); // waiting for signal from parent_pid
    printf(1,"child got signal from parent sending signal to parent\n");
    sigsend(ppid, 1);
    exit(0);
  }
  // now test all signals
  for(i=0 ; i < 31 ; i++)
    signal(i,D);
  signal(31, E); // this should make the child exit, else go to infinite loop
  cpid = fork();
  if(cpid){
    for (i=0 ; i<32 ; i++)
      sigsend(cpid,i);

    wait(0);
  }
  else{
    while(1);
  }
  
 return 0; 
}