#ifndef TOOLS_H
#define TOOLS_H

/* standard external libraries */
#include <string>
#include <vector>
#include <sstream>

/* graphviz haders */
#include <gvc.h>
#include <graphviz_version.h>

#define DEBUG_REDTXT "\x1B[31m"
#define DEBUG_GRNTXT "\x1B[32m"
#define DEBUG_YELTXT "\x1B[33m"
#define DEBUG_BLUTXT "\x1B[34m"
#define DEBUG_MGTTXT "\x1B[35m"
#define DEBUG_CYNTXT "\x1B[36m"
#define DEBUG_BLDTXT "\x1B[1m"
#define DEBUG_RESTXT "\x1B[0m"

extern char global_join_char;

enum CM_OUTPUT_OUTPUT_MODES
{
    CM_OUTPUT_SVG,
    CM_OUTPUT_PNG,
    CM_OUTPUT_DOT
};

using namespace std;

string lastname(string path, char splitchar);
string lastwo(std::string path, char splitchar, char joinchar);
string firstname(string path, char splitchar);
void erasestring(vector<string>& old, const string& key);
bool charin(char c, const std::string& str);
//void cm_render(const string& input, std::stringstream& output,CM_OUTPUT_OUTPUT_MODES mode);
void cm_render(const string& input, std::string& output, CM_OUTPUT_OUTPUT_MODES mode);

#endif