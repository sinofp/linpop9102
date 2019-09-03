#include "helper.h"
#include <cstring>
#include <map>
#include <string>
#include <sys/socket.h>

std::map<std::string, int> user_map;
std::map<std::string, int>::iterator it;

int locate_user_fd(char name[20])
{
    it = user_map.find(name);
    // if it==map.end -> no user
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

void broadcast_loginout()
{
    //    Message msg;
    //    memset(&msg, 0, sizeof(msg));
    //    msg.msgType = action; // 客户端得到exit是有人下线了，服务端得到exit是发来的人请求下线。login同理
    //    strcpy(msg.sendName, name);
    //    for (auto& user : user_map) {
    //        send(user.second, &msg, sizeof(msg), 0);
    //    }
    for (auto& user : user_map) {
        send(user.second, &message, sizeof(message), 0);
    }
}

void send_current_online(int fd)
{
    memset(&message.content, 0, sizeof(message.content));
    // 第一个上线的话，user_map为空，不发这个消息
    for (auto& user : user_map) {
        strcat(message.content, user.first.c_str());
        strcat(message.content, "|");
    }
    puts(message.content);
    message.msgType = VIEW_USER_LIST;
    send(fd, &message, sizeof(message), 0);
}