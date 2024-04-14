#include "parser.hh"
#include "lox.hh"

ExprPtr Parser::equlity(){
	auto expr = comparision();
	ExprPtr right = nullptr;
	while(is_match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)){
		auto& operat = previous();
		right = comparision();
		expr = std::shared_ptr<Expr>(new Binary(expr, operat, right));
	}
	return expr;
}

ExprPtr Parser::comparision(){
	auto expr = term();
	ExprPtr right = nullptr;
	while(is_match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)){
		auto& operat = previous();
		right = term();
		expr = std::shared_ptr<Expr>(new Binary(expr, operat, right));
	}
	return expr;
}

ExprPtr Parser::term(){
	auto expr = factor();
	ExprPtr right = nullptr;
	while(is_match(TokenType::MINUS, TokenType::PLUS)){
		auto& operat = previous();
		right = factor();
		expr = std::shared_ptr<Expr>(new Binary(expr, operat, right));
	}
	return expr;
}

ExprPtr Parser::factor(){
	auto expr = unary();
	ExprPtr right = nullptr;
	while(is_match(TokenType::SLASH, TokenType::STAR)){
		auto& operat = previous();
		right = unary();
		expr = std::shared_ptr<Expr>(new Binary(expr, operat, right));
	}
	return expr;
}

ExprPtr Parser::unary(){
	if(is_match(TokenType::BANG, TokenType::MINUS)){
		auto& operat = previous();
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
		return std::shared_ptr<Expr>(new Literal(previous().literal));
	if(is_match(TokenType::LEFT_PAREN)){
		auto expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
		return std::shared_ptr<Expr>(new Grouping(expr));
	}
	throw error(peek(), "Expect expression.");
}

void Parser::consume(TokenType type, std::string_view msg){
	if(check(type))
		advance();
	else
		throw error(peek(), msg);
}

std::string Parser::error(Token const& token, std::string_view msg)const{
	std::stringstream ss;
	if(token.type == TokenType::EEOF)
		ss << "line " << token.line << " at the end: " << msg;
	else
		ss << "line " << token.line << " at " << token.lexeme << ": " << msg;
	return ss.str();
}