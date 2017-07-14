

#include <iostream>
#include <vector>
using namespace std;

#ifdef _DEBUG
#include <vld.h>
#endif

#include <wiz/ClauText.h>


// rename?
bool IsEmpty(vector<int>& chk_brace, const string& str)
{
	for (int i = 0; i < str.size(); ++i) {
		if ('{' == str[i]) {
			chk_brace.push_back(0);
		}
		else if ('}' == str[i]) {
			if (chk_brace.empty()) {
				chk_brace.push_back(1);
				return false;
			}
			else {
				chk_brace.pop_back();
			}
		}
	}

	return chk_brace.empty();
}


int main(void)
{
	wiz::load_data::UserType global;
	vector<int> chk_brace;
	string command;
	string totalCommand;

	while (true)
	{
		cout << "<< : "; //
		getline(cin, command);

		if (command.empty()) {
			continue;
		}

		if (!command.empty() && '$' == command[0]) {
			if ("$print" == command) {
				cout << ">> : global" << endl;
				cout << global.ToString() << endl;
				cout << endl;
			}
			else if ("$exit" == command) {
				break;
			}
			else if ("$M" == command) {
				MStyleTest(&global);  // for windows!
				system("cls"); // for windows!

				gotoxy(0, 0);
				setcolor(7, 0);

				cout << ">> : $M end" << endl;
			}
			else if (wiz::String::startsWith(command, "$load"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					try {
						const string name = test.GetItemList(0).Get(0);
						const string result = wiz::String::substring(name, 1, name.size() - 2);

						if (wiz::load_data::LoadData::LoadDataFromFile(result, global)) {}
						else {
							cout << ">> : load data from file error" << endl;
						}
					}
					catch (...) // any exception..
					{
						cout << ">> : load error" << endl;
					}
				}
				else {
					cout << ">> : $load syntax Error" << endl;
				}
			}
			else if (wiz::String::startsWith(command, "$call"))
			{
				wiz::load_data::UserType test;

				try {
					if (wiz::load_data::LoadData::LoadDataFromString(command, test))
					{
						try {
							const string id = test.GetItemList(0).Get(0);
							const string result = excute_module("Main = { $call = { id = " + id + "} }", &global, ExcuteData(), 1);

							cout << endl << "excute result is : " << result << endl;
						}
						catch (...) // any exception..
						{
							cout << ">> : $call id or excute module error" << endl;
						}
					}
					else {
						cout << ">> : $call Error" << endl;
					}
				}
				catch (...) {
					cout << ">> : $call load data from string error" << endl;
				}
			}
			else if ("$cls" == command) {
				system("cls"); // for windows!
			}
		}
		else {
			if (IsEmpty(chk_brace, command)) {
				totalCommand.append(command);
				if (wiz::load_data::LoadData::LoadDataFromString(totalCommand, global)) { 
					cout << ">> : Data Added!" << endl;
				}
				else {
					cout << ">> : Error : loaddata from string " << endl;
				}
				command = "";
				totalCommand = "";
			}
			else {
				if (chk_brace[0] == 1) {
					cout << ">> : Error in command, reset command" << endl;
					totalCommand = "";
					command = "";
					chk_brace.clear();
				}
				else {
					totalCommand.append(command);
					command = "";
				}
			}
		}
	}

	return 0;
}

