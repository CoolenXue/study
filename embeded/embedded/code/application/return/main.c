#include <stdio.h>

//lint -e530 -e123 -e428

typedef struct {
    int i0_;
    int i1_;
    int i2_;
} func_return_t;

func_return_t foo (int _param)
{
    func_return_t local;
    int reg_esp, reg_ebp;
    
asm volatile(
    // get EBP
    "movl %%ebp, %0    \n"
    // get ESP
    "movl %%esp, %1    \n"
    : "=r" (reg_ebp), "=r" (reg_esp)
    );
    printf ("foo (): EBP = %x\n", reg_ebp);
    printf ("foo (): ESP = %x\n", reg_esp);
    printf ("foo (): (EBP) = %x\n", *(int *)reg_ebp);
    printf ("foo (): return address = %x\n", *(((int *)reg_ebp + 1)));
    local.i0_ = 1;
    local.i1_ = 2;
    local.i2_ = 3;
    printf ("foo (): &_param = %p\n", &_param);
    printf ("foo (): return value = %x\n", *(((int *)&_param) - 1));
    printf ("foo (): &local = %p\n", &local);
    printf ("foo (): &reg_esp = %p\n", &reg_esp);
    printf ("foo (): &reg_ebp = %p\n", &reg_ebp);
    return local;
}

int main ()
{
    int reg_esp, reg_ebp;
    func_return_t local = foo (100);
    
asm volatile(
    // get EBP
    "movl %%ebp, %0    \n"
    // get ESP
    "movl %%esp, %1    \n"
    : "=r" (reg_ebp), "=r" (reg_esp)
    );
    printf ("main (): EBP = %x\n", reg_ebp);
    printf ("main (): ESP = %x\n", reg_esp);
    printf ("main (): &local = %p\n", &local);
    printf ("main (): &reg_esp = %p\n", &reg_esp);
    printf ("main (): &reg_ebp = %p\n", &reg_ebp);
    return 0;
}
