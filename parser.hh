#pragma once

#include "stdafx.hh"
#include "token.hh"
#include "expression.h"

class Parser{
	std::vector<Token> tokens;
	size_t current{0};

	Token const& peek()const{
		return tokens[current];
	}

	bool is_at_end()const{
		return peek().type == TokenType::EEOF;
	}

	Token const& previous()const{
		return tokens[current - 1];
	}

	template<class... Types>
	bool is_match(TokenType type, Types... types){
		if(check(type)){
			advance();
			return true;
		}
		return is_match(types...);
	}

	template<>
	bool is_match(TokenType type){
		if(check(type)){
			advance();
			return true;
		}
		return false;
	}

	bool check(TokenType type)const{
		if(is_at_end()) return false;
		return peek().type == type;
	}

	Token const& advance(){
		if(!is_at_end()) ++current;
		return previous();
	}

	ExprPtr expression(){
		return equlity();
	}

	ExprPtr equlity();
	ExprPtr comparision();
	ExprPtr term();
	ExprPtr factor();
	ExprPtr unary();
	ExprPtr primary();

	void consume(TokenType type, std::string_view msg);
	std::string error(Token const& token, std::string_view msg)const;

public:
	Parser(std::vector<Token> _tokens){
		tokens.swap(_tokens);
	}
	
	ExprPtr parse(){
		return expression();
	}
};
