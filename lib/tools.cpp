#include <string>
#include <vector>
#include <sstream>

using namespace std;

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