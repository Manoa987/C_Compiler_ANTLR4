#ifndef LDCONST_H
#define LDCONST_H

#include <string>
#include <iostream>

#include "IRInstr.h"
using namespace std;

class Instr_ldconst : public IRInstr {
 
   public:
	
	/**  constructor */
	Instr_ldconst(BasicBlock* bb_, Type t, int v, int n): bb(bb_), t(t), val(v), dest(n){};
	
	/** Actual code generation */
	virtual void gen_asm_x86(ostream &o); /**< x86 assembly code generation for this IR instruction */
#if 0 
	virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
	virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */	
#endif

 private:
	BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
	Type t;
    int val;
    int dest;
};

#endif