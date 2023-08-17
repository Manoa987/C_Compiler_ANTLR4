#include "Instr_comp.h"
using namespace std;


/**< x86 assembly code generation for this IR instruction */
void Instr_comp::gen_asm_x86(ostream &o){
	if (compareWithOne) {
		o << "	cmpl $1, -" << x << "(\%rbp)\n";
	} else {
		o << "	movl -" << y << "(\%rbp), \%ebx\n";
		o << "	movl -" << x << "(\%rbp), \%eax\n";
		o << "	cmpl \%ebx, \%eax\n";
		if (operateur.compare("==")==0){
			o << "	sete  \%al\n";
		} else if (operateur.compare("!=")==0){
			o << "	setne  \%al\n";
		} else if (operateur.compare("<")==0){
			o << "	setl  \%al\n";
		} else if (operateur.compare("<=")==0){
			o << "	setle  \%al\n";
		} else if (operateur.compare(">")==0){
			o << "	setg  \%al\n";
		} else if (operateur.compare(">=")==0){
			o << "	setge  \%al\n";
		}
		o << "	movzb  \%al, \%eax\n";
		o << "	movl \%eax, -" << dest << "(\%rbp)\n";
	}
}
// virtual void gen_asm_arm(ostream &o); /**< arm assembly code generation for this IR instruction */
// virtual void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */


