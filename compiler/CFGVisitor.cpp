#include "CFG.h"

#include "CFGVisitor.h"
#include <stdexcept>
using namespace std;

antlrcpp::Any CFGVisitor::visitProg(ifccParser::ProgContext *context) 
{
	//cout << "#visitProg" << endl;
	visitChildren(context);

	return 0;
}

antlrcpp::Any CFGVisitor::visitFunctionList(ifccParser::FunctionListContext *context){
	//cout << "#visitFunctionList" << endl;
	auto functions = context->function();

	for(auto function : functions){
		string name = function->children[1]->getText();
		CFG* cfg = new CFG(name);
		fonctions[name] = stringToType(function->children[0]->getText());
		currentCFG = cfg;
		visit(function);
	}
	return 0;
}

antlrcpp::Any CFGVisitor::visitFunction(ifccParser::FunctionContext *context) {
	//cout<< "#visitFunction" << endl;
	string lfb = ".LFB"+to_string(compteurFonction);
	BasicBlock* bbepilogue = new BasicBlock(currentCFG, "");	
	BasicBlock* bbmain = new BasicBlock(currentCFG, lfb);
	compteurFonction++;
	
	bbmain->exit_true = bbepilogue;
	bbepilogue->exit_true = nullptr;

	currentCFG->add_bb(bbmain);

	currentCFG->current_bb = bbmain;
	
    visitChildren(context);

	currentCFG->add_bb(bbepilogue);

	switch(typeAssembleur){
		case x86: 
			currentCFG->gen_asm_x86(*sortie);
			break;

		case arm:
			currentCFG->gen_asm_arm(*sortie);
			break;

		case msp430:
			currentCFG->gen_asm_msp430(*sortie);
			break;

		default:
			currentCFG->gen_asm_x86(*sortie);
			break;
	}

	currentCFG->verifierAffectationSymboles();
	return 0;
}

antlrcpp::Any CFGVisitor::visitBloc(ifccParser::BlocContext *context) {
	//cout<< "#visitBloc" << endl;
	visit(context->instructionsList());
	return currentCFG->current_bb->label;
}

antlrcpp::Any CFGVisitor::visitInstruction(ifccParser::InstructionContext *context) {
	//cout<< "#visitInstr" << endl;
	if (context->RETURN() != 0) {
		auto val = visit(context->expr()).as<string>();
		Instr_select_return *instr = new Instr_select_return(currentCFG->current_bb, currentCFG->get_var_type(val), currentCFG->get_var_index(val));
		currentCFG->current_bb->add_IRInstr(instr);
	} else {	
		visitChildren(context);
	}
	return 0;
}


antlrcpp::Any CFGVisitor::visitDeclaration(ifccParser::DeclarationContext *context) 
{
	//cout<< "#visitDecla" << endl;

	string var;
	Type type = stringToType(context->TYPE()->getText());
	for (auto element : context->varPar()) {
		var = element->VAR()->getText();
		currentCFG->add_to_symbol_table(var, type);
		add_to_global_symbol_table(var, type);
	}
	return var;
}


antlrcpp::Any CFGVisitor::visitAffectationSimple(ifccParser::AffectationSimpleContext *context) {
	//cout<< "#AffectationSimple" << endl;
	auto val = visit(context->expr()).as<string>();
	auto dest = context->VAR()->getText();
	currentCFG->setVariableUtilisee(dest);
	Instr_copy *instr = new Instr_copy(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(val));
	currentCFG->current_bb->add_IRInstr(instr);
	return 0;
}


antlrcpp::Any CFGVisitor::visitAffectationDeclaration(ifccParser::AffectationDeclarationContext *context)  {
	//cout<< "#AffectationDeclaration" << endl;
	auto dest = visit(context->declaration()).as<string>();
	currentCFG->setVariableUtilisee(dest);
	auto val = visit(context->expr()).as<string>();
	Instr_copy *instr = new Instr_copy(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(val));
	currentCFG->current_bb->add_IRInstr(instr);
	return 0;
}

antlrcpp::Any CFGVisitor::visitAffectationSimpleFunction(ifccParser::AffectationSimpleFunctionContext *context) {
	//cout<< "#AffectationSimpleFunction" << endl;
	auto val = visit(context->functionCall()).as<string>();
	auto dest = context->VAR()->getText();
	currentCFG->setVariableUtilisee(dest);
	Instr_copy *instr = new Instr_copy(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(val));
	currentCFG->current_bb->add_IRInstr(instr);
	return 0;
}
antlrcpp::Any CFGVisitor::visitAffectationDeclarationFunction(ifccParser::AffectationDeclarationFunctionContext *context) {
	//cout<< "#AffectationDeclarationFunction" << endl;
	auto dest = visit(context->declaration()).as<string>();
	currentCFG->setVariableUtilisee(dest);
	auto val = visit(context->functionCall()).as<string>();
	Instr_copy *instr = new Instr_copy(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(val));
	currentCFG->current_bb->add_IRInstr(instr);
	return 0;
}

antlrcpp::Any CFGVisitor::visitIfStatement(ifccParser::IfStatementContext *context) {
	//cout<< "#IfStatement" << endl;
	string labelDeDebut = currentCFG->current_bb->label; 
	BasicBlock* blockDeFin = currentCFG->current_bb->exit_true;
	BasicBlock* blockIf = new BasicBlock(currentCFG, currentCFG->newBlockLabel());
	currentCFG->add_bb(blockIf);
	lastLabelBlockIf = blockIf->label;
	currentCFG->current_bb->exit_true = blockIf;
	currentCFG->current_bb = blockIf;
	currentCFG->current_bb->test_var_name = visit(context->expr()).as<string>();
	Instr_comp* compareWithOne = new Instr_comp(currentCFG->current_bb,"==", true, 0, currentCFG->get_var_index(currentCFG->current_bb->test_var_name), 0);
	currentCFG->current_bb->add_IRInstr(compareWithOne);
	auto blocs = context->bloc();
	if (blocs.size()==1){	//just if condition
		//cout<< "#JusteIf" << endl;
		string labelThen = currentCFG->newBlockLabel();
		string labelEndIf = currentCFG->newBlockLabel();
		Instr_jump* instr = new Instr_jump(currentCFG->current_bb, true, labelThen, labelEndIf);
		currentCFG->current_bb->add_IRInstr(instr);
		BasicBlock* blockThen = new BasicBlock(currentCFG, labelThen);
		BasicBlock* blockEndIf = new BasicBlock(currentCFG, labelEndIf);
		currentCFG->add_bb(blockThen);
		currentCFG->add_bb(blockEndIf);
		blockIf->exit_true = blockThen;
		blockIf->exit_false = blockEndIf;
		blockThen->exit_true = blockEndIf;
		blockEndIf->exit_true = blockDeFin;

		currentCFG->current_bb = blockThen;
		visit(context->bloc(0));
		if (lastLabelBlockIf.compare(blockIf->label)==0) {
			Instr_jump* instr1 = new Instr_jump(blockThen, false, labelEndIf, "");
			blockThen->add_IRInstr(instr1);
		} else {
			Instr_jump* instr1 = new Instr_jump(blockThen, false, lastLabelBlockIf, "");
			blockThen->add_IRInstr(instr1);
		}
		currentCFG->current_bb = blockEndIf;
		if (lastLabelBlockEndIf.compare(blockEndIf->label)!=0 && lastLabelBlockEndIf.compare("")!=0) {
			BasicBlock* block = currentCFG->findBlock(lastLabelBlockEndIf);
			Instr_jump* instr2 = new Instr_jump(block, false, blockEndIf->label, "");
			block->add_IRInstr(instr2);
		}
		lastLabelBlockEndIf = blockEndIf->label;

	} else {	//if and else condition
		//cout<< "#If&Else" << endl;
		string labelThen = currentCFG->newBlockLabel();
		string labelElse = currentCFG->newBlockLabel();
		string labelEndIf = currentCFG->newBlockLabel();
		Instr_jump* instr = new Instr_jump(currentCFG->current_bb, true, labelThen, labelElse);
		currentCFG->current_bb->add_IRInstr(instr);
		BasicBlock* blockThen = new BasicBlock(currentCFG, labelThen);
		BasicBlock* blockElse = new BasicBlock(currentCFG, labelElse);
		BasicBlock* blockEndIf = new BasicBlock(currentCFG, labelEndIf);
		currentCFG->add_bb(blockThen);
		currentCFG->add_bb(blockElse);
		currentCFG->add_bb(blockEndIf);
		blockIf->exit_true = blockThen;
		blockIf->exit_false = blockElse;
		blockThen->exit_true = blockEndIf;
		blockElse->exit_true = blockEndIf;
		blockEndIf->exit_true = blockDeFin;

		currentCFG->current_bb = blockThen;
		visit(context->bloc(0));
		if (lastLabelBlockIf.compare(blockIf->label)==0) {
			Instr_jump* instr1 = new Instr_jump(blockThen, false, labelEndIf, "");
			blockThen->add_IRInstr(instr1);
		} else {
			Instr_jump* instr1 = new Instr_jump(blockThen, false, lastLabelBlockIf, "");
			blockThen->add_IRInstr(instr1);
		}
		currentCFG->current_bb = blockElse;
		visit(context->bloc(1));
		if (lastLabelBlockIf.compare(blockIf->label)==0) {
			Instr_jump* instr2 = new Instr_jump(blockElse, false, labelEndIf, "");
			blockElse->add_IRInstr(instr2);
		} else {
			Instr_jump* instr2 = new Instr_jump(blockElse, false, lastLabelBlockIf, "");
			blockElse->add_IRInstr(instr2);
		}
		currentCFG->current_bb = blockEndIf;
		if (lastLabelBlockEndIf.compare(blockEndIf->label)!=0 && lastLabelBlockEndIf.compare("")!=0) {
			BasicBlock* block = currentCFG->findBlock(lastLabelBlockEndIf);
			Instr_jump* instr3 = new Instr_jump(block, false, blockEndIf->label, "");
			block->add_IRInstr(instr3);
		}
		lastLabelBlockEndIf = blockEndIf->label;
	}
	return 0;
}

antlrcpp::Any CFGVisitor::visitWhileStatement(ifccParser::WhileStatementContext *context) {
	//cout<< "#WhileStatement" << endl;
	string labelWhile = currentCFG->newBlockLabel();
	BasicBlock* blockDeFin = currentCFG->current_bb->exit_true;
	BasicBlock* blockWhile = new BasicBlock(currentCFG, labelWhile);
	currentCFG->add_bb(blockWhile);
	currentCFG->current_bb->exit_true = blockWhile;
	currentCFG->current_bb = blockWhile;
	currentCFG->current_bb->test_var_name = visit(context->expr()).as<string>();
	Instr_comp* compareWithOne = new Instr_comp(currentCFG->current_bb,"==", true, 0, currentCFG->get_var_index(currentCFG->current_bb->test_var_name), 0);
	currentCFG->current_bb->add_IRInstr(compareWithOne);
	auto bloc = context->bloc();

	string labelBody = currentCFG->newBlockLabel();
	string labelEndWhile = currentCFG->newBlockLabel();
	Instr_jump* instr = new Instr_jump(currentCFG->current_bb, true, labelBody, labelEndWhile);
	currentCFG->current_bb->add_IRInstr(instr);

	BasicBlock* blockBody = new BasicBlock(currentCFG, labelBody);
	BasicBlock* blockEndWhile = new BasicBlock(currentCFG, labelEndWhile);
	currentCFG->add_bb(blockBody);
	currentCFG->add_bb(blockEndWhile);
	blockWhile->exit_true = blockBody;
	blockWhile->exit_false = blockEndWhile;
	blockBody->exit_true = blockWhile;
	blockEndWhile->exit_true = blockDeFin;

	currentCFG->current_bb = blockBody;
	visit(context->bloc());

	Instr_jump* instr1 = new Instr_jump(currentCFG->current_bb, false, labelWhile, "");
	currentCFG->current_bb->add_IRInstr(instr1);

	currentCFG->current_bb = blockEndWhile;
	return 0;
}


antlrcpp::Any CFGVisitor::visitConst(ifccParser::ConstContext *context) {
	//cout<< "#visitConst" << endl;
	auto var = visit(context->constante()).as<string>();
	return var;
}


antlrcpp::Any CFGVisitor::visitVar(ifccParser::VarContext *context) {
	//cout<< "#visitVar" << endl;
	return context->VAR()->getText();
}


antlrcpp::Any CFGVisitor::visitProdop(ifccParser::ProdopContext *context) {
	//cout<< "#visitProdOP" << endl;
	char op = context->prodoper()->getText()[0];
	auto facteur1 = visit(context->expr(0)).as<string>();
	auto facteur2 = visit(context->expr(1)).as<string>();
	Type typeFacteur1 = currentCFG->get_var_type(facteur1);
	Type typeFacteur2 = currentCFG->get_var_type(facteur2);
	if (typeFacteur1 != typeFacteur2) {
		cout << "#WARNING : Diferent type operand" << endl;
	}
	string dest = currentCFG->create_new_tempvar(typeFacteur1);
	add_to_global_symbol_table(facteur1, typeFacteur1);

	if (op=='/')
	{
		Instr_div *instr = new Instr_div(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(facteur1), currentCFG->get_var_index(facteur2));
		currentCFG->current_bb->add_IRInstr(instr);
	} else
	{
		Instr_mul *instr = new Instr_mul(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(facteur1), currentCFG->get_var_index(facteur2));
		currentCFG->current_bb->add_IRInstr(instr);
	}
	return dest;
}


antlrcpp::Any CFGVisitor::visitSumop(ifccParser::SumopContext *context) {
	//cout<< "#visitSumOP" << endl;
	char op = context->sumoper()->getText()[0];
	auto terme1 = visit(context->expr(0)).as<string>();
	auto terme2 = visit(context->expr(1)).as<string>();
	Type typeTerme1 = currentCFG->get_var_type(terme1);
	Type typeTerme2 = currentCFG->get_var_type(terme2);
	if (typeTerme1 != typeTerme2) {
		cout << "#WARNING : Diferent type operand" << endl;
	}
	string dest = currentCFG->create_new_tempvar(typeTerme1);
	add_to_global_symbol_table(terme1, typeTerme1);


	if (op=='+')
	{
		Instr_add *instr = new Instr_add(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(terme1), currentCFG->get_var_index(terme2));
		currentCFG->current_bb->add_IRInstr(instr);
	} else
	{
		Instr_sub *instr = new Instr_sub(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(terme1), currentCFG->get_var_index(terme2));
		currentCFG->current_bb->add_IRInstr(instr);
	}
	return dest;
}

antlrcpp::Any CFGVisitor::visitCompop(ifccParser::CompopContext *context) {
	//cout<< "#visitCompOP" << endl;
	string op = context->compoper()->getText();
	auto terme1 = visit(context->expr(0)).as<string>();
	auto terme2 = visit(context->expr(1)).as<string>();
	Type typeTerme1 = currentCFG->get_var_type(terme1);
	Type typeTerme2 = currentCFG->get_var_type(terme2);
	if (typeTerme1 != typeTerme2) {
		cout << "#WARNING : Diferent type operand" << endl;
	}
	string dest = currentCFG->create_new_tempvar(typeTerme1);
	add_to_global_symbol_table(terme1, typeTerme1);

	Instr_comp *instr = new Instr_comp(currentCFG->current_bb, op, false, currentCFG->get_var_index(dest), currentCFG->get_var_index(terme1), currentCFG->get_var_index(terme2));
	currentCFG->current_bb->add_IRInstr(instr);
	return dest;
}


antlrcpp::Any CFGVisitor::visitNeg(ifccParser::NegContext *context){
	//cout<< "#visitNeg" << endl;
	auto val = visit(context->expr()).as<string>();
	Type typeVal = currentCFG->get_var_type(val);
	string reg_tmp = currentCFG->create_new_tempvar(typeVal);
	add_to_global_symbol_table(val, typeVal);
	Instr_opposite * instr = new Instr_opposite(currentCFG->current_bb,currentCFG->get_var_type(val),currentCFG->get_var_index(reg_tmp),currentCFG->get_var_index(val));
	currentCFG->current_bb->add_IRInstr(instr);
	return reg_tmp;
}


antlrcpp::Any CFGVisitor::visitNegConst(ifccParser::NegConstContext *context){
	auto var = visit(context->constante()).as<string>();
	Instr_opposite * instr = new Instr_opposite(currentCFG->current_bb,currentCFG->get_var_type(var),currentCFG->get_var_index(var), currentCFG->get_var_index(var));
	currentCFG->current_bb->add_IRInstr(instr);
	return var;
}

antlrcpp::Any CFGVisitor::visitFuncCall(ifccParser::FuncCallContext *context) {
	auto val = visit(context->functionCall()).as<string>();
	return val;
}

antlrcpp::Any CFGVisitor::visitFunctionCall(ifccParser::FunctionCallContext *context){
	string name = checkFonctionName(context->VAR()->getText());
	Type nameType = fonctions.find(name)->second;
	string reg_tmp = currentCFG->create_new_tempvar(nameType);
	add_to_global_symbol_table(name, nameType);

	Instr_call * instr = new Instr_call(currentCFG->current_bb, currentCFG->get_var_index(reg_tmp), name, trouverIndexDernierRegiste());
	currentCFG->current_bb->add_IRInstr(instr);
	return reg_tmp;
}


antlrcpp::Any CFGVisitor::visitNot(ifccParser::NotContext *context) {
	auto var = visit(context->expr()).as<string>();
	string reg_tmp = currentCFG->create_new_tempvar(_INT);
	add_to_global_symbol_table(var, _INT);
	string dest = currentCFG->create_new_tempvar(_INT);
	add_to_global_symbol_table(dest, _INT);
	Instr_ldconst * instr0 = new Instr_ldconst(currentCFG->current_bb,_INT,0,currentCFG->get_var_index(reg_tmp));
	Instr_comp * instr = new Instr_comp(currentCFG->current_bb,"==", false, currentCFG->get_var_index(dest), currentCFG->get_var_index(var), currentCFG->get_var_index(reg_tmp));
	currentCFG->current_bb->add_IRInstr(instr0);
	currentCFG->current_bb->add_IRInstr(instr);
	return dest;
} 


antlrcpp::Any CFGVisitor::visitPar(ifccParser::ParContext *context){
	//cout<< "#visitPar" << endl;
	auto val = visit(context->expr()).as<string>();
	return val;
}


antlrcpp::Any CFGVisitor::visitBinop(ifccParser::BinopContext *context){
	//cout<< "#visitBinop" << endl;
	char op = context->binoper()->getText()[0];
	auto val1 = visit(context->expr(0)).as<string>();
	auto val2 = visit(context->expr(1)).as<string>();
	Type typeVal1 = currentCFG->get_var_type(val1);
	Type typeVal2 = currentCFG->get_var_type(val2);
	if (typeVal1 != typeVal2) {
		cout << "#WARNING : Diferent type operand" << endl;
	}

	string dest = currentCFG->create_new_tempvar(typeVal1);
	add_to_global_symbol_table(dest, typeVal1);
	if (op=='&')
	{
		Instr_and_bin *instr = new Instr_and_bin(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(val1), currentCFG->get_var_index(val2));
		currentCFG->current_bb->add_IRInstr(instr);	
	} else if (op=='|')
	{
		Instr_or_bin *instr = new Instr_or_bin(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(val1), currentCFG->get_var_index(val2));
		currentCFG->current_bb->add_IRInstr(instr);	
	} else 
	{
		Instr_xor_bin *instr = new Instr_xor_bin(currentCFG->current_bb, currentCFG->get_var_type(dest), currentCFG->get_var_index(dest), currentCFG->get_var_index(val1), currentCFG->get_var_index(val2));
		currentCFG->current_bb->add_IRInstr(instr);	
	}
	return dest;
}


antlrcpp::Any CFGVisitor::visitChar(ifccParser::CharContext *context){
	//cout<< "#visitChar" << endl;
	string reg_tmp = currentCFG->create_new_tempvar(_CHAR);
	add_to_global_symbol_table(reg_tmp, _CHAR);
	Instr_ldconst *instr = new Instr_ldconst(currentCFG->current_bb, _CHAR, context->CHAR()->getText()[1], currentCFG->get_var_index(reg_tmp));
	currentCFG->current_bb->add_IRInstr(instr);
	return reg_tmp;
}


antlrcpp::Any CFGVisitor::visitNum(ifccParser::NumContext *context){
	string reg_tmp = currentCFG->create_new_tempvar(_INT);
	add_to_global_symbol_table(reg_tmp, _INT);
	Instr_ldconst *instr = new Instr_ldconst(currentCFG->current_bb,_INT,stoi(context->NUMBER()->getText()),currentCFG->get_var_index(reg_tmp));
	currentCFG->current_bb->add_IRInstr(instr);
	return reg_tmp;
}

void CFGVisitor::add_to_global_symbol_table(string name, Type t) {
	if (SymbolIndexGlobal.find(name) == SymbolIndexGlobal.end())
    {
        switch (t){
            case _INT:
                lastIndexGlobal += 4; // +4 pour type int
                break;
            case _CHAR:
                lastIndexGlobal += 1;
			case _DOUBLE:
                lastIndexGlobal += 8;
            break;
        }
        SymbolIndexGlobal[name] = lastIndexGlobal;
        SymbolTypeGlobal[name] = t;
    }
    // else
    // {
    //     throw runtime_error("Variable déjà définie dans le programme");
    // }
}

string CFGVisitor::checkFonctionName(string name) {
    if (fonctions.find(name) != fonctions.end())
    {

		return name;
    }
    else
    {
        throw runtime_error("Fonction non déclarée");
    }
}

int CFGVisitor::trouverIndexDernierRegiste(){
	int maxIndex = 0;
	for (const auto& var : SymbolIndexGlobal) {
		if (var.second > maxIndex) maxIndex = var.second;
    }
	return maxIndex;
 
}



