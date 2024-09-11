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

