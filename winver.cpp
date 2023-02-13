#include "nssm.h"

#include <stdio.h>
#include <regex> // C++11
#include <string> // ditto

bool get_win_version(DWORD* p_maj, DWORD* p_min, DWORD* p_bld) {

    // read output of `ver.exe`
    std::string response;
    FILE* child = popen("ver.exe", "r");
    char chunk[MAX_PATH];
    std::size_t chunk_sz;
    while(MAX_PATH == fread(chunk, MAX_PATH, 1, child)) {
        response.append(chunk, chunk + chunk_sz);
    }
    fclose(child);

    // parse out the version number (at least its first three fields)
    std::regex mmb_regex("(\\d+)\\.(\\d+)\\.(\\d+)(\\.\\d+)*", std::regex_constants::ECMAScript);
    std::smatch mmb_find;
    if(std::regex_search(response, mmb_find, mmb_regex)) {
        if(p_maj) *p_maj = atoi(mmb_find.str(1).c_str());
        if(p_min) *p_min = atoi(mmb_find.str(1).c_str());
        if(p_bld) *p_bld = atoi(mmb_find.str(1).c_str());
        return true;
    } else {
        return false;
    }
}
