#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

void FindFiles(vector<string>* list, string ext);
void LoadFilelist(vector<string>* fileslist, vector<string> extensions);
void InsertCopyright(string filename, string copyrightStr);
void CutCopyright(string filename, string copyrightStr);
void PrintEditingInfo(vector<string> list);

string ReadFile(const string &fileName);
string ParseCopyright();
vector<string> ParseExtensions();

int main(int argc, char** argv)
{
	vector<string> filesList;
	vector<string> extensionsList;
	string copyright;
	
	extensionsList = ParseExtensions();
	copyright = ParseCopyright();
	LoadFilelist(&filesList, extensionsList);

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

	
	PrintEditingInfo(filesList);

	return 0;
}

void LoadFilelist(vector<string>* fileslist, vector<string> extensions)
{
	for (vector<string>::iterator it = extensions.begin(); it != extensions.end(); ++it)
	{
		FindFiles(fileslist, *it);
	}
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

string ParseCopyright()
{
	string temp;
	string configFileStr = ReadFile("config.ini");

	for (size_t i = configFileStr.find_last_of("]") + 2; i < configFileStr.length(); i++)
	{
		temp += configFileStr[i];
	}
	return temp;
}

vector<string> ParseExtensions()
{
	string configFileStr = ReadFile("config.ini");
	string parsedExtensions;
	string curExtension;
	vector<string> temp;

	for (size_t i = configFileStr.find_first_of("]") + 2; i < configFileStr.find_last_of("["); i++)
	{
		parsedExtensions += configFileStr[i];
	}

	for (size_t i = 0; i < parsedExtensions.length(); i++)
	{
		if (parsedExtensions[i] != '\n')
		{
			curExtension += parsedExtensions[i];
		}
		else
		{
			temp.push_back(curExtension);
			curExtension = "";
		}
	}

	return temp;
}

void PrintEditingInfo(vector<string> list)
{
	cout << "Edited files:" << endl;
	for (vector<string>::iterator it = list.begin(); it != list.end(); ++it)
	{
		cout << *it << endl;
	}
	cout << "Success!";
}