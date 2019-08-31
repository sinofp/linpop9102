#ifndef RADIOACTIVITY_HELPER_H
#define RADIOACTIVITY_HELPER_H
//https://isocpp.org/wiki/faq/mixing-c-and-cpp#include-c-hdrs-personal
#ifdef __cplusplus
extern "C" {
#endif

int locate_user_fd(char name[20]);
void insert_user_fd(char name[20], int fd);

#ifdef __cplusplus
}
#endif
#endif //RADIOACTIVITY_HELPER_H
