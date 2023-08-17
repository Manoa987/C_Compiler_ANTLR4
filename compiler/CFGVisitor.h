#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
using namespace std;
#include <map>
#include <iostream>
#include <any>
#include "BasicBlock.h"
#include "Types.h"
#include "IRInstr.h"
#include "Instr_add.h"
#include "Instr_copy.h"
#include "Instr_div.h"
#include "Instr_ldconst.h"
#include "Instr_mul.h"
#include "Instr_sub.h"
#include "Instr_opposite.h"
#include "Instr_and_bin.h"
#include "Instr_or_bin.h"
#include "Instr_xor_bin.h"
#include "Instr_select_return.h"
#include "Instr_comp.h"
#include "Instr_jump.h"
#include "CFG.h"
#include "Instr_call.h"



class CFGVisitor : public ifccBaseVisitor
{
public:
	CFGVisitor(TypeAssembleur typeAsm, ostream &o) : typeAssembleur(typeAsm), sortie(&o){};
	
	void add_to_global_symbol_table(string name, Type t);
	string checkFonctionName(string name);
	int trouverIndexDernierRegiste();

	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *context) override;
	virtual antlrcpp::Any visitFunctionList(ifccParser::FunctionListContext *context) override;
	virtual antlrcpp::Any visitFunction(ifccParser::FunctionContext *context) override;
    virtual antlrcpp::Any visitBloc(ifccParser::BlocContext *context) override;
	virtual antlrcpp::Any visitInstruction(ifccParser::InstructionContext *context) override;
	virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *context) override;
	virtual antlrcpp::Any visitAffectationSimple(ifccParser::AffectationSimpleContext *context) override;
	virtual antlrcpp::Any visitAffectationDeclaration(ifccParser::AffectationDeclarationContext *context) override;
	virtual antlrcpp::Any visitAffectationSimpleFunction(ifccParser::AffectationSimpleFunctionContext *context) override;
    virtual antlrcpp::Any visitAffectationDeclarationFunction(ifccParser::AffectationDeclarationFunctionContext *context) override;
	virtual antlrcpp::Any visitIfStatement(ifccParser::IfStatementContext *context) override;
    virtual antlrcpp::Any visitWhileStatement(ifccParser::WhileStatementContext *context) override;
	virtual antlrcpp::Any visitConst(ifccParser::ConstContext *context) override;
	virtual antlrcpp::Any visitVar(ifccParser::VarContext *context) override;
	virtual antlrcpp::Any visitProdop(ifccParser::ProdopContext *context) override;
	virtual antlrcpp::Any visitSumop(ifccParser::SumopContext *context) override;
	virtual antlrcpp::Any visitCompop(ifccParser::CompopContext *context) override;
	virtual antlrcpp::Any visitNeg(ifccParser::NegContext *context) override;
	virtual antlrcpp::Any visitNegConst(ifccParser::NegConstContext *context) override;
    virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *context) override;
    virtual antlrcpp::Any visitFunctionCall(ifccParser::FunctionCallContext *context) override;
	virtual antlrcpp::Any visitNot(ifccParser::NotContext *context) override;
	virtual antlrcpp::Any visitPar(ifccParser::ParContext *context) override;
	virtual antlrcpp::Any visitBinop(ifccParser::BinopContext *context) override;
	virtual antlrcpp::Any visitNum(ifccParser::NumContext *context) override;
	virtual antlrcpp::Any visitChar(ifccParser::CharContext *context) override;
	// virtual antlrcpp::Any visitParametersList(ifccParser::ParametersListContext *context) override;
	// virtual antlrcpp::Any visitParameter(ifccParser::ParameterContext *context) override;
	// virtual antlrcpp::Any visitArgumentsList(ifccParser::ArgumentsListContext *context) override;


protected:
	int lastIndexGlobal = 0;
	string lastLabelBlockIf, lastLabelBlockEndIf = "";
	ostream* sortie;
	vector<CFG*> cfgs;
	map<string, Type> fonctions;
	TypeAssembleur typeAssembleur;
	CFG* currentCFG;
	int compteurFonction = 0;
	map <string, int> SymbolIndexGlobal;
	map <string, Type> SymbolTypeGlobal;

};
