/****************************************************
Course		: CO2210 Programming Quest
Name		: YAPA.YMTL
Index no.	: 17_ENG_119
Asgmnt No.	: 01
Task		: 01
Date		: 26-08-2019
****************************************************/


//Header files
#include<iostream>
#include <string>
#include<fstream>
#include<vector>


using namespace std;

/************* Begingig of stack ************************/
class node
{
public:
	node* next;
	float data;
};

class stack
{
	node* top;
public:
	stack() { top = NULL; }
	void push(float);
	float pop();
	void prfloat();
	float topdata();//Return top data of the stack
	bool isempty();//Check the is stack empty
};

void stack::push(float num=0)
{
	node* n=new node;
	n->data = num;
	if (top == NULL)
	{
		top = n;
		n->next = NULL;
	}
	else
	{
		n->next = top;
		top = n;
	}
}

float stack::pop()
{
	if (top == NULL)
	{
		//cout << "\nStack is empty.\n\n";
	}
	else
	{
		float top_data;
		node* ptr = top;
		top_data = top->data;
		top = top->next;
		if (top != NULL)
			delete[]ptr;
		return top_data;
	}
}

void stack::prfloat()
{
	if (top == NULL)
	{
		//cout << "\nStack is empty.\n\n";
	}
	else
	{
		node* ptr = top;
		cout << "***Stack List***\n";
		while (ptr != NULL)
		{
			cout << "->" << ptr->data << endl;
			ptr = ptr->next;
		}
	}
}

float stack::topdata()
{
	if (top != NULL)
		return top->data;
	else
		return 0;
}

bool stack::isempty()
{
	if (top == NULL)
		return 1;
	else
		return 0;
}
/**************end of stack ***********************/


//Use templates to any types of variables
template<class T>

float cal(T N, T M, char sym)
{
	float n = N;
	float m = M;
	if (sym == '-')
		return n - m;

	else if (sym == '+')
		return n + m;

	else if (sym == '*')
		return n * m;

	else if (sym == '/')
		return n / m;
}

int main()
{
	string file_location;//Save file location which get from user
	string formula;

	cout << "Enter your file location (eg:-F:\\Programming Quest\\Assignment 01)\n\t";

	getline(cin,file_location);//Get file location from user

	ifstream from_file;
	ofstream to_file;
	from_file.open(file_location + "\\formulas.txt");
	to_file.open(file_location + "\\Results.txt");
	

	if (!from_file)//Validate the file location and file
	{
		cout << "ERROR!!!\nFailed to open text file please check the location & file........\n\n";
		system("pause");
		system("cls");
		cout << "Enter location again or if you want to exit enter 'e' \n (eg:-F : \\Programming Quest\\Assignment 01\\formulas.txt)\n\t";

		getline(cin, file_location);

		if(file_location=="e"||file_location=="E")
			exit(1);//Exit from the programme due to unable to find file

		from_file.open(file_location + "\\formulas.txt");
	}

	while (from_file>>formula)
	{
		cout << formula;
		to_file << formula;//save formula in file

		formula.pop_back();//Remove '=' sign
		formula.push_back(')');//add close bracket to the end of formula
		formula.insert(formula.begin(), '(');//Add open bracket to the begining of the formula


		stack list;//Stack for save the number
		float temp_n, temp_m;
		char temp_sym;
		bool check=0;
		vector<char> symbols;
	

		
		for (int i = 0; i < formula.size(); i++)
		{
			if (float(formula[i]) >= 48 && float(formula[i] <= 57))
			{
				float num=float(formula[i])-48;//Creat number from string and add to the stack
				if (list.topdata() != 0 && !check)
				{
					list.push(list.pop()*10+num);
				}
				else
				{
					list.push(num);
					check = 0;
				}
			}
			else
			{
				if ( formula[i] == ')' || formula[i] == '}' || formula[i] == ']')//Check whether it is a closing bracket or not
				{
					while (symbols.back()!='('&& symbols.back() != '['&& symbols.back() != '{')//If so pop the symbols vector until met open bracket
					{
						temp_sym = symbols.back();
						temp_n = list.pop();
						temp_m = list.pop();
						if (symbols.back() == '/' && (temp_n == 0 || temp_m == 0))//Validate for not dividing from zero
						{
							to_file << "E\n";
							cout << "E\n";
							i = formula.size();
							//exit(1);
						} 
						else
						{
							symbols.pop_back();
							list.push(cal(temp_m, temp_n, temp_sym));
						}
					}

					if (!symbols.empty() && ((formula[i] == '}' && symbols.back() != '{') || (formula[i] == ']' && symbols.back() != '[') || (formula[i] == ')' && symbols.back() != '(')))
					{
						to_file << "E\n";
						cout << "E\n";
						i = formula.size();
						//exit(1);
					}
					else
						symbols.pop_back();
				}
				else
				{
					if (!symbols.empty()&&(symbols.back() == '/' || symbols.back() == '*')&& (formula[i] == '-' || formula[i] == '+'))
					{
						temp_sym = symbols.back();
						temp_n = list.pop();
						temp_m = list.pop();
						symbols.pop_back();
						if (symbols.back() == '/' && (temp_n == 0 || temp_m == 0))
						{
							to_file << "E\n";
							cout << "E\n";
							i = formula.size();
						}
						else
						{
							symbols.push_back(formula[i]);
							list.push(cal(temp_m, temp_n, temp_sym));
						}
					}
					else if (!symbols.empty() && symbols.back() == '-' )
					{
						if (formula[i] == '+')
						{
							temp_sym = symbols.back();
							symbols.pop_back();
							symbols.push_back(formula[i]);
							temp_n = list.pop();
							temp_m = list.pop();
							if (symbols.back() == '/' && (temp_n == 0 || temp_m == 0))
							{
								to_file << "E\n";
								cout << "E\n";
								i = formula.size();
							}
							else
								list.push(cal(temp_m, temp_n, temp_sym));
						}
					}

					else
					{
						symbols.push_back(formula[i]);
					}
				}
				check = 1;
			}
		}
		if (symbols.empty())
		{
			float temp = floor(list.topdata() * 100)/100;//Cut the floating number after 2 points
			to_file <<temp<<endl;
			cout << temp<<endl;
		}
		else 
			to_file << "E\n";
	}

	cout << "\t\tDone.";

	return 0;
}


