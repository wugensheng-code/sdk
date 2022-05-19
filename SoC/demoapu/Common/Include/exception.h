#ifndef __EXCEPTION_H
#define __EXCEPTION_H
struct pt_regs {
	unsigned long regs[31];
	unsigned long elr;
	unsigned long esr;
	unsigned long pstate;
};

#endif
