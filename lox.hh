#pragma once

#include "stdafx.hh"
#include "scanner.hh"

class Lox{
public:
  static bool had_error;

  static void run_file(char const* path){
    auto f = std::ifstream(path);
    if(!f.is_open()){
      puts("error during open source file.");
      exit(1);
    }
    std::stringstream ss;
    ss << f.rdbuf();
    run(ss.str());
  }

  static void run_prompt(){
    std::string line;
    std::cout << "> ";
    while(std::getline(std::cin, line)){
      run(line);
      had_error = false;
      std::cout << "\n> ";
    }
  }

private:
  static void run(std::string_view source){
		Scanner scanner(source);
		for(auto& token: scanner.scan_tokens()){
			std::cout << token << "\n";
		}
  }

public:
	static void report(std::string_view msg){
		had_error = true;
		std::cout << msg;
	}

};