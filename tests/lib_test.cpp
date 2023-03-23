#include "lib.hpp"
#include <cstdio>
#include <gtest/gtest.h>

extern int symbol;
extern int value;
extern FILE *current_stream;
extern std::string str;

void write_string_to_stdin(char *str) {
  current_stream = fmemopen(str, strlen(str), "r+");
}

void write_string_to_stdin(const std::string &str) {
  current_stream = fmemopen((void *)str.c_str(), str.size(), "r+");
}

TEST(YourLibTests, TestFunction) { EXPECT_EQ(test_test(), 0); }
TEST(YourLibTests, TestScan) {
  std::string test_string = "try {throw new E (1);} catch(E exc){print ('x');}";
  write_string_to_stdin(test_string);

  symbol = scan();
  EXPECT_EQ(symbol, TRY);

  symbol = scan();
  EXPECT_EQ(symbol, O_CURLY_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, THROW);

  symbol = scan();
  EXPECT_EQ(symbol, NEW);

  symbol = scan();
  EXPECT_EQ(symbol, EXC);

  symbol = scan();
  EXPECT_EQ(symbol, O_ROUND_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, INT);
  EXPECT_EQ(1, value);

  symbol = scan();
  EXPECT_EQ(symbol, C_ROUND_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, SEMICOLON);

  symbol = scan();
  EXPECT_EQ(symbol, C_CURLY_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, CATCH);

  symbol = scan();
  EXPECT_EQ(symbol, O_ROUND_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, EXC);

  symbol = scan();
  EXPECT_EQ(symbol, STR);
  EXPECT_EQ("exc", str);

  symbol = scan();
  EXPECT_EQ(symbol, C_ROUND_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, O_CURLY_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, PRINT);

  symbol = scan();
  EXPECT_EQ(symbol, O_ROUND_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, STR);
  EXPECT_EQ("x", str);

  symbol = scan();
  EXPECT_EQ(symbol, C_ROUND_BRACKET);

  symbol = scan();
  EXPECT_EQ(symbol, SEMICOLON);

  symbol = scan();
  EXPECT_EQ(symbol, C_CURLY_BRACKET);
}

TEST(YourLibTests, TestValGrammar) {
  symbol = INT;
  value = 10;

  auto result = val();

  if (auto p = std::get_if<int>(&result)) {
    EXPECT_EQ(*p, 10);
  } else {
    FAIL() << "Returned value should be int type\n";
  }

  symbol = STR;
  str = "fi";

  result = val();

  if (auto p = std::get_if<std::string>(&result)) {
    EXPECT_EQ(*p, "fi");
  } else {
    FAIL() << "Returned value should be string type\n";
  }
}

TEST(YourLibTests, TestStm1Grammar) {
  // type1
  std::string test_string = "throw new E (1);";
  write_string_to_stdin(test_string);

  symbol = scan();
  auto result = stm1();

  EXPECT_EQ(result->type, stm1_result::THROW_2);
  if (auto p = std::get_if<int>(&result->val)) {
    EXPECT_EQ(*p, 1);
  } else {
    FAIL() << "Returned value should be int type\n";
  }
  EXPECT_EQ(result->child, nullptr);
  // type 2
  test_string = "print('10');";
  write_string_to_stdin(test_string);

  symbol = scan();
  result = stm1();

  EXPECT_EQ(result->type, stm1_result::PRINT_3);
  if (auto p = std::get_if<std::string>(&result->val)) {
    EXPECT_EQ(*p, "10");
  } else {
    FAIL() << "Returned value should be string type\n";
  }
  EXPECT_EQ(result->child, nullptr);
}

TEST(YourLibTests, TestSGrammar) {
  std::string test_string = "try {throw new E (1);} catch(E exc){print ('x');}";
  write_string_to_stdin(test_string);

  symbol = scan();
  auto result = s();

  EXPECT_EQ(result.try_block->type, stm1_result::THROW_2);
  if (auto p = std::get_if<int>(&result.try_block->val)) {
    EXPECT_EQ(*p, 1);
  } else {
    FAIL() << "Returned value should be int type\n";
  }
  EXPECT_EQ(result.try_block->child, nullptr);

  EXPECT_EQ(result.catch_if_block, "exc");

  EXPECT_EQ(result.catch_block->type, stm1_result::PRINT_3);
  if (auto p = std::get_if<std::string>(&result.catch_block->val)) {
    EXPECT_EQ(*p, "x");
  } else {
    FAIL() << "Returned value should be string type\n";
  }
  EXPECT_EQ(result.catch_block->child, nullptr);
}

TEST(YourLibTests, TestAll) {
  std::string test_string = "try {print(1);print('y');throw new E "
                            "('exc');print('This will not be printed');} "
                            "catch(E exc){print ('x'); print(1);}";
  write_string_to_stdin(test_string);

  symbol = scan();
  auto result_tree = s();
  std::string result_output;
  exec(result_tree, result_output);
  std::string expected_output = "1\ny\nx\n1\n";
  EXPECT_EQ(expected_output, result_output);
  EXPECT_EQ(result_output, expected_output);
}
