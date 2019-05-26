#include <stdio.h>

//lint -e530 -e123

void tail (int _param)
{
    int local = 0;
    int reg_esp, reg_ebp;
    
asm volatile(
    // get EBP
    "movl %%ebp, %0    \n"
    // get ESP
    "movl %%esp, %1    \n"
    : "=r" (reg_ebp), "=r" (reg_esp)
    );
    printf ("tail (): EBP = %x\n", reg_ebp);
    printf ("tail (): ESP = %x\n", reg_esp);
    printf ("tail (): (EBP) = %x\n", *(int *)reg_ebp);
    printf ("tail (): return address = %x\n", *(((int *)reg_ebp + 1)));
    printf ("tail (): &local = %p\n", &local);
    printf ("tail (): &reg_esp = %p\n", &reg_esp);
    printf ("tail (): &reg_ebp = %p\n", &reg_ebp);
    printf ("tail (): &_param = %p\n", &_param);
}

int middle (int _p0, int _p1, int _p2)
{
    int reg_esp, reg_ebp;
    
asm volatile(
    // get EBP
    "movl %%ebp, %0    \n"
    // get ESP
    "movl %%esp, %1    \n"
    : "=r" (reg_ebp), "=r" (reg_esp)
    );
    tail (_p0);
    printf ("middle (): EBP = %x\n", reg_ebp);
    printf ("middle (): ESP = %x\n", reg_esp);
    printf ("middle (): (EBP) = %x\n", *(int *)reg_ebp);
    printf ("middle (): return address = %x\n", *(((int *)reg_ebp + 1)));
    printf ("middle (): &reg_esp = %p\n", &reg_esp);
    printf ("middle (): &reg_ebp = %p\n", &reg_ebp);
    printf ("middle (): &_p0 = %p\n", &_p0);
    printf ("middle (): &_p1 = %p\n", &_p1);
    printf ("middle (): &_p2 = %p\n", &_p2);
    return 1;
}

int main ()
{
    int reg_esp, reg_ebp;
    int local = middle (1, 2, 3);
    
asm volatile(
    // get EBP
    "movl %%ebp, %0    \n"
    // get ESP
    "movl %%esp, %1    \n"
    : "=r" (reg_ebp), "=r" (reg_esp)
    );
    printf ("main (): EBP = %x\n", reg_ebp);
    printf ("main (): ESP = %x\n", reg_esp);
    printf ("main (): (EBP) = %x\n", *(int *)reg_ebp);
    printf ("main (): return address = %x\n", *(((int *)reg_ebp + 1)));
    printf ("main (): &reg_esp = %p\n", &reg_esp);
    printf ("main (): &reg_ebp = %p\n", &reg_ebp);
    printf ("main (): &local = %p\n", &local);
    return 0;
}
