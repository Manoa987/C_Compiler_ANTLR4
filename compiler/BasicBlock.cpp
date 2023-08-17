#include "BasicBlock.h"
using namespace std;


void BasicBlock::gen_asm_x86(ostream &o){
	for(auto instr: instrs){
		instr->gen_asm_x86(o);
	}
}
void BasicBlock::gen_asm_arm(ostream &o){
	for(auto instr: instrs){
		//instr->gen_asm_arm(o);
	}
}

void BasicBlock::gen_asm_msp430(ostream &o){
	for(auto instr: instrs){
		//instr->gen_asm_msp430(o);
	}
}
void BasicBlock::add_IRInstr(IRInstr *instr){
	instrs.push_back(instr);
}
