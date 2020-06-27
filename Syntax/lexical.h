#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// TYPE
bool isType(const string& str);
// INTEGER
bool isInteger(const string& str);
// STRING
bool isString(const string& str);
// CHARACTER
bool isCharacter(const string& str);
// BOOLSTRING
bool isBoolstring(const string& str);
// FLOAT
bool isFloat(const string& str);
// ID
bool isID(const string& str);
// KEYWORD
bool isKeyword(const string& str);
// ARITHMETIC
bool isArithmetic(const string& str);
// BITWISE
bool isBitwise(const string& str);
// ASSIGNMENT
bool isAssignment(const string& str);
// COMPARISON
bool isComparison(const string& str);
// SEPARATOR
bool isSeparator(const string& str);
// WHITESPACE
bool isWhitespace(const string& str);

// 출력
string tokenRole(const string& token);
// 시작
void lexicalAnalyzer(const string& nameOfFile);

