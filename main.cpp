#include "lib.hpp"
#include <string.h>

extern int symbol;
extern int value;
extern FILE *current_stream;
extern std::string str;

void write_string_to_stdin(const std::string &str) {
  current_stream = fmemopen((void *)str.c_str(), str.size(), "r+");
}

int main() {
  std::string input = "try {print(1);print('y');throw new E "
                      "('exc');print('This will not be printed');} "
                      "catch(E exc){print ('x'); print(1);}";
  write_string_to_stdin(input);
  std::cout << input << "\n\n";

  symbol = scan();
  auto result_tree = s();
  std::string result_output;
  exec(result_tree, result_output);

  if (!(symbol == EOF || input.size() == ftell(current_stream)))
    throw std::runtime_error("Поток не дочитан до конца");

  printf("\n");
  return 0;
}
