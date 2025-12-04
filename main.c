#include "inc/graphics.h"
#include "inc/math.h"
#include "inc/Sleep.h"
#include "inc/io.h"
#include "inc/PIT.h"
#include "inc/rand.h"

#include "inc/defines/game_defines.h"
#include "inc/defines/scancodes.h"

void draw_moon() {
    int cx = SCREEN_WIDTH - 30;
    int cy = 40;
    int r  = 15;

    for (int y = -r; y <= r; y++) {
        int dx = (int)sqrt(r * r - y * y);
        for (int x = cx - dx; x <= cx + dx; x++) {
            SetPixel(x, cy + y, 15);
        }
    }
}

void bg() {
    for (int i = 0; i < STARS; i++) {
        SetPixel(game.stx[i], game.sty[i], 0);
    }

    for (int i = 0; i < STARS; i++) {
        game.stx[i] -= 1;
        if (game.stx[i] < 0) {
            game.stx[i] = SCREEN_WIDTH - 1;
            game.sty[i] = rand() % (SCREEN_HEIGHT / 2) + 20;
        }
        SetPixel(game.stx[i], game.sty[i], 15);
    }

    draw_moon();
}

void RGB_sphere() { 
    int rectH = SCREEN_HEIGHT * 0.9; 
    int rectW = SCREEN_WIDTH * 1;   
    int startY = SCREEN_HEIGHT - rectH + 29;

    int cx = SCREEN_WIDTH / 2; 
    int cy = SCREEN_HEIGHT / 2;

    for (int x = 0; x < rectW; x++) {
      for (int y = startY; y < startY + rectH && y < SCREEN_HEIGHT; y++) {
          SetPixel(x, y, 0);
      }
    }

    for (double theta = 0; theta <= PI; theta += 0.15) {
          for (double phi = 0; phi <= TAU; phi += 0.15) {
              double x = R * sin(wrap_angle(theta)) * cos(wrap_angle(phi));
              double y = R * cos(wrap_angle(theta));
              double z = R * sin(wrap_angle(theta)) * sin(wrap_angle(phi));

              double cosY = cos(wrap_angle(game.an_y)), sinY = sin(wrap_angle(game.an_y));
              double cosX = cos(wrap_angle(game.an_x)), sinX = sin(wrap_angle(game.an_x));

              double x2 =  x * cosY + z * sinY;
              double z2 = -x * sinY + z * cosY;
              double y2 =  y * cosX - z2 * sinX;
              z2        =  y * sinX + z2 * cosX;

              int sx = (int)(cx + x2);
              int sy = (int)(cy + y2);

              if (sx >= 0 && sx < SCREEN_WIDTH && sy >= 0 && sy < SCREEN_HEIGHT) {
                    double intensity = (y / R + 1.0) / 2.0;
                    int col = 1 + (int)(intensity * 14);
                    SetPixel(sx, sy, col);
              } 
          }
    }

    game.an_y += 0.05;
    game.an_x += 0.03;
}

void draw_msg() {
    kprintf("DINO GAME MADE BY KILLWINX!", 0, 8, 8, 2);


    if ((game.frame / 2) % 2 == 0) {
        kprintf("CLICK ENTER TO PLAY", 0, 8, 19, 4); 
    } else {
        kprintf("                        ", 0, 8, 19, 0); 
    }
}


void main_screen() {
    while (1) {
        draw_msg();
        RGB_sphere();
        Sleep(80);

        game.frame++;

        while (inb(KEYBOARD_STATUS) & 1) {
            int sc = inb(KEYBOARD_DATA);
            if (sc == ENTER) {    
                clear_s();       
                return;
            }
        }
    }
}

void clear_s() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            SetPixel(x, y, 0);
        }
    }
}

void clear() {
    for (int i = 0; i < dino.h; i++) {
        for (int j = 0; j < dino.w; j++) {
            SetPixel(dino.x + i, (int)dino.y + j, 0);
        }
    }
}

void clear_obj() {
    for (int n = 0; n < OBJ; n++) {
        if (!obj[n].active) continue;
        for (int i = 0; i < obj[n].h; i++) {
            for (int j = 0; j < obj[n].w; j++) {
                SetPixel(obj[n].x + j, obj[n].y - i, 0);
            }
        }
    }
}

void setup() {
    dino.x = 20;
    dino.y = 120.0f;
    dino.w = 16;
    dino.h = 16;
    dino.dy = 0.0f;
    dino.jump = 0;

    game.speed = 12;

    for (int i = 0; i < 10; i++) {
        obj[i].x = 200 + i * 250;
        obj[i].y = 135;
        obj[i].w = 10;
        obj[i].h = 9;
        if(i < 3) {
            obj[i].active = 1;
        } else {
            obj[i].active = 0;
            obj[i].x = -100;
        }
        obj[i].passed = 0;
    }

    pit_init(1000);
    srand(timer_ticks);

    game.score = 0;
    game.over = 0;

    game.gx = 0;
    game.gy = 136;
    game.gw = 0;
    game.gh = 4;

    for (int i = 0; i < STARS; i++) {
        game.stx[i] = rand() % SCREEN_WIDTH;
        game.sty[i] = rand() % (SCREEN_HEIGHT / 2) + 20;
    }
}

void draw_player() {
    if (dino.x < 0) dino.x = 0;
    if (dino.x + dino.w >= SCREEN_WIDTH) dino.x = SCREEN_WIDTH - dino.w;
    if (dino.y < 0) dino.y = 0;
    if (dino.y + dino.h >= SCREEN_HEIGHT) dino.y = SCREEN_HEIGHT - dino.h;

    for (int i = 0; i < dino.h; i++) {
        for (int j = 0; j < dino.w; j++) {
            SetPixel(dino.x + i, (int)dino.y + j, COLOR_PLAYER);
        }
    }
}

void draw_ground() {
    for (int y = 0; y < game.gh; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            SetPixel(x, game.gy + y, COLOR_GROUND);
        }
    }
}

void draw_obj() {
    for (int n = 0; n < OBJ; n++) {
        if (!obj[n].active) continue;
        int x = obj[n].x;
        int y = obj[n].y;
        int size = obj[n].h;
        for (int j = 0; j < size; j++) {
            int start = x - (size - j - 1);
            int end = x + (size - j - 1);
            int draw_y = y - j;
            for (int i = start; i <= end; i++) {
                if (i >= 0 && i < SCREEN_WIDTH && draw_y >= 0 && draw_y < SCREEN_HEIGHT)
                    SetPixel(i, draw_y, COLOR_TRIG);
            }
        }
    }
}

int check_col() {
    for (int n = 0; n < OBJ; n++) {
        if (!obj[n].active) continue;
        if (dino.x < obj[n].x + obj[n].w &&
            dino.x + dino.w > obj[n].x &&
            (int)dino.y < obj[n].y + obj[n].h &&
            (int)dino.y + dino.h > obj[n].y) {
            return 1;
        }
    }
    return 0;
}

void draw_score() {
    kprintf("SCORE: %d", game.score, 10, 10, 15);
}

void physics() {
    dino.y += dino.dy;
    dino.dy += 0.3f;

    if (dino.y >= 120.0f) {
        dino.y = 120.0f;
        dino.dy = 0.0f;
        dino.jump = 0;
    }

    for (int n = 0; n < OBJ; n++) {
        if (!obj[n].active) continue;

        obj[n].x -= 2;

        if (obj[n].x + obj[n].w < 0) {
            int min_distance = 250;
            int max_extra = 100;
            int farthest = 0;
            for (int k = 0; k < OBJ; k++) {
                if (k == n) continue;
                if (obj[k].x > farthest) farthest = obj[k].x;
            }

            obj[n].x = farthest + min_distance + rand() % max_extra;
            obj[n].y = 135;
            obj[n].h = 9;
            obj[n].w = 10;
            obj[n].active = 1;
            obj[n].passed = 0;

            if (rand() % 2 == 0) {
                for (int m = 0; m < OBJ; m++) {
                    if (m == n) continue;
                    if (!obj[m].active) {
                        obj[m].x = obj[n].x + obj[n].w + 2;
                        obj[m].y = 135;
                        obj[m].h = 5 + rand() % 3;
                        obj[m].w = obj[m].h;
                        obj[m].active = 1;
                        obj[m].passed = 0;
                        break;
                    }
                }
            }
        }

        if (!obj[n].passed && dino.x > obj[n].x + obj[n].w) {
            game.score++;
            game.high++;
            obj[n].passed = 1;
        }
    }

    if (check_col()) {
        game.over++;
    }
}

void keyboard() {
    while (inb(KEYBOARD_STATUS) & 1) {
        int sc = inb(KEYBOARD_DATA);
        if (sc == SPACE_KEY && dino.jump == 0) {
            dino.dy = -5.0f;
            dino.jump = 1;
        }
    }
}

void easter_egg_key() {
    int scan = inb(KEYBOARD_DATA);
    if(scan == backs) {
        game.speed++;
        game.score += 1;
        dino.dy += 0.10f;
    }
}

void easter_egg() { 
    for (int frame = 0; frame < SCREEN_HEIGHT; frame++) {
        clear_s();
        for (int n = 0; n < OBJ; n++) {
            if (!obj[n].active) continue;
            obj[n].y -= 2;
            if (obj[n].y < 0) obj[n].y = SCREEN_HEIGHT;
            int x = obj[n].x;
            int y = obj[n].y;
            int size = obj[n].h;
            for (int j = 0; j < size; j++) {
                int start = x - (size - j - 1);
                int end = x + (size - j - 1);
                int draw_y = y - j;
                for (int i = start; i <= end; i++) {
                    if (i >= 0 && i < SCREEN_WIDTH && draw_y >= 0 && draw_y < SCREEN_HEIGHT)
                        SetPixel(i, draw_y, 4);
                }
            }
        }

        dino.y -= 3;
        if (dino.y < 0) dino.y = SCREEN_HEIGHT;

        for (int i = 0; i < dino.h; i++) {
            for (int j = 0; j < dino.w; j++) {
                SetPixel(dino.x + i, (int)dino.y + j, COLOR_PLAYER);
            }
        }

        for (int y = 0; y < game.gh; y++) {
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                int gy = game.gy - frame + y;
                if (gy >= 0 && gy < SCREEN_HEIGHT)
                    SetPixel(x, gy, COLOR_GROUND);
            }
        }

        const char* msg = "!!KERNEL PANIC KERNEL PANIC DEVIL!!!";
        int len = strlen(msg);

        int x = (SCREEN_WIDTH - (len * FONT_WIDTH)) / 2;
        int y = SCREEN_HEIGHT / 2;
        game.col_timer += 3;

        kprintf(msg, 0, x, y, game.col_timer);

        SetPixel(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 4);

        Sleep(10);
    }
}

void main() {
    while (!game.over) {
        clear();
        clear_obj();
        bg();     
        physics();
        draw_ground();
        draw_obj();
        draw_player();
        draw_score();
        keyboard();
        easter_egg_key();
        Sleep(game.speed);

        if(game.score >= 666) {
            easter_egg();
        }
    }

    const char* msg = "GAME OVER!";
    int len = strlen(msg);

    int x = (SCREEN_WIDTH - (len * FONT_WIDTH)) / 2;
    int y = SCREEN_HEIGHT / 2;

    int sc = inb(KEYBOARD_DATA);

    kprintf(msg, 0, x, y, 4);
    kprintf("HIGH: %d", game.high, x, y + 9, 3);
}

void kmain() {
    setup();
    main_screen();

    main();
}