/* external headers */
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <gvc.h>

/* codemapper project headers */
#include <tools.h>

using namespace std;

char global_join_char = '_';

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
/*
void cm_render(const string& input, std::stringstream& output,CM_OUTPUT_OUTPUT_MODES mode)
{
    GVC_t *gvc;
    Agraph_t *g;
    FILE *fp;
    char* buffer;
    unsigned int len;

    gvc = gvContext();
    fp = fmemopen((void*)input.c_str(), input.length(), "r");
    g = agread(fp, 0);
    gvLayout(gvc, g, "dot");
    //gvRender(gvc, g, "svg", stdout);
    if (mode==CM_OUTPUT_SVG)
        gvRenderData(gvc, g, "svg", &buffer, &len); 
    else
        gvRenderData(gvc, g, "png", &buffer, &len);
         
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    fclose(fp);
    //output << stdout;
    if (buffer != NULL) 
    {
        output << buffer;
        gvFreeRenderData(buffer);
    }    
}
*/

void cm_render(const string& input, std::string& output, CM_OUTPUT_OUTPUT_MODES mode)
{
    GVC_t *gvc;
    Agraph_t *g;
    FILE *fp;
    char* buffer;
    unsigned int len;

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
