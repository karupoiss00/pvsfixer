#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

void FindFiles(vector<string>* list, string ext);
void InsertCopyright(string filename, string copyrightStr);
void CutCopyright(string filename, string copyrightStr);
string ReadFile(const string &fileName);

int main(int argc, char** argv)
{
	string copyright = "// This is a personal academic project. Dear PVS-Studio, please check it.\n\n// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com";
	vector<string> filesList;
	FindFiles(&filesList, ".cpp");
	FindFiles(&filesList, ".h");

	if (argc < 2)
	{
		cout << "Wrong arguments!";
		return 1;
	}

	if (argv[1][0] == '1')
	{
		for (vector<string>::iterator it = filesList.begin(); it != filesList.end(); ++it)
		{
			InsertCopyright(*it, copyright);
		}
	}
	else if (argv[1][0] == '0')
	{
		for (vector<string>::iterator it = filesList.begin(); it != filesList.end(); ++it)
		{
			CutCopyright(*it, copyright);
		}
	}
	else
	{
		cout << "Wrong arguments!";
		return 1;
	}

	cout << "Success!";
		
	return 0;
}

void FindFiles(vector<string>* list, string ext)
{
	for (auto &p : filesystem::recursive_directory_iterator(fs::current_path().generic_string()))
	{
		if (!fs::is_regular_file(p.status()))
		{
			continue;
		}
			

		std::string name(p.path().string());

		bool match = !name.compare(name.size() - ext.size(), ext.size(), ext);

		if (!match)
		{
			continue;
		}
			
		list->push_back(name);
	}
}

void InsertCopyright(string filename, string copyrightStr)
{
	string temp = ReadFile(filename);
	fstream file(filename);
	file.seekg(file._Seekbeg);

	copyrightStr += "\n\n";

	temp = copyrightStr + temp;

	file.write(temp.c_str(), temp.length());
}

void CutCopyright(string filename, string copyrightStr)
{
	string temp = ReadFile(filename);
	string out;

	fstream file(filename);

	copyrightStr += "\n\n";

	for (size_t i = copyrightStr.length(); i < temp.length(); i++)
	{
		out += temp[i];
	}

	file.write(out.c_str(), out.length());
}

string ReadFile(const string &fileName) 
{
	ifstream f(fileName);
	stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}