// <expression> ::= <term> + <expression> | <term> - <expression> | <term>
// <term> ::= <factor> * <term> | <factor> / <term> | <factor>
// <factor> ::= <literal> | ( <expression> )
// <literal> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
#include <iostream>
#include <ctime>
#include <string.h>
#include <variant>
#include <ctype.h>

typedef std::string str;
std::string::size_type cursor = 0;
str input;
char token;
struct exptree{
	char val;
	exptree *left;
	exptree *right;
};

bool get_token(){
	for (; cursor < input.length() && input[cursor] == ' '; cursor++){}
	if (cursor < input.length()) {
		token = input[cursor];
		std::cout << "current token: " << token << std::endl;
		cursor++;
		return true;
	}
	std::cout << "no more tokens\n";
	return false;
}

exptree literal(){
	exptree ret_tree;
	ret_tree.val = token;
	return ret_tree;
}

exptree expression();

exptree factor(){
	// <factor> ::= <literal> | ( <expression> )
	exptree ret_tree;
	if (token == '('){
		// ( <expression> )
		get_token();
		ret_tree = expression();
		get_token(); //get past the closed parenthesis
	} else {
		ret_tree = literal();
	}
	return ret_tree;
}

exptree term(){
	// <term> ::= <factor> * <term> | <factor> / <term> | <factor>
	exptree ret_tree;
	exptree factor_tree = factor();
	if (not get_token()){
		ret_tree = factor_tree;
	} else if (token == '*'){
		// <factor> * <term>
		get_token();
		exptree subterm_tree = term();
		ret_tree.val = '*';
		ret_tree.left = &factor_tree;
		ret_tree.right = &subterm_tree;
	} else if (token == '/'){
		// <factor> / <term>
		get_token();
		exptree subterm_tree = term();
		ret_tree.val = '/';
		ret_tree.left = &factor_tree;
		ret_tree.right = &subterm_tree;
	} else {
		ret_tree = factor_tree;
	}
	return ret_tree;
}

exptree expression(){
	// <expression> ::= <term> + <expression> | <term> - <expression> | <term>
	exptree ret_tree;
	exptree term_tree = term();
	if (not get_token()){
		ret_tree = term_tree;
	} else if (token == '+'){
		// <term> + <expression>
		get_token();
		exptree subexp_tree = expression();
		ret_tree.val = '+';
		ret_tree.left = &term_tree;
		ret_tree.right = &subexp_tree;
	} else if (token == '-'){
		// <term> - <expression>
		get_token();
		exptree subexp_tree = expression();
		ret_tree.val = '-';
		ret_tree.left = &term_tree;
		ret_tree.right = &subexp_tree;
	} else {
		// <term>
		ret_tree = term_tree;
	}
	return ret_tree;
}

int evaluate(exptree exp){
	if (isdigit(exp.val)) {
		return int(exp.val - '0');
	}
	std::cout << exp.val << std::endl;
	std::cout << "evaluating operator\n";
	int a = evaluate(*exp.left);
	int b = evaluate(*exp.right);
	switch (exp.val)
	{
		case '+':
			return a+b;
			break;
		case '-':
			return a-b;
			break;
		case '*':
			return a*b;
			break;
		case '/':
			return a/b;
			break;
	}
	return 0;
}

int main(int argc, char* argv[]){
	input = argv[1];
	get_token();
	exptree exp_tree = expression();
	int result = evaluate(exp_tree);
	std::cout << input << " = " << result << std::endl;
}

