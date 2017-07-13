

#include <iostream>
#include <vector>
using namespace std;


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

