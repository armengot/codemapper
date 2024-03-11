#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>
#include <sstream>
#include <gvc.h>

#define DEBUG_REDTXT "\x1B[31m"
#define DEBUG_GRNTXT "\x1B[32m"
#define DEBUG_YELTXT "\x1B[33m"
#define DEBUG_BLUTXT "\x1B[34m"
#define DEBUG_MGTTXT "\x1B[35m"
#define DEBUG_CYNTXT "\x1B[36m"
#define DEBUG_BLDTXT "\x1B[1m"
#define DEBUG_RESTXT "\x1B[0m"

extern char global_join_char;

using namespace std;

string lastname(string path, char splitchar);
string lastwo(std::string path, char splitchar, char joinchar);
string firstname(string path, char splitchar);
void erasestring(vector<string>& old, const string& key);
bool charin(char c, const std::string& str);
void cm_render(const string& input, std::stringstream& output);

#endif