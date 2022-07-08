# cat_grep.exe
Partial cat+grep utilities implementation

# General info
This project was made under educational program of School 21. It mimics almost full functionality of original UNIX `cat` and `grep` utilities. 
Code behaviour is based on original untilies that were on school PCs. This is seen when running `grep` with `-lc` flags, original grep on those pc was broken 
and did not supress `-c` output(which was always 1 somehow).
# Supported flags
`cat`: -benst  
`grep`: -eivclnhsfo  
`man cat/grep` to get more info on flags
# Built-in tests
### cat
Built in cat tests contains all combinations of 4 flags out of supported 5 (including empty flag ex. `cat -bet`, one flag is missing therefore empty) 
which is 360 tests. It tests functionality on output of Beethoven's 9th symphony Presto which includes extended ASCII characters. 
The tests run twice with different type of flag input(separated `-b -e -t` and combined `-bet`) so test count is doubled(720).
### grep
Built in grep tests contains all combinations of 4 flags out of supported(including empty). It runs two patterns `"D"` and `"D?"` on all these combinations 
which is roughly 6000 tests. It tests functionality on output of Beethoven's 9th symphony Presto which includes extended ASCII characters.
# Build
- `make all` - Builds cat and grep.
- `make s21_cat` - Builds cat.
- `make s21_grep` - Builds grep.
- `make test_cat` - Runs built-in cat test
- `make test_grep` - Runs built-in grep test
- `make clean` - Cleans up project dir
# TODO
-Make it crossplatform(ex. add CLRF support)
