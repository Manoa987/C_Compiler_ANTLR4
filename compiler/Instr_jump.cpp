#include "Instr_jump.h"
using namespace std;


/**< x86 assembly code generation for this IR instruction */
void Instr_jump::gen_asm_x86(ostream &o){
	
	if (sautConditionel){
		o << "	jne " << jumpFalse << "\n";
		o << "	jmp " << jumpTrue << "\n";
	} else {
		o << "	jmp " << jumpTrue << "\n";
	}
}


// virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
// virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */


