typedef uint64_t MATRIX_MAP;

void matrix_scan(void);
void init_keyboard(void);

#define NOS_KEY_BUFFER_LEN 16

extern volatile int nos_key_in;
extern volatile int nos_key_out;

void nos_put_key(char key);
char nos_get_key(void);
void check_keys(void);

typedef int KEYCODE;

KEYCODE kb_getk(void);
KEYCODE kb_test(void);

extern KEYCODE kb_external_key;


void key_scan(void);
void drive_column(int column, int state);
  void drive_row(int row);



#if 0
int matrix_map[24] =
  {
    20, 6, 0, 0,    // 0
    0, 0, 7, 0,     // 4
    4, 0, 0, 0,     // 8
    5, 0, 0, 0,     // 12
    0, 0, 0, 0,     // 16
    0, 0, 0, 0,     // 20
  };

5
6
16
8
12
1
6
9
13
2
9
10
14
3
7
11
15
4
10
0
0
11
5
4



#endif
