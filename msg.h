#ifndef MSG_H
#define MSG_H
#define CONTENT_SIZE 70
#define FILE_NAME_MAX 100
enum MessageType {
    REGISTER = 1, /*注册请求*/
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
    FL_CONTENT, //文件内容
    ADD_FRIEND, //添加好友
    DELETE_FRIEND, //删除好友
    MOVE_FRIEND //变更好友分组
};

/*定义操作结果 */
enum StateRet {
    EXCEED, //已达服务器链接上限
    SUCCESS, //成功
    FAILED, //失败
    DUPLICATEID, //重复的用户名
    INVALID, //不合法的用户名
    ID_NOT_EXIST, //账号不存在
    WRONGPWD, //密码错误
    ALREADY_ONLINE, //已经在线
    ID_NOT_ONLINE, //账号不在线
    ALL_NOT_ONLINE, //无人在线
    MESSAGE_SELF //消息对象不能选择自己
};

// 设置结构体字节对齐，否则windows、linux可能不一样
#pragma pack(push, 4)
/*定义服务器 -- 客户端 消息传送结构体*/
typedef struct Message {
    char content[CONTENT_SIZE]; /*针对聊天类型的消息，填充该字段*/
    char sendName[20]; /*发送者名称*/
    char recvName[20]; /*接收者名称*/
    int msgType; /*消息类型 即为MessageType中的值*/
    int msgRet; /*针对操作结果类型的消息，填充该字段*/
    char msgTime[20]; /*消息发送时间*/
    char fileName[FILE_NAME_MAX]; /*发送的文件名*/
    int fileLength;
} Message;
#pragma pack(pop)
//有用么？接受直接用message，发送用的buf只是为了命令行选择登录还是私聊
#define BUF_SIZE sizeof(Message) / sizeof(char)

#endif //RADIOACTIVITY_REDAE_H
