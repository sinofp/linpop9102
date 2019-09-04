#ifndef RADIOACTIVITY_HELPER_H
#define RADIOACTIVITY_HELPER_H

#include "msg.h"
extern Message message;

//https://isocpp.org/wiki/faq/mixing-c-and-cpp#include-c-hdrs-personal
#ifdef __cplusplus
extern "C" {
#endif

int locate_user_fd(char name[20]);
void locate_user_name(int fd);
void insert_user_fd(char name[20], int fd);
void delete_user_fd(char name[20]);
void broadcast_loginout();
void send_current_online(int fd);
int user_already_online();

#ifdef __cplusplus
}
#endif
#endif //RADIOACTIVITY_HELPER_H
