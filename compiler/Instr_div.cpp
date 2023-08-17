#include "Instr_div.h"
using namespace std;


/**< x86 assembly code generation for this IR instruction */
void Instr_div::gen_asm_x86(ostream &o){
	o << "	movl -" << numerateur << "(\%rbp), \%eax\n";
	o << "	cltd\n";
	o << "	idivl -" << denominateur << "(\%rbp)\n";
	o << " 	movl " << "\%eax, -" << dest << "(\%rbp)\n";
}


// virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
// virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */
