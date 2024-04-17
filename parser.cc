#include "parser.hh"
#include "lox.hh"

StmtPtr Parser::declaration(){
	StmtPtr res = nullptr;
	try{
		if(is_match(TokenType::VAR))
			res = var_decl();
		else
			res = statement();
	} catch(std::string e){
		std::cout << e << std::endl;
		synchronize();
		Lox::had_error = true;
		return nullptr;
	}
	return res;
}

ExprPtr Parser::assignment(){
	auto expr = equlity();
	if(is_match(TokenType::EQUAL)){
		auto equals = previous();
		auto value = assignment();
		if(auto var = std::dynamic_pointer_cast<Variable>(expr)){
			auto name = var->get_name();
			return std::shared_ptr<Expr>(new Assign(name, value));
		}
		throw error(equals, "Invalid assignment target.");
	}
	return expr;
}

ExprPtr Parser::equlity(){
	auto expr = comparision();
	ExprPtr right = nullptr;
	while(is_match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)){
		auto operat = previous();
		right = comparision();
		expr = std::shared_ptr<Expr>(new Binary(expr, operat, right));
	}
	return expr;
}

ExprPtr Parser::comparision(){
	auto expr = term();
	ExprPtr right = nullptr;
	while(is_match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)){
		auto operat = previous();
		right = term();
		expr = std::shared_ptr<Expr>(new Binary(expr, operat, right));
	}
	return expr;
}

ExprPtr Parser::term(){
	auto expr = factor();
	ExprPtr right = nullptr;
	while(is_match(TokenType::MINUS, TokenType::PLUS)){
		auto operat = previous();
		right = factor();
		expr = std::shared_ptr<Expr>(new Binary(expr, operat, right));
	}
	return expr;
}

ExprPtr Parser::factor(){
	auto expr = unary();
	ExprPtr right = nullptr;
	while(is_match(TokenType::SLASH, TokenType::STAR)){
		auto operat = previous();
		right = unary();
		expr = std::shared_ptr<Expr>(new Binary(expr, operat, right));
	}
	return expr;
}

ExprPtr Parser::unary(){
	if(is_match(TokenType::BANG, TokenType::MINUS)){
		auto operat = previous();
		auto right = unary();
		return std::shared_ptr<Expr>(new Unary(operat, right));
	}
	return primary();
}

ExprPtr Parser::primary(){
	if(is_match(TokenType::FALSE))
		return std::shared_ptr<Expr>(new Literal(false));
	if(is_match(TokenType::TRUE))
		return std::shared_ptr<Expr>(new Literal(true));
	if(is_match(TokenType::NIL))
		return std::shared_ptr<Expr>(new Literal());
	if(is_match(TokenType::STRING, TokenType::NUMBER))
		return std::shared_ptr<Expr>(new Literal(previous()->literal));
	if(is_match(TokenType::LEFT_PAREN)){
		auto expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
		return std::shared_ptr<Expr>(new Grouping(expr));
	}
	if(is_match(TokenType::IDENTIFIER))
		return std::shared_ptr<Expr>(new Variable(previous()));
	throw error(peek(), "Expect expression.");
}

TokenPtr Parser::consume(TokenType type, std::string_view msg){
	if(check(type))
		return advance();
	else
		throw error(peek(), msg);
}

std::string Parser::error(TokenPtr token, std::string_view msg)const{
	std::stringstream ss;
	if(token->type == TokenType::EEOF)
		ss << "line " << token->line << " at the end: " << msg;
	else
		ss << "line " << token->line << " at " << token->lexeme << ": " << msg;
	return ss.str();
}