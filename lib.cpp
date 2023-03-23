#include "lib.hpp"
#include <algorithm>

int symbol = 0;
int t;
int value;
FILE *current_stream;
std::string str;

int test_test() { return 0; }

/////////////////////////////////////////////////////// SCANER FUNCTIONS

////////// IS BLOCK
bool is_any_separator(const char &t) {
  return (t == ' ' || t == '\t' || t == '\n' || t == '\r' || t == EOF);
}
bool is_one_symbol_token(const char &t) {
  return (t == ';' || t == '(' || t == ')' || t == '{' || t == '}');
}
bool is_try(const std::string &str) {
  return std::string("try").compare(str) == 0;
}
bool is_catch(const std::string &str) {
  return std::string("catch").compare(str) == 0;
}
bool is_throw(const std::string &str) {
  return std::string("throw").compare(str) == 0;
}
bool is_new(const std::string &str) {
  return std::string("new").compare(str) == 0;
}
bool is_exc(const std::string &str) {
  return std::string("E").compare(str) == 0;
}
bool is_print(const std::string &str) {
  return std::string("print").compare(str) == 0;
}

bool is_special_word(const std::string &str) {
  return (is_try(str) || is_catch(str) || is_throw(str) || is_new(str) ||
          is_exc(str) || is_print(str));
}

bool is_str(const std::string &str) {
  return *str.begin() == '\'' && str.back() == '\'';
}

/////// RETURN BLOCK
int return_one_symbol_token(const char &t) {
  switch (t) {
  case ';':
    return SEMICOLON;
  case '(':
    return O_ROUND_BRACKET;
  case ')':
    return C_ROUND_BRACKET;
  case '{':
    return O_CURLY_BRACKET;
  case '}':
    return C_CURLY_BRACKET;
  }
  throw std::runtime_error("Internal error in return_one_symbol_token");
}

int return_special_word(const std::string &str) {
  if (is_try(str))
    return TRY;
  if (is_catch(str))
    return CATCH;
  if (is_throw(str))
    return THROW;
  if (is_new(str))
    return NEW;
  if (is_exc(str))
    return EXC;
  if (is_print(str))
    return PRINT;
  throw std::runtime_error("Internal error in return_special_word");
}

int scan() {
  str.clear();
  while (1) {
    t = getc(current_stream);
    if (is_any_separator(t)) {
      if (t == EOF)
        return EOF;
    } else if (is_one_symbol_token(t)) {
      return return_one_symbol_token(t);
    } else if (t >= '0' && t <= '9') {
      value = t - '0';
      t = getc(current_stream);
      while (t >= '0' && t <= '9') {
        value = value * 10 + t - '0';
        t = getc(current_stream);
      }
      if (is_one_symbol_token(t))
        ungetc(t, current_stream);
      return INT;
    } else {
      str.push_back(t);
      while (1) {
        if (is_special_word(str)) {
          return return_special_word(str);
        }
        if (t == '\'') {
          while (!(t == EOF)) {
            t = getc(current_stream);
            str.push_back(t);
            if (is_str(str)) {
              str.erase(std::remove(str.begin(), str.end(), '\''),
                        str.end()); // remove ' from string
              return STR;
            }
          }
          ungetc(t, current_stream);
          throw std::runtime_error("WE SHOULD NOT BE HERE!-----------\n" + str +
                                   "\n");
        }
        t = getc(current_stream);
        if (is_one_symbol_token(t) || is_any_separator(t)) {
          if (is_one_symbol_token(t)) {
            ungetc(t, current_stream);
          }
          break;
        }
        str.push_back(t);
      }
      return STR;
    }
  }
  // impossible position
  throw std::runtime_error("Unknown token!-----------\n" + str + "\n");
}

/////////////////////////////////////////////////////// SCANER FUNCTIONS END

/////////////////////////////////////////////////////// GRAMMAR FUNCTIONS
// 1.<S>::=try{<stm1>}catch(E str){<stm1>}
// 2.<stm1>:: = throw new E(<val>);<stm1>
// 3.<stm1>:: = print(<val>); <stm1>
// 4.<stm1>:: = epsilon
// 5. <val> :: = i
// 6. <val> :: = str

S_result s() {
  // 1.<S>::=try{<stm1>}catch(E str){<stm1>}
  S_result result;

  if (symbol != TRY)
    throw std::runtime_error("Expected TRY");

  symbol = scan();
  if (symbol != O_CURLY_BRACKET)
    throw std::runtime_error("Expected {");

  symbol = scan();
  result.try_block = stm1();

  // symbol = scan();
  if (symbol != C_CURLY_BRACKET)
    throw std::runtime_error("Expected } in try");

  symbol = scan();
  if (symbol != CATCH)
    throw std::runtime_error("Expected catch");

  symbol = scan();
  if (symbol != O_ROUND_BRACKET)
    throw std::runtime_error("Expected (");

  symbol = scan();
  if (symbol != EXC)
    throw std::runtime_error("Expected E");

  symbol = scan();
  if (symbol != STR)
    throw std::runtime_error("Expected STR");
  result.catch_if_block = str;

  symbol = scan();
  if (symbol != C_ROUND_BRACKET)
    throw std::runtime_error("Expected )");

  symbol = scan();
  if (symbol != O_CURLY_BRACKET)
    throw std::runtime_error("Expected {");

  symbol = scan();
  result.catch_block = stm1();

  // symbol = scan();
  if (symbol != C_CURLY_BRACKET)
    throw std::runtime_error("Expected } in catch");

  return result;
  // 1.<S>::=try{<stm1>}catch(E str){<stm1>}
}

stm1_result *stm1() {
  // 2.<stm1>:: = throw new E(<val>);<stm1> // 1
  // 3.<stm1>:: = print(<val>); <stm1> // 2
  // 4.<stm1>:: = epsilon // 3
  stm1_result *result = new stm1_result();
  result->child = nullptr;
  if (symbol == THROW) {
    // 2.<stm1>:: = throw new E(<val>);<stm1> // 1
    result->type = stm1_result::THROW_2;
    symbol = scan();
    if (symbol != NEW)
      throw std::runtime_error("Expected NEW");

    symbol = scan();
    if (symbol != EXC)
      throw std::runtime_error("Expected E");

    symbol = scan();
    if (symbol != O_ROUND_BRACKET)
      throw std::runtime_error("Expected (");

    symbol = scan();
    result->val = val();

    symbol = scan();
    if (symbol != C_ROUND_BRACKET)
      throw std::runtime_error("Expected )");

    symbol = scan();
    if (symbol != SEMICOLON)
      throw std::runtime_error("Expected ;");

    symbol = scan();
    result->child = stm1();
    return result;

  } else if (symbol == PRINT) {
    // 3.<stm1>:: = print(<val>); <stm1> // 2
    result->type = stm1_result::PRINT_3;

    symbol = scan();
    if (symbol != O_ROUND_BRACKET)
      throw std::runtime_error("Expected (");

    symbol = scan();
    result->val = val();

    symbol = scan();
    if (symbol != C_ROUND_BRACKET)
      throw std::runtime_error("Expected )");

    symbol = scan();
    if (symbol != SEMICOLON)
      throw std::runtime_error("Expected ;");

    symbol = scan();
    result->child = stm1();
    return result;
  }
  return nullptr;
  throw std::runtime_error("Unknown symbol in stm1_result");
}

val_type val() {
  // 5. <val> :: = i
  // 6. <val> :: = str
  if (symbol == STR)
    return str;
  if (symbol == INT)
    return value;

  throw std::runtime_error("Unknown symbol in variant");
}
/////////////////////////////////////////////////////// GRAMMAR FUNCTIONS END

/////////////////////////////////////////////////////// SEMANTHICS FUNCTIONS
void exec(S_result grammar_tree, std::string &output_str) {
  auto try_block = grammar_tree.try_block;
  bool thrw = false;
  while (true) {
    if (try_block == nullptr)
      break;
    else if (try_block->type == stm1_result::THROW_2) {
      thrw = true;
      break;
    } else if (try_block->type == stm1_result::PRINT_3) {
      if (auto p = std::get_if<std::string>(&try_block->val)) {
        output_str += (*p + "\n");
      }
      if (auto p = std::get_if<int>(&try_block->val)) {
        output_str += (std::to_string(*p) + "\n");
      }
    } else {
      throw std::runtime_error("Impossible state in exec\n");
    }
    try_block = try_block->child;
  }

  if (!thrw)
    // Если не было throw, то и смысла дальше смотреть нет
    return;

  if (auto catch_if_e = std::get_if<std::string>(&try_block->val)) {
    auto catch_if = *catch_if_e;
    if (catch_if != grammar_tree.catch_if_block)
      // Если значение выкинутого exception не совпадает с catch, то и смысла
      // раскрывать catch нет
      return;

  } else {
    return;
  }

  auto catch_block = grammar_tree.catch_block;
  while (true) {
    if (catch_block == nullptr)
      break;
    else if (catch_block->type == stm1_result::THROW_2) {
      break;
    } else if (catch_block->type == stm1_result::PRINT_3) {
      if (auto p = std::get_if<std::string>(&catch_block->val)) {
        output_str += (*p + "\n");
      }
      if (auto p = std::get_if<int>(&catch_block->val)) {
        output_str += (std::to_string(*p) + "\n");
      }
    } else {
      throw std::runtime_error("Impossible state in exec\n");
    }
    catch_block = catch_block->child;
  }
}
