#pragma once

#include "stdafx.hh"

enum class TokenType{
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  IDENTIFIER, STRING, NUMBER,

  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  EEOF
};

static const char* enum_table[39] = {
	"LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
	"COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",

	"BANG", "BANG_EQUAL",
	"EQUAL", "EQUAL_EQUAL",
	"GREATER", "GREATER_EQUAL",
	"LESS", "LESS_EQUAL",

	"IDENTIFIER", "STRING", "NUMBER",

	"AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
	"PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",

	"EOF"
};

class Object{
	std::optional<std::variant<double, std::string, bool, int> > data;

public:
	Object(): data(std::nullopt){}

	template<class T>
	Object(T _data): data(_data){}

	Object(std::string_view _data){
		data = std::string(_data);
	}

	friend std::ostream& operator<<(std::ostream& os, Object const& self){
		if(self.data.has_value()){
			auto visitor = [&os](const auto& t){os << t; };
			std::visit(visitor, self.data.value());
		} else os << "Nil";
		return os;
	}

};

class Token{
	TokenType type;
	std::string lexeme;
	Object literal;
	size_t line;

public:
	Token(TokenType _type, std::string_view _lexeme, Object _literal, size_t _line):
	type(_type), lexeme(_lexeme), literal(_literal), line(_line){}

	friend std::ostream& operator<<(std::ostream& os, Token const& self){
		os << enum_table[static_cast<int>(self.type)] << " " << self.lexeme << " " << self.literal << " " << self.line;
		return os;
	}
};

static const std::unordered_map<std::string, TokenType> key_words{
	{"and",    TokenType::AND},
	{"class",  TokenType::CLASS},
	{"else",   TokenType::ELSE},
	{"false",  TokenType::FALSE},
	{"for",    TokenType::FOR},
	{"fun",    TokenType::FUN},
	{"if",     TokenType::IF},
	{"nil",    TokenType::NIL},
	{"or",     TokenType::OR},
	{"print",  TokenType::PRINT},
	{"return", TokenType::RETURN},
	{"super",  TokenType::SUPER},
	{"this",   TokenType::THIS},
	{"true",   TokenType::TRUE},
	{"var",    TokenType::VAR},
	{"while",  TokenType::WHILE},
};