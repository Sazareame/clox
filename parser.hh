#pragma once

#include "stdafx.hh"
#include "token.hh"
#include "statement.hh"

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

	StmtPtr statement(){
		if(is_match(TokenType::PRINT))
			return print_stmt();
		return expr_stmt();
	}

	StmtPtr declaration();

	StmtPtr var_decl(){
		auto& name = consume(TokenType::IDENTIFIER, "Expect variable name.");
		ExprPtr initializer = nullptr;
		if(is_match(TokenType::EQUAL))
			initializer = expression();
		consume(TokenType::SEMICOLON, "Expect `;` after variable declaration.");
		return std::shared_ptr<Stmt>(new Var(name, initializer));
	}

	StmtPtr print_stmt(){
		auto value = expression();
		consume(TokenType::SEMICOLON, "Expect `;` after value.");
		return std::shared_ptr<Stmt>(new PrintStmt(value));
	}

	StmtPtr expr_stmt(){
		auto value = expression();
		consume(TokenType::SEMICOLON, "Expect `;` after expression.");
		return std::shared_ptr<Stmt>(new ExprStmt(value));
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

	Token const& consume(TokenType type, std::string_view msg);
	std::string error(Token const& token, std::string_view msg)const;

	void synchronize(){
		advance();
		while(!is_at_end()){
			if(previous().type == TokenType::SEMICOLON) return;
			switch(peek().type){
				case TokenType::CLASS:
				case TokenType::FUN:
				case TokenType::VAR:
				case TokenType::FOR:
				case TokenType::IF:
				case TokenType::WHILE:
				case TokenType::PRINT:
				case TokenType::RETURN: return;
				default: break;
			}
			advance();
		}
	}

public:
	Parser(std::vector<Token> _tokens){
		tokens.swap(_tokens);
	}
	
	std::vector<StmtPtr> parse(){
		std::vector<StmtPtr> stmts;
		while(!is_at_end())
			stmts.push_back(declaration());
		return stmts;
	}
};
