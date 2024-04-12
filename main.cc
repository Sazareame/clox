#include "lox.hh"

bool Lox::had_error = false;

signed main(int argc, char const** argv){
  if(argc == 1) Lox::run_prompt();
  else if(argc == 2) Lox::run_file(argv[1]);
  else{
    puts("invalid input args.");
    exit(1);
  }
  return 0;
}