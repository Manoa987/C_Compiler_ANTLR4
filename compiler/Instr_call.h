#ifndef CALL_H
#define CALL_H

#include <string>
#include <iostream>

#include "IRInstr.h"
using namespace std;

class Instr_call : public IRInstr {
 
   public:
	
	/**  constructor */
	Instr_call(BasicBlock* bb_, int d, string s, int taille): bb(bb_), dest(d), nomFonction(s), nbOctetsUtilises(taille) {};
	
	/** Actual code generation */
	virtual void gen_asm_x86(ostream &o); /**< x86 assembly code generation for this IR instruction */
#if 0 
	virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
	virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */	
#endif

 private:
	BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
    int dest;
	string nomFonction;
	int nbOctetsUtilises;
};

#endif