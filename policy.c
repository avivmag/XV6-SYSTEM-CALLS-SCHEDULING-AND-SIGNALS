#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    printf(2, "Usage: policy policy ID..\n");
    exit(-1);
  }
  if(strlen(argv[1]) > 1)
  {
    printf(2, "policy ID must be between 1 and 3.\n");
    exit(-1);
  }
  if(argv[1][0] < '1' || argv[1][0] > '3')
  {
    printf(2, "policy ID must be between 1 and 3.\n");
    exit(-1);
  }
  	
  schedp(argv[1][0] - '0');

  exit(0);
  return 0;
} 