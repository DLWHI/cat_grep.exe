#ifndef SRC_GREP_S21_LIST_H_
#define SRC_GREP_S21_LIST_H_

void list_print();
void list_destroy();
int list_size();
void list_push(void* data);
void* list_pop();
void* list_index(int index);
int list_exists(unsigned char* byte_seq, int len);
#endif  // SRC_GREP_S21_LIST_H_
