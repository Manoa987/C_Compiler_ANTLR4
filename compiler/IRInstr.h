#ifndef IRInstr_H
#define IRInstr_H

#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>
#include "Types.h"
#include "BasicBlock.h"

using namespace std;

// Declarations from the parser -- replace with your own
// #include "type.h"
// #include "symbole.h"

class BasicBlock;
class CFG;


//! The class for one 3-address instruction
class IRInstr {
 
   public:
	/**  constructor */
	IRInstr(){};
	IRInstr(BasicBlock* bb_, Type t);
	
	/** Actual code generation */
	virtual void gen_asm_x86(ostream &o) = 0; /**< x86 assembly code generation for this IR instruction */
#if 0
	virtual void gen_asm_arm(ostream &o) = 0; /**< arm assembly code generation for this IR instruction */
	virtual void gen_asm_msp430(ostream &o) = 0; /**< msp430 assembly code generation for this IR instruction */
#endif

 private:
	BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
	Type t;
};

#endif
