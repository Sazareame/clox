#include "statement.hh"
#include "environment.hh"

inline std::string op_error(Token const& op, std::string_view msg){
	std::stringstream ss;
	ss << "line " << op.line << ": Operands of " << op.lexeme << " " << msg;
	return ss.str();
}

inline bool check_addtion(Token const& op, Object const& lhs, Object const& rhs){
	if(lhs.has_type<double>() && rhs.has_type<double>()) return true;
	if(lhs.has_type<std::string>() && rhs.has_type<std::string>()) return false;
	throw op_error(op, "must be number or string.");
}

inline void check_operands(Token const& op, Object const& lhs, Object const& rhs){
	if(lhs.has_type<double>() && rhs.has_type<double>()) return;
	throw op_error(op, "must be number.");
}

inline void check_operand(Token const& op, Object const& hs){
	if(hs.has_type<double>()) return;
	throw op_error(op, "must be number.");
}

Object Binary::evaluate(){
	auto lhs = left->evaluate();
	auto rhs = right->evaluate();
	if(oper.type == TokenType::PLUS){
		if(check_addtion(oper, lhs, rhs))
			return Object(lhs.number() + rhs.number());
		else
			return Object(lhs.string() + rhs.string());
	}
	check_operands(oper, lhs, rhs);
	switch(oper.type){
		case TokenType::MINUS: return Object(lhs.number() - rhs.number()); break;
		case TokenType::SLASH: return Object(lhs.number() / rhs.number()); break;
		case TokenType::STAR: return Object(lhs.number() * rhs.number()); break;
		case TokenType::GREATER: return Object(lhs.number() > rhs.number()); break;
		case TokenType::GREATER_EQUAL: return Object(lhs.number() >= rhs.number()); break;
		case TokenType::LESS: return Object(lhs.number() < rhs.number()); break;
		case TokenType::LESS_EQUAL: return Object(lhs.number() <= rhs.number()); break;
		case TokenType::EQUAL_EQUAL: return Object(lhs.number() == rhs.number()); break;
		case TokenType::BANG_EQUAL: return Object(lhs.number() != rhs.number()); break;
	}
}

Object Literal::evaluate(){
	return lexeme;
}

Object Grouping::evaluate(){
	return expression->evaluate();
}

Object Unary::evaluate(){
	auto rhs = right->evaluate();
	if(oper.type == TokenType::MINUS){
		check_operand(oper, rhs);
		return Object(-rhs.number());
	}
	if(oper.type == TokenType::BANG){
		return Object(rhs.logic_not());
	}
}

Object Variable::evaluate(){
	return Environ::get(name);
}

void Var::execute(){
	Object value = Object();
	if(expr) value = expr->evaluate();
	Environ::define(name.lexeme, value);
}
