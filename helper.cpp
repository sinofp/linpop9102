#include "helper.h"
#include <map>
#include <string>

std::map<std::string, int> user_map;
std::map<std::string, int>::iterator it;

int locate_user_fd(char name[20])
{
    it = user_map.find(name);
    // if it==map.end -> nouser
    return it->second;
}

void insert_user_fd(char name[20], int fd)
{
    user_map[name] = fd;
    // 检查返回值？
}