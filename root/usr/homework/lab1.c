// lab1.c -- timer, interrupt and output

#include <u.h>


//TIME instruction to set the timer
stmr(int val)   { asm(LL,8); asm(TIME); }
//IVEC instruction to set the interrupt vector
ivec(void *isr) { asm(LL,8); asm(IVEC); }
//BOUT instruction for output
out(port, val)  { asm(LL,8); asm(LBL,16); asm(BOUT); }

//HALT instruction to stop the system
halt(val)       { asm(LL,8); asm(HALT); }

int current;

//the interrupt vector(function)
alltraps()
{
  asm(PSHA);
  asm(PSHB);

  current++;

  asm(POPB);
  asm(POPA);
  asm(RTI);
}

main()
{
  current = 0;

  stmr(2000);
  ivec(alltraps);
  
  //turn on the interrupt
  asm(STI);
  
  while (current < 10) {
    if (current & 1) out(1, '1'); else out(1, '0');
  }

  halt(0);
}
