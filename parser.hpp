#ifndef PARSER_HPP
#define PARSER_HPP

#include "include.hpp"
#include "lexer.hpp"

class ASTNode { public: virtual ~ASTNode() = default; };
 
// Statement node
class Stmt : public ASTNode {};
// Expession node
class Expr : public ASTNode {};

// Statement definitions
class DeclareStmt : public Stmt {
public: // type id;
	Token type;
	std::string id;
};
class DefineStmt : public Stmt {
public: // id = val;
	std::string id;
	std::unique_ptr<Expr> val;
};
class DeclareDefineStmt : public Stmt {
public: // type id = val;
	Token type;
	std::string id;
	std::unique_ptr<Expr> val;
};
class IfStmt : public Stmt {
public:
	struct IfBlock { // if (cond) {then}
		std::unique_ptr<Expr> cond;
		std::vector<std::unique_ptr<Stmt>> then;
	};
	std::vector<IfBlock> branches; // if (cond) {then} else if (cond) {then} ...
	std::vector<std::unique_ptr<Stmt>> elseBranch; // else {elseBranch}
};
class ForStmt : public Stmt {
public: // for (init; cond; update) {loop}
	std::unique_ptr<Stmt> init;
	std::unique_ptr<Expr> cond;
	std::unique_ptr<Stmt> update;
	std::vector<std::unique_ptr<Stmt>> loop;
};
class WhileStmt : public Stmt {
public: // while (cond) {loop}
	std::unique_ptr<Expr> cond;
	std::vector<std::unique_ptr<Stmt>> loop;
};
class ContinueStmt : public Stmt {}; // continue;
class BreakStmt : public Stmt {}; // break;

// Expression definitions
class LiteralExpr : public Expr {
public: // Boolean literals(true/false), String literals, Number literals
	Token val;
};
class IDExpr : public Expr {
public: // Identifiers (Variables)
	std::string id;
};
class BinaryExpr : public Expr {
public: // left op right
	std::unique_ptr<Expr> left;
	Token op;
	std::unique_ptr<Expr> right;
};
class UnaryExpr : public Expr {
public: // op expr
	Token op;
	std::unique_ptr<Expr> expr;
};
class GroupExpr : public Expr {
public: // (expr)
	std::unique_ptr<Expr> expr;
};

class Parser {
public:
	Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {
		//
	}
private:
	std::vector<Token> tokens;
	int pos;
};

#endif

/*
TODO:
- Add parsing logic
*/
