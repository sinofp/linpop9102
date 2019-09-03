#include "helper.h"
#include "msg.h"
#include <cstring>
#include <map>
#include <string>
#include <sys/socket.h>

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

void delete_user_fd(char name[20])
{
    it = user_map.find(name);
    user_map.erase(it);
}

void broadcast_loginout(char name[20], MessageType action)
{
    Message msg;
    memset(&msg, 0, sizeof(msg));
    msg.msgType = action; // 客户端得到exit是有人下线了，服务端得到exit是发来的人请求下线。login同理
    strcpy(msg.sendName, name);
    for (auto& user : user_map) {
        send(user.second, &msg, sizeof(msg), 0);
    }
}