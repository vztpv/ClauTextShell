

#include <iostream>
#include <vector>
#include <fstream>


#ifdef _DEBUG
#include <vld.h>
#endif

#include <wiz/ClauText.h>


// rename?
bool IsEmpty(std::vector<int>& chk_brace, const std::string& str)
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
void SaveWithOutEvent(std::ostream& stream, wiz::load_data::UserType* ut, int depth)
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
				std::string temp;
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
	std::vector<int> chk_brace;
	std::string command;
	std::string totalCommand;

	while (true)
	{
		std::cout << "<< : "; //
		std::getline(std::cin, command);

		if (command.empty()) {
			continue;
		}
		
		// Evnent = {
		//		$call = { id = 0 } # tab or 들여쓰기!!
		if (!command.empty() && '$' == command[0]) {
			if ("$print" == command) {
				std::cout << ">> : global" << std::endl;
				//cout << global.ToString() << endl;
				global.Save1(std::cout);
				std::cout << std::endl;
			}
			else if ("$print_data_only" == command) {
				std::cout << ">> : global" << std::endl;
				SaveWithOutEvent(std::cout, &global, 0);
				std::cout << std::endl;
			}
			else if ("$exit" == command) {
				break;
			}
			else if ("$M" == command) {
				MStyleTest(&global);  // for windows!
				system("cls"); // for windows!

				gotoxy(0, 0);
				setcolor(7, 0);

				std::cout << ">> : $M end" << std::endl;
			}
			else if (wiz::String::startsWith(command, "$call"))
			{
				wiz::load_data::UserType test;

				try {
					if (wiz::load_data::LoadData::LoadDataFromString(command, test))
					{
						try {
							wiz::load_data:: UserType ut = global;
							const std::string id = test.GetItemList(0).Get(0);
							const std::string result = excute_module("Main = { $call = { id = " + id + "} }", &ut, ExcuteData(), 1);

							global = std::move(ut);
							std::cout << std::endl << "excute result is : " << result << std::endl;
						}
						catch (...) // any exception..
						{
							std::cout << ">> : $call id or excute module error" << std::endl;
						}
					}
					else {
						std::cout << ">> : $call Error" << std::endl;
					}
				}
				catch (...) {
					std::cout << ">> : $call load data from string error" << std::endl;
				}
			}
			else if (wiz::String::startsWith(command, "$load"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					try {
						const std::string name = test.GetItemList(0).Get(0);
						const std::string result = wiz::String::substring(name, 1, name.size() - 2);

						if (wiz::load_data::LoadData::LoadDataFromFile(result, global)) {}
						else {
							std::cout << ">> : load data from file error" << std::endl;
						}
					}
					catch (...) // any exception..
					{
						std::cout << ">> : load error" << std::endl;
					}
				}
				else {
					std::cout << ">> : $load syntax Error" << std::endl;
				}
			}
			else if (wiz::String::startsWith(command, "$save_event_only"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					std::ofstream outFile;

					try {
						const std::string name = test.GetItemList(0).Get(0);
						const std::string result = wiz::String::substring(name, 1, name.size() - 2);

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
						std::cout << ">> : $save_event_only error" << std::endl;
					}
				}
				else {
					std::cout << ">> : $save_event_only syntax Error" << std::endl;
				}
			}

			else if (wiz::String::startsWith(command, "$save_data_only"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					std::ofstream outFile;

					try {
						const std::string name = test.GetItemList(0).Get(0);
						const std::string result = wiz::String::substring(name, 1, name.size() - 2);

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
						std::cout << ">> : $save_data_only error" << std::endl;
					}
				}
			}
			else if (wiz::String::startsWith(command, "$save"))
			{
				wiz::load_data::UserType test;
				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					const std::string name = test.GetItemList(0).Get(0);
					const std::string result = wiz::String::substring(name, 1, name.size() - 2);

					if (wiz::load_data::LoadData::SaveWizDB(global, result, "1")) {

					}
					else {
						std::cout << ">> : $save error" << std::endl;
					}
				}
				else {
					std::cout << ">> : $save syntax Error" << std::endl;
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
					std::cout << ">> : Data Added!" << std::endl;
				}
				else {
					std::cout << ">> : Error : loaddata from string " << std::endl;
				}
				command = "";
				totalCommand = "";
			}
			else {
				if (chk_brace[0] == 1) {
					std::cout << ">> : Error in command, reset command" << std::endl;
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

