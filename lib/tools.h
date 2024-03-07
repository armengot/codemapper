#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>
#include <sstream>

extern char global_join_char;

using namespace std;

string lastname(string path, char splitchar);
string lastwo(std::string path, char splitchar, char joinchar);
string firstname(string path, char splitchar);
void erasestring(vector<string>& old, const string& key);
bool charin(char c, const std::string& str);

#endif