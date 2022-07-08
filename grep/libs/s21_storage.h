#ifndef SRC_GREP_S21_STORAGE_H_
#define SRC_GREP_S21_STORAGE_H_

#define S_BEG 0
#define S_CUR 1
#define S_END 2

void storage_put(char* tail);
void storage_seek(int pos, long offs);
char* storage_get();
int storage_size();
void storage_destroy();
#endif  // SRC_GREP_S21_STORAGE_H_
