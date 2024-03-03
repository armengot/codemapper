/* external sources */
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

/* codemapper sources */
#include <cm_graph.h>
#include <cm_lan.h>
#include <cm_clan.h>

namespace fs = std::filesystem;

cpp_language::cpp_language(const string& in_target) : language(in_target)
{
    src_ext = "cpp";
    lib_ext = "h";
    inc_key = "include";   
}




