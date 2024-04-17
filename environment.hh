#pragma once

#include "stdafx.hh"
#include "statement.hh"

class Environ{
public:
	static std::unordered_map<std::string, Object> values;

	static void define(std::string const& name, Object const& value){
		values.insert({name, value});
	}

	static Object const& get(TokenPtr name){
		if(auto res = values.find(name->lexeme); res != values.end())
			return res->second;
		std::stringstream ss;
		ss << "Undefined variable " << name->lexeme << ".";
		throw ss.str();
	}

	static void assign(TokenPtr name, Object const& value){
		if(auto res = values.find(name->lexeme); res != values.end()){
			values[name->lexeme] = value;
			return;
		}
		std::stringstream ss;
		ss << "Undefined variable " << name->lexeme << ".";
		throw ss.str();
	}
};