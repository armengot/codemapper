/* external headers */
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <gvc.h>
#include <iostream>

/* codemapper project headers */
#include <tools.h>

using namespace std;

const char CM_GLOBAL_JOIN_CHAR = '_';
const char CM_SYS_SPLITER_CHAR = '/';

void rechar(std::string& str, char oldc, char newc)
{
    for (size_t i = 0; i < str.length(); ++i) 
    {
        if (str[i] == oldc) 
        {
            str[i] = newc;
        }
    }
}

bool charin(char c, const std::string& str) 
{
    for (char ch : str) 
    {
        if (ch == c) 
        {
            return true;
        }
    }
    return false;
}

string lastname(string path, char splitchar)
{
    vector<string> splitter;
    istringstream ss(path);
    string last;

    while (getline(ss, last, splitchar))
    {
        splitter.push_back(last);
    }
    if (!splitter.empty())
    {
        return splitter.back();
    }
    else
        return("");
}

string lastwo(std::string path, char splitchar, char joinchar) 
{
    vector<string> splitter;
    istringstream ss(path);
    string last;

    while (getline(ss, last, splitchar)) 
    {
        splitter.push_back(last);
    }
    if (splitter.size() >= 2) 
    {
        return splitter[splitter.size() - 2] + joinchar + splitter.back();
    } 
    else if (!splitter.empty()) 
    {
        return splitter.back();
    } 
    else 
    {
        return "";
    }
}

string firstname(string path, char splitchar)
{
    vector<string> splitter;
    istringstream ss(path);
    string last;

    while (getline(ss, last, splitchar))
    {
        splitter.push_back(last);
    }
    if (!splitter.empty())
    {
        if (splitter.size()>1)
        {
            return(splitter[splitter.size()-2]);
        }
    }
    return "";
}

void erasestring(vector<string>& old, const string& key) 
{    
    auto it = find(old.begin(),old.end(),key);
    
    if (it != old.end()) 
    {
        old.erase(it);        
    } 
}

/* graphviz library real connection */
/* same example as simple.c from graphviz docs */
void cm_render(const string& input, std::string& output, CM_OUTPUT_MODES mode)
{
    GVC_t *gvc;
    Agraph_t *g;
    FILE *fp;
    char* buffer;
    unsigned int len;

    std::cerr << "cm_render: using graphviz under https://graphviz.org/license/ terms" << std::endl;
    gvc = gvContext();
    fp = fmemopen((void*)input.c_str(), input.length(), "r");
    g = agread(fp, 0);
    gvLayout(gvc, g, "dot");
    
    if (mode == CM_OUTPUT_SVG)
        gvRenderData(gvc, g, "svg", &buffer, &len); 
    else if (mode == CM_OUTPUT_PNG)
        gvRenderData(gvc, g, "png", &buffer, &len);
    
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    fclose(fp);
    
    if (buffer != NULL) 
    {
        output.assign(buffer, len);
        gvFreeRenderData(buffer);
    }    
}

void cm_dashclean(std::string& str) 
{
    int c=0;
    for (size_t i = 0; i < str.length() - 1; ++i) 
    {
        if (str[i] == '-' && str[i + 1] != '>') 
        {
            c = c + 1;
            str.erase(i, 1);  // Borra el guion
        }
    }
    std::cerr << "cm_dashclean: " << c << " slashes removed" << std::endl;
}
