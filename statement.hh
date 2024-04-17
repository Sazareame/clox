#pragma once

#include "stdafx.hh"
#include "expression.h"

class Stmt{
public:
	virtual void execute() = 0;
	virtual ~Stmt() = default;
};

typedef std::shared_ptr<Stmt> StmtPtr;

class PrintStmt: public Stmt{
	ExprPtr expr;

public:
	PrintStmt(ExprPtr _expr): expr(_expr){}
	void execute()override{
		auto value = expr->evaluate();
		std::cout << value.to_stringstream().str() << std::endl;
	}
};

class ExprStmt: public Stmt{
	ExprPtr expr;

public:
	ExprStmt(ExprPtr _expr): expr(_expr){}
	void execute()override{
		expr->evaluate();
	}
};

class Var: public Stmt{
	TokenPtr name;
	ExprPtr expr;

public:
	Var(TokenPtr _name, ExprPtr _expr): name(_name), expr(_expr){}
	void execute()override;
};

inline void interpreter(std::vector<StmtPtr> const& stmts){
	for(auto const& stmt : stmts)
		stmt->execute();
}
