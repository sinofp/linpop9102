#ifndef MSG_H
#define MSG_H

#include <stdint.h>

#define CONTENT_SIZE 2048
#define FILE_NAME_MAX 2048
enum MessageType {
    REGISTER = 1, /*注册请求*/
<<<<<<< HEAD
    LOGIN=0, /*登陆请求*/
    HELP, /*帮助请求*/
    EXIT, /*退出请求*/
    VIEW_USER_LIST, /*查看在线列表*/
    GROUP_CHAT, /*群聊请求*/
    PERSONAL_CHAT, /*私聊请求*/
    VIEW_RECORDS, /*查看聊天记录请求*/
    RESULT, /*结果消息类型*/
    UNKONWN, /*未知请求类型*/
    REPLY, // 服务器返回的
    FL, //提醒接受文件
    FL_CONTENT //文件内容
=======
    LOGIN = 0, /*登陆请求*/
    HELP = 9, /*帮助请求*/
    EXIT = 10, /*退出请求*/
    VIEW_USER_LIST = 2, /*查看在线列表*/
    GROUP_CHAT = 11, /*群聊请求*/
    PERSONAL_CHAT = 3, /*私聊请求*/
    VIEW_RECORDS = 4, /*查看聊天记录请求*/
    RESULT = 12, /*结果消好友名字息类型*/
    UNKONWN = 13, /*未知请求类型*/
    REPLY = 5, // 服务器返回的
    FL = 6, //提醒接受文件
    FL_CONTENT = 7, //文件内容
    ADD_FRIEND, //增加好友
    REMOVE_FRIEND //删除好友
>>>>>>> 2b8b2d44787b1d4b440d1de448e8072d467842ef
};

/*定义操作结果 */
enum StateRet {
    EXCEED, //已达服务器链接上限
    SUCCESS = 14, //成功
    FAILED = 19, //失败
    DUPLICATEID = 17, //重复的用户名
    INVALID = 16, //不合法的用户名
    ID_NOT_EXIST = 18, //账号不存在
    WRONGPWD = 15, //密码错误
    ALREADY_ONLINE = 20, //已经在线
    ID_NOT_ONLINE = 21, //账号不在线
    ALL_NOT_ONLINE = 22, //无人在线
    MESSAGE_SELF = 23 //消息对象不能选择自己
};

// 设置结构体字节对齐，否则windows、linux可能不一样
// https://www.jianshu.com/p/d994731f658d
#pragma pack(push, 4)
/*定义服务器 -- 客户端 消息传送结构体*/
typedef struct _Message {
    char content[CONTENT_SIZE]; /*针对聊天类型的消息，填充该字段*/
    int8_t msgType; /*消息类型 即为MessageType中的值*/
    int8_t msgRet; /*针对操作结果类型的消息，填充该字段*/
    char sendName[20]; /*发送者名称*/
    char recvName[20]; /*接收者名称*/
    char msgTime[20]; /*消息发送时间*/
    char fileName[FILE_NAME_MAX]; /*发送的文件名*/
} Message;
#pragma pack(pop)

//有用么？接受直接用message，发送用的buf只是为了命令行选择登录还是私聊
#define BUF_SIZE sizeof(Message) / sizeof(char)
#endif // MSG_H
