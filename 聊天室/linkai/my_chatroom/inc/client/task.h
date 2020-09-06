#ifndef _TASK_H_
#define _TASK_H_

extern char account[20];

extern char uid[10];

extern struct packet ret_pack_buf;

extern int pchat_ing;

extern int gchat_ing;

extern int chat_uid;

extern int chat_gid;

void register_user(int connect_fd);

void login_user(int connect_fd);

void reset_pwd(int connect_fd);

void private_chat(int connect_fd);

void add_friend_req(int connect_fd);

void add_friend_res(int connect_fd);

void del_friend(int connect_fd);

void inq_friend(int connect_fd);

void list_friend(int connect_fd);

void shield_friend(int connect_fd);

void unshield_friend(int connect_fd);

void private_record(int connect_fd);

void group_record(int connect_fd);

void group_chat(int connect_fd);

void create_group(int connect_fd);

void dismiss_group(int connect_fd);

void apply_group_req(int connect_fd);

void apply_group_res(int connect_fd);

void quit_group(int connect_fd);

void list_group_all(int connect_fd);

void list_group_mine(int connect_fd);

void inq_group(int connect_fd);

void send_file_req(int connect_fd);

void send_file_res(int connect_fd);

void list_user(int connect_fd);

void set_admin(int connect_fd);

void quit_user(int connect_fd);

void put_ret_in_buf(struct packet ret_pack);

void main_menu(int connect_fd);

void chat_menu(int connect_fd);

void handle_message(int connect_fd);

void private_chat_loop(char *prefix, int connect_fd);

void group_chat_loop(char *prefix, int connect_fd);

void private_chat_res(int connect_fd);

void group_chat_res(int connect_fd);

void update_user_info(int connect_fd);

void update_group_info(int connect_fd);

#endif
