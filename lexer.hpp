#ifndef LEXER_HPP
#define LEXER_HPP

#include "./include.hpp"

enum class TokenType {
	// Text types
	KEYWORD,
	IDENTIFIER,
	// Literals
	NUMBER,
	STRING,
	// Operators (Assign)
	EQUAL,
	// Operators (Arithmetic)
	PLUS, PLUS_EQ,
	MINUS, MINUS_EQ,
	ASTERISK, ASTERISK_EQ,
	SLASH, SLASH_EQ,
	PERCENT, PERCENT_EQ,
	POWER, POWER_EQ,
	INCREMENT,
	DECREMENT,
	// Operators (Bitwise)
	AMPERSAND, AMPERSAND_EQ,
	PIPE, PIPE_EQ,
	CARET, CARET_EQ,
	TILDE,
	SHIFTL, SHIFTL_EQ,
	SHIFTR, SHIFTR_EQ,
	// Operators (Logic)
	DOUBLE_AMPERSAND, DOUBLE_AMPERSAND_EQ,
	DOUBLE_PIPE, DOUBLE_PIPE_EQ,
	DOUBLE_CARET, DOUBLE_CARET_EQ,
	BANG,
	// Operators (Comparison)
	EQUALITY,
	INEQUALITY,
	LESS,
	GREATER,
	LESS_EQ,
	GREATER_EQ,
	// Operators (Miscellaneous)
	SEMICOLON,
	DOT,
	// Separators
	LPAREN,
	RPAREN,
	LSQUARE,
	RSQUARE,
	LBRACE,
	RBRACE,
	COMMA,
	// Special
	UNKNOWN,
	TOKEN_EOF,
};

std::unordered_set<std::string> keywords = {
	// Data Types
	"bool",
	"int",
	"string",
	// Control Flow
	"if",
	"else",
	"for",
	"while",
	"continue",
	"break",
	// Constants
	"true",
	"false",
};

std::unordered_map<std::string, TokenType> symbolMap = {
	// Assign
	{ "=", TokenType::EQUAL },
	// Arithmetic
	{ "+", TokenType::PLUS }, { "+=", TokenType::PLUS_EQ },
	{ "-", TokenType::MINUS }, { "-=", TokenType::MINUS_EQ },
	{ "*", TokenType::ASTERISK }, { "*=", TokenType::ASTERISK_EQ },
	{ "/", TokenType::SLASH }, { "/=", TokenType::SLASH_EQ },
	{ "%", TokenType::PERCENT }, { "%=", TokenType::PERCENT_EQ },
	{ "**", TokenType::POWER }, { "**=", TokenType::POWER_EQ },
	{ "++", TokenType::INCREMENT },
	{ "--", TokenType::DECREMENT },
	// Bitwise
	{ "&", TokenType::AMPERSAND }, { "&=", TokenType::AMPERSAND_EQ },
	{ "|", TokenType::PIPE }, { "|=", TokenType::PIPE_EQ },
	{ "^", TokenType::CARET }, { "^=", TokenType::CARET_EQ },
	{ "~", TokenType::TILDE },
	{ "<<", TokenType::SHIFTL }, { "<<=", TokenType::SHIFTL_EQ },
	{ ">>", TokenType::SHIFTR }, { ">>=", TokenType::SHIFTR_EQ },
	// Logic
	{ "&&", TokenType::DOUBLE_AMPERSAND }, { "&&=", TokenType::DOUBLE_AMPERSAND_EQ },
	{ "||", TokenType::DOUBLE_PIPE }, { "||=", TokenType::DOUBLE_PIPE_EQ },
	{ "^^", TokenType::DOUBLE_CARET }, { "^^=", TokenType::DOUBLE_CARET_EQ },
	// Comparison
	{ "==", TokenType::EQUALITY }, { "!=", TokenType::INEQUALITY },
	{ "<", TokenType::LESS }, { "<=", TokenType::LESS_EQ },
	{ ">", TokenType::GREATER }, { ">=", TokenType::GREATER_EQ },
	/* Terminator */ { ";", TokenType::SEMICOLON },
	/* Property */ { ".", TokenType::DOT },
	// Separators
	{ "(", TokenType::LPAREN }, { ")", TokenType::RPAREN },
	{ "[", TokenType::LSQUARE }, { "]", TokenType::RSQUARE },
	{ "{", TokenType::LBRACE }, { "}", TokenType::RBRACE },
	{ ",", TokenType::COMMA },
};
std::unordered_set<char> validSymbolStarters;
void initialiseSymbolStarters() {
	for (const auto& symbol : symbolMap) {
		validSymbolStarters.insert(symbol.first[0]);
	}
}

struct Token {
	TokenType type;
	std::string value;
	int line;
	int col;

	Token(TokenType type, std::string value, int line, int col)
		: type(type), value(value), line(line), col(col) {}
};

struct SyntaxError {
	std::string message;
	int line;
	int col;
	SyntaxError(std::string message, int line, int col)
		: message(message), line(line), col(col) {}
};

class Lexer {
public:
	Lexer(const std::string& filename, const std::string& source) : filename(filename), src(source), pos(0), line(1), col(1) {
		if (validSymbolStarters.empty()) {
			initialiseSymbolStarters();
		}
	}
	void logErrors();
	inline char peek(int);
	void skip();
	Token getKeywordOrIdentifier();
	Token getNumber();
	Token getString();
	Token nextToken();
	std::vector<Token> tokenize();
private:
	std::string filename;
	std::string src;
	int pos;
	int line;
	int col;
	std::vector<std::string> lines;
	std::vector<SyntaxError> syntaxErrors;
};

void Lexer::logErrors() {
	if (lines.empty()) {
		std::stringstream ss(src);
		std::string line;
		while (std::getline(ss, line)) {
			lines.push_back(line);
		}
	}
	for (auto& error : syntaxErrors) {
		std::cout << filename << ':' << error.line << ':' << error.col << ": ";
		std::cout << "\033[1;31mSyntaxError:\033[0m " << error.message << '\n';
		std::cout << error.line << "|" << lines[error.line - 1] << "\n";
		std::cout << std::string(std::ceil(std::log10(error.line + 0.1)) + error.col, ' ') << "^\n";
	}
	if (!syntaxErrors.empty()) {
		std::cout << syntaxErrors.size() << " errors generated.\n";
	}
}

inline char Lexer::peek(int position) {
	if (position >= src.size() || position < 0) return '\0';
	return src[position];
}

// Skip whitespace and comments (does not return EOF)
void Lexer::skip() {
	while (true) {
		// Skip whitespace
		while (isspace(peek(pos))) {
			if (src[pos] == '\n') {
				line++;
				col = 1;
			} else {
				col++;
			}
			pos++;
		}
		// Skip comments
		if (peek(pos) == '/') {
			if (peek(pos + 1) == '/') {
				// Single line comment
				while (pos < src.size() && src[pos] != '\n') {
					pos++;
					col++;
				}
				if (pos == src.size()) {
					return; // Return EOF in nextToken()
				}
				line++;
				pos++;
				col = 1;
				continue;
			} else if (peek(pos + 1) == '*') {
				// Multi line comment
				int startLine = line, startCol = col;
				pos += 2;
				col += 2;
				bool closed = false;
				while (pos + 1 < src.size()) {
					if (src[pos] == '\n') {
						pos++;
						line++;
						col = 1;
						continue;
					} else if (src[pos] == '*' && src[pos + 1] == '/') {
						// Closing `*/` found
						pos += 2;
						col += 2;
						closed = true;
						break;
					}
					pos++;
					col++;
				}
				if (!closed) {
					// If the last character is a newline, update the line and col of EOF
					if (src[pos] == '\n') {
						line++;
						col = 1;
					}
					syntaxErrors.push_back(SyntaxError("Unmatched '/*' found", startLine, startCol));
					pos = src.size();
					return; // Return EOF in nextToken()
				}
				continue;
			}
		}
		break; // No more skipping to be done
	}
}

Token Lexer::getKeywordOrIdentifier() {
	int startCol = col;
	std::string val = "";
	while (isalnum(peek(pos)) || peek(pos) == '_') {
		val += src[pos];
		pos++;
		col++;
	}
	if (keywords.count(val)) return Token(TokenType::KEYWORD, val, line, startCol);
	return Token(TokenType::IDENTIFIER, val, line, startCol);
}

// Helper function to get whether the number is a valid digit according to the base
bool isDigitBased(char c, int base) {
	int digit = -1;
	if (isdigit(c)) {
		digit = c - '0';
	} else if (isalpha(c)) {
		digit = toupper(c) - 'A' + 10;
	}
	return digit >= 0 && digit < base;
}

Token Lexer::getNumber() {
	int startCol = col;
	// + or - infront of a number literal is treated as a unary operator
	int base = 10; // decimal
	std::string val = "";
	if (src[pos] == '0') {
		switch (peek(pos + 1)) {
			case 'b':
			case 'B':
				base = 2; // binary
				pos += 2;
				col += 2;
				val += "0b";
				break;
			case 'o':
			case 'O':
				base = 8; // octal
				pos += 2;
				col += 2;
				val += "0o";
				break;
			case 'x':
			case 'X':
				base = 16; // hexadecimal
				pos += 2;
				col += 2;
				val += "0x";
				break;
		}
	}
	if (base != 10 && !isDigitBased(peek(pos), base)) {
		syntaxErrors.push_back(SyntaxError("Expected non-decimal literal", line, col));
	}
	bool hasDot = false;
	bool scientific = false;
	while (pos < src.size()) {
		char c = src[pos];
		if (c == '.') {
			// Decimal point
			if (base != 10) {
				syntaxErrors.push_back(SyntaxError("Non-decimal literal found with a decimal point", line, col));
				break;
			}
			if (hasDot) {
				syntaxErrors.push_back(SyntaxError("Number literal with two decimal points found", line, col));
				break;
			}
			if (scientific) {
				syntaxErrors.push_back(SyntaxError("Non-integer scientific index of number literal found", line, col));
				break;
			}
			if (!isDigitBased(peek(pos + 1), base)) {
				syntaxErrors.push_back(SyntaxError("Expected decimal part of number literal", line, col));
				break;
			}
			hasDot = true;
		} else if (base == 10 && (c == 'e' || c == 'E')) {
			// Scientific notation (only consider if decimal)
			if (!isdigit(peek(pos + 1))) {
				// If the next character is not a digit, it has to be +/- followed by a digit
				if ((peek(pos + 1) != '+' && peek(pos + 1) != '-') || !isdigit(peek(pos + 2))) {
					syntaxErrors.push_back(SyntaxError("Expected scientific index of decimal number literal", line, col));
					break;
				}
			}
			scientific = true;
			hasDot = false;
		} else if (c == '+' || c == '-') {
			// If the number is not decimal or the sign is not after scientific notation, it is a separate token
			if (base != 10 || (peek(pos - 1) != 'e' && peek(pos - 1) != 'E')) break;
			// Otherwise this sign is for the scientific index
		} else if (c == '\'') {
			// Only include in the number literal if surrounded by digits
			if (!isDigitBased(peek(pos - 1), base) || !isDigitBased(peek(pos + 1), base)) {
				break;
			}
		} else if (!isDigitBased(c, base)) {
			// An invalid digit was found, end token here
			if (!isspace(c) && !validSymbolStarters.count(c)) {
				syntaxErrors.push_back(SyntaxError("Invalid digit found in number literal ('" + std::string(1, c) + "')", line, col));
			}
			break;
		}
		val += c;
		pos++;
		col++;
	}
	return Token(TokenType::NUMBER, val, line, startCol);
}

Token Lexer::getString() {
	int startLine = line, startCol = col;
	pos++;
	col++;
	std::string val = "";
	bool escape = false;
	while (pos < src.size()) {
		char c = src[pos];
		if (c == '\n') {
			pos++;
			line++;
			col = 0;
			break;
		}
		if (escape) {
			switch (c) {
				case '\'': val += '\''; break;
				case '"': val += '"'; break;
				case '\\': val += '\\'; break;
				case 'a': val += '\a'; break;
				case 'b': val += '\b'; break;
				case 'f': val += '\f'; break;
				case 'n': val += '\n'; break;
				case 'r': val += '\r'; break;
				case 't': val += '\t'; break;
				case 'v': val += '\v'; break;
				case 'x': { // `\xnn`: Hexadecimal
					if (!isDigitBased(peek(pos), 16)) {
						syntaxErrors.push_back(SyntaxError("Expected hexadecimal value after hexadecimal escape sequence", line, col));
						break; // Eat this character in the outer loop
					}
					pos++;
					col++;
					int num = 0;
					for (; isDigitBased(peek(pos), 16); pos++, col++) {
						int digit = isdigit(peek(pos)) ? peek(pos) - '0' : toupper(peek(pos)) - 'A' + 10;
						num = num * 16 + digit;
					}
					val += static_cast<char>(num);
					pos--; col--; // The last character will be eaten in the outer loop
					break;
				} default:
					if (isDigitBased(c, 8)) { // `\nnn`: Octal
						int num = 0;
						for (int i = 0; i < 3 && isDigitBased(peek(pos), 8); i++, pos++, col++) {
							num = num * 8 + (peek(pos) - '0');
						}
						val += static_cast<char>(num);
						pos--; col--; // The last character will be eaten in the outer loop
						break;
					}
					syntaxErrors.push_back(SyntaxError("Unknown escape sequence found inside string literal ('\\" + std::string(1, c) + "')", line, col));
					val += c;
					break;
			}
			escape = false;
		} else if (c == '\\') {
			escape = true;
		} else if (c == '"') {
			pos++;
			col++;
			return Token(TokenType::STRING, val, line, startCol);
		} else {
			val += src[pos];
		}
		pos++;
		col++;
	}
	// If this code is reached, the string is unclosed in the same line
	syntaxErrors.push_back(SyntaxError("Unclosed string literal found", startLine, startCol));
	return Token(TokenType::STRING, val, startLine, startCol);
}

Token Lexer::nextToken() {
	// Perform skips until a parseable character is found
	skip();
	// Check for end of file
	if (pos >= src.size()) {
		return Token(TokenType::TOKEN_EOF, "", line, col);
	}
	char c = src[pos];
	// Check for keywords and identifiers
	if (isalpha(c) || c == '_') {
		return getKeywordOrIdentifier();
	}
	// Check number literal
	if (isdigit(c) || (c == '.' && isdigit(peek(pos + 1)))) {
		return getNumber();
	}
	// Check string literal
	if (c == '"') {
		return getString();
	}
	// Handle unopened `*/`
	if (c == '*' && peek(pos + 1) == '/') {
		syntaxErrors.push_back(SyntaxError("Unmatched '*/' found", line, col));
		Token t = Token(TokenType::UNKNOWN, src.substr(pos, 2), line, col);
		pos += 2;
		col += 2;
		return t;
	}
	// Check symbol for operator and separator
	int maxSymbolLen = std::min(3, static_cast<int>(src.size()) - pos);
	// The largest symbol is 3 characters long
	// Find the largest matching symbol with max `maxSymbolLen` characters
	for (int len = maxSymbolLen; len >= 1; len--) {
		std::string symbol = src.substr(pos, len);
		if (symbolMap.count(symbol)) {
			int startCol = col;
			pos += len;
			col += len;
			return Token(symbolMap[symbol.data()], "", line, startCol);
		}
	}
	// Unknown token
	syntaxErrors.push_back(SyntaxError("Unknown token found", line, col));
	return Token(TokenType::UNKNOWN, std::string(1, src[pos++]), line, col++);
}

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;
	while (true) {
		Token t = nextToken();
		tokens.push_back(t);
		if (t.type == TokenType::TOKEN_EOF) break;
	}
	return tokens;
}

#endif

/*
TODO:
- Add unicode support to string literals?
- Handle incorrect position pointing in error logs with tabs
*/
