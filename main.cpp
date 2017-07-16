

#include <iostream>
#include <vector>
#include <fstream>
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
void SaveWithOutEvent(ostream& stream, wiz::load_data::UserType* ut, int depth)
{
	int itemListCount = 0;
	int userTypeListCount = 0;

	for (int i = 0; i < ut->GetCommentListSize(); ++i) {
		for (int k = 0; k < depth; ++k) {
			stream << "\t";
		}
		stream << (ut->GetCommentList(i));

		if (i < ut->GetCommentListSize() - 1 || 0 == ut->GetIListSize()) {
			stream << "\n";
		}
	}

	for (int i = 0; i < ut->GetIListSize(); ++i) {
		//std::cout << "ItemList" << endl;
		if (ut->IsItemList(i)) {
			for (int j = 0; j < ut->GetItemList(itemListCount).size(); j++) {
				string temp;
				for (int k = 0; k < depth; ++k) {
					temp += "\t";
				}
				if (ut->GetItemList(itemListCount).GetName() != "") {
					temp += ut->GetItemList(itemListCount).GetName();
					temp += "=";
				}
				temp += ut->GetItemList(itemListCount).Get(j);
				if (j != ut->GetItemList(itemListCount).size() - 1) {
					temp += "\n";
				}
				stream << temp;
			}
			if (i != ut->GetIListSize() - 1) {
				stream << "\n";
			}
			itemListCount++;
		}
		else if (ut->IsUserTypeList(i)) {
			if (ut->GetUserTypeList(userTypeListCount)->GetName() == "Event"
				|| ut->GetUserTypeList(userTypeListCount)->GetName() == "Main") {
				userTypeListCount++;
				continue;
			}

			// std::cout << "UserTypeList" << endl;
			for (int k = 0; k < depth; ++k) {
				stream << "\t";
			}

			if (ut->GetUserTypeList(userTypeListCount)->GetName() != "") {
				stream << ut->GetUserTypeList(userTypeListCount)->GetName() << "=";
			}

			stream << "{\n";

			SaveWithOutEvent(stream, ut->GetUserTypeList(userTypeListCount), depth + 1);
			stream << "\n";

			for (int k = 0; k < depth; ++k) {
				stream << "\t";
			}
			stream << "}";
			if (i != ut->GetIListSize() - 1) {
				stream << "\n";
			}

			userTypeListCount++;
		}
	}
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
				//cout << global.ToString() << endl;
				global.Save1(cout);
				cout << endl;
			}
			else if ("$print_data_only" == command) {
				cout << ">> : global" << endl;
				SaveWithOutEvent(cout, &global, 0);
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
			else if (wiz::String::startsWith(command, "$call"))
			{
				wiz::load_data::UserType test;

				try {
					if (wiz::load_data::LoadData::LoadDataFromString(command, test))
					{
						try {
							wiz::load_data:: UserType ut = global;
							const string id = test.GetItemList(0).Get(0);
							const string result = excute_module("Main = { $call = { id = " + id + "} }", &ut, ExcuteData(), 1);

							global = std::move(ut);
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
			else if (wiz::String::startsWith(command, "$save_event_only"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					ofstream outFile;

					try {
						const string name = test.GetItemList(0).Get(0);
						const string result = wiz::String::substring(name, 1, name.size() - 2);

						outFile.open(result);
						if (outFile.fail()) {
							//
						}
						else {
							for (int i = 0; i < global.GetUserTypeListSize(); ++i) {
								if (global.GetUserTypeList(i)->GetName() == "Event") {
									outFile << "Event = {\n";
									global.GetUserTypeList(i)->Save1(outFile, 1);
									outFile << "\n}\n";
								}
							}
							outFile.close();
						}
					}
					catch (...) // any exception..
					{
						if (outFile.is_open()) {
							outFile.close();
						}
						cout << ">> : $save_event_only error" << endl;
					}
				}
				else {
					cout << ">> : $save_event_only syntax Error" << endl;
				}
			}

			else if (wiz::String::startsWith(command, "$save_data_only"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					ofstream outFile;

					try {
						const string name = test.GetItemList(0).Get(0);
						const string result = wiz::String::substring(name, 1, name.size() - 2);

						outFile.open(result);
						if (outFile.fail()) {
							//
						}
						else {
							SaveWithOutEvent(outFile, &global, 0);
							outFile.close();
						}
					}
					catch (...) // any exception..
					{
						if (outFile.is_open()) {
							outFile.close();
						}
						cout << ">> : $save_data_only error" << endl;
					}
				}
			}
			else if (wiz::String::startsWith(command, "$save"))
			{
				wiz::load_data::UserType test;
				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					const string name = test.GetItemList(0).Get(0);
					const string result = wiz::String::substring(name, 1, name.size() - 2);

					if (wiz::load_data::LoadData::SaveWizDB(global, result, "1")) {

					}
					else {
						cout << ">> : $save error" << endl;
					}
				}
				else {
					cout << ">> : $save syntax Error" << endl;
				}
			}
			else if ("$cls" == command) {
				system("cls"); // for windows!
			}
		}
		else {
			if (IsEmpty(chk_brace, command)) {
				command.append("\n");

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
					command.append("\n");

					totalCommand.append(command);
					command = "";
				}
			}
		}
	}

	return 0;
}

