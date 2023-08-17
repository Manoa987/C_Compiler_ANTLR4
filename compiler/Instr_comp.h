#ifndef COMP_H
#define COMP_H

#include <string>
#include <iostream>

#include "IRInstr.h"
using namespace std;

class Instr_comp : public IRInstr {
 
   public:
	
	/**  constructor */
	Instr_comp(BasicBlock* bb_, string op, bool one, int d, int x, int y): bb(bb_), operateur(op), compareWithOne(one), dest(d), x(x), y(y) {};

	/** Actual code generation */
	virtual void gen_asm_x86(ostream &o); /**< x86 assembly code generation for this IR instruction */
#if 0 
	virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
	virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */
#endif
 private:
	BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
	string operateur;
	bool compareWithOne;
    int dest;
	int x;
	int y;
};

#endif