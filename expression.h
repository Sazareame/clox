#pragma once

#include "stdafx.hh"
#include "token.hh"

class Expr{
public:
	virtual std::string ast_print() = 0;
	virtual ~Expr() = default;
};

typedef std::shared_ptr<Expr> ExprPtr;

std::string parenthesis(std::string const& name, std::vector<ExprPtr> const& exprs);

class Binary: public Expr{
	ExprPtr left;
	Token oper;
	ExprPtr right;

public:
	Binary(ExprPtr _left, Token const& _oper, ExprPtr _right):
		left(_left), oper(_oper), right(_right){}

	std::string ast_print() override{
		return parenthesis(oper.lexeme, std::vector<ExprPtr>{left, right});
	}
};

class Grouping: public Expr{
	ExprPtr expression;

public:
	Grouping(ExprPtr _expression): expression(_expression){}

	std::string ast_print() override{
		return parenthesis("group", std::vector<ExprPtr>{expression});
	}
};

class Literal: public Expr{
	Object lexeme;

public:
	Literal(){
		lexeme = Object();
	}

	template<class T>
	Literal(T _lexeme_type){
		lexeme = Object(_lexeme_type);
	}

	std::string ast_print() override{
		return lexeme.to_stringstream().str();
	}
};

class Unary: public Expr{
	Token oper;
	ExprPtr right;

public:
	Unary(Token const& _oper, ExprPtr _right):
		oper(_oper), right(_right){}

	std::string ast_print() override{
		return parenthesis(oper.lexeme, std::vector<ExprPtr>{right});
	}
};

inline std::string parenthesis(std::string const& name, std::vector<ExprPtr> const& exprs){
	std::string s = "(";
	s.append(name);
	for(const auto& expr : exprs){
		s.push_back(' ');
		s.append(expr->ast_print());
	}
	s.push_back(')');
	return s;
}

