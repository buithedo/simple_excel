//Implement a simple excel application.
//***Input: input will be given via stdin
//-The first line will contain a number N indicating the number of cells
//- The rest 2*N lines will have the following structure:
//+ The first line contains the cell name (e.g A1)
//+ The second line contains the cell content. The content of each cell can be a
//number (double), or a mathematic formula (consists of +-*/) and the formula
//can also refers to other cells.the formula is written in polish postfix
//notation
//***Output : the final values of each cells, sorted alphabetically by the cell names, to stdout
//***Important:
//- The solution will be evaluated automatically, so the output must strictly adhere to
//the above format
//- The application must report for any circular dependencies

#include <iostream>
#include <string> 
#include <locale>    
#include <regex>
#include<vector>
#include <stack> 
#include <algorithm>
using namespace std;

class Cell
{
	public:
		static bool isNumber(std::string str_test)
		{
			return std::regex_match(str_test, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
		}

		static bool is_operator(char ch)
		{
			if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
				return true;
			else
				return false;
		}

		static double cal_operation(double op1, double op2, char op)
		{
			double re;
			switch (op) {
				case '+':
					re = op2 + op1;
					break;
				case '-':
					re = op2 - op1;
					break;
				case '*':
					re = op2 * op1;
					break;
				case '/':
					re = op2 / op1;
					break;
			}
			return re;
		}

	public:
		vector<Cell> *sheet;
		string name;
		string formula;

		Cell(string name, string formula) {
			this->name = name;
			this->formula = formula;
		}

		double eval_postfix_expressions(string postfix)
		{
			if (Cell::isNumber(postfix)) {
				return stod(postfix);
			}

			stack <double> s;
			char buffer[15];
			int j = 0;
			double result, op1, op2, x;
			for (int i = 0; i < postfix.length(); i++) {
				if ((postfix[i] >= '0' && postfix[i] <= '9') 
					|| (postfix[i] >= 'a' && postfix[i] <= 'z') 
					|| (postfix[i] >= 'A' && postfix[i] <= 'Z')
					|| postfix[i] == '.' ){
					buffer[j++] = postfix[i];
				}
				else if (postfix[i] == ' ') {
					if (j > 0) {
						buffer[j] = '\0';
						if (isalpha(buffer[0]) == true) {
							for (int ii = 0; ii < this->sheet->size(); ii++) {
								if (this->sheet->at(ii).name == string(buffer)) {
									x = this->sheet->at(ii).value();
									break;
								}
							}
						}
						else {
							x = atof(buffer);
						}
						s.push(x);
						j = 0;
					}
				}
				else if (Cell::is_operator(postfix[i])) {
					op1 = s.top();
					s.pop();
					op2 = s.top();
					s.pop();
					s.push(Cell::cal_operation(op1, op2, postfix[i]));
				}

			}
			result = s.top();
			return result;
		}

		double value() {
			return Cell::eval_postfix_expressions(this->formula);
		}

		void print() {
			cout << this->name << endl << this->value();
		}
};


int main() {
	//Number of cells
	int n = 0;
	cin >> n;
	cin.ignore();
	if (cin.fail())
		cout << "Error number of cells";

	// Input all cell
	vector<Cell> sheet;
	for (int i = 1; i <= n; i++) {

		// Input cell name
		string cell_name;
		getline(cin, cell_name);
	
		// Input cell formula
		string formula;
		getline(cin, formula);
	
		Cell my_cell = Cell(cell_name, formula);
		my_cell.sheet = &sheet;
		sheet.push_back(my_cell);
	}

	// Check Circular
	bool is_check_circular = false;
	for (int i = 0; i < sheet.size(); i++) {
		if (is_check_circular == true) {
			break;
		}
		for (int j = 0; j < sheet.size(); j++) {
			if (sheet.at(j).formula.find(sheet.at(i).name) != string::npos ){
				if (sheet.at(i).formula.find(sheet.at(j).name) != string::npos) {
					cout << "Circular dependency between "<<sheet.at(i).name<<" and "<<sheet.at(j).name<<" detected";
					is_check_circular = true;
					break;
				}
			}			
		}
	}

	// Print result
	if (is_check_circular == false) {
		for (int i = 0; i < sheet.size(); i++) {
			sheet.at(i).print();
			if (i < sheet.size() - 1) {
				cout << endl;
			}
		}
	}
	
	//cout << endl;
	//system("pause");
}


