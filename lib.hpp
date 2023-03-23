#pragma once
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// 1.<S>::=try{<stm1>}catch(E str){<stm1>}
// 2.<stm1>:: = throw new E(<val>);<stm1>
// 3.<stm1>:: = print(<val>); <stm1>
// 4.<stm1>:: = epsilon
// 5. <val> :: = i
// 6. <val> :: = str

// one-symbol tokens
#define SEMICOLON 256       // ;
#define O_ROUND_BRACKET 257 // (
#define C_ROUND_BRACKET 258 // )
#define O_CURLY_BRACKET 259 // {
#define C_CURLY_BRACKET 260 // }
#define QUOTE 261           // '

// SPECIAL WORDS
#define TRY 271   // try
#define CATCH 272 // catch
#define THROW 273 // throw
#define NEW 274   // new
#define EXC 275   // E
#define PRINT 276 // PRINT
// #define E 267     // E

// lexems
#define INT 280
#define STR 281

int scan();

////////// IS BLOCK
bool is_any_separator(const char &t);
bool is_one_symbol_token(const char &t);
bool is_special_word(const std::string &);

bool is_str(const std::string &);
bool is_try(const std::string &);
bool is_catch(const std::string &str);
bool is_throw(const std::string &str);
bool is_new(const std::string &str);
bool is_exc(const std::string &str);
bool is_print(const std::string &str);

/////// RETURN BLOCK
int return_one_symbol_token(const char &t);
int return_special_word(const char &t);

int test_test();

/////////////////////////////////////////////////////// GRAMMAR FUNCTIONS

using val_type = std::variant<int, std::string>;

struct stm1_result {
  enum stm1_type { THROW_2, PRINT_3 } type;
  val_type val;
  stm1_result *child;
};

struct S_result {
  stm1_result *try_block;
  stm1_result *catch_block;
  std::string catch_if_block;
};

val_type val();
stm1_result *stm1();
S_result s();

void exec(S_result, std::string &);
