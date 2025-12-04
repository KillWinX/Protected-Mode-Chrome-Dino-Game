#define COLOR_PLAYER  4  
#define COLOR_GROUND 15
#define COLOR_TRIG 3
#define OBJ 11
#define KEYBOARD_STATUS 0x64 
#define KEYBOARD_DATA 0x60
#define STARS 80
#define NUM_ROCKS 20
#define R 50

typedef struct {
    int x;
    float y;
    int w;
    int h;
    float dy;     
    int jump;  
} Player;

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int active;
    int passed;
    int flying;   
} Obj;

typedef struct {
    int score;
    int high;
    int over;
    int speed;
    int gx;
    int gy;
    int gw;
    int gh;
    int sx;
    int sy;
    double r;
    int stx[STARS];
    int sty[STARS];
    int col_timer;
    double an_x;
    double an_y;
    int frame;
} Game;

Player dino;
Game game;

void retry(); 


Obj obj[OBJ];

int rock_x[NUM_ROCKS];
int rock_y[NUM_ROCKS];
int rock_r[NUM_ROCKS];
int rocks_initialized = 0;