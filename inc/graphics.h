#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdarg.h>
#include "defines/font.h"

#define VIDEO_MEMORY ((unsigned char*)0xA0000)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define FONT_WIDTH   8
#define FONT_HEIGHT  8
#define FONT_SPACING 1

static inline void SetPixel(int x, int y, unsigned char color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    VIDEO_MEMORY[y * SCREEN_WIDTH + x] = color;
}

static inline void draw_char(const uint8_t font[8], int index, int x, int y, int color) {
    int x_off = x + index * (FONT_WIDTH + FONT_SPACING);

    for (int row = 0; row < FONT_HEIGHT; row++) {
        uint8_t row_data = font[row];
        for (int col = 0; col < FONT_WIDTH; col++) {
            if (row_data & (1 << col)) {
                SetPixel(x_off + col, y + row, color);
            }
        }
    }
}

static inline int char2int(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return 26 + (c - 'a');
    if (c == ' ') return SPACE;

    if (c >= '0' && c <= '9') return N0 + (c - '0');

    switch (c) {
        case '!': return EXCL;
        case '"': return QUOT;
        case '#': return HASH;
        case '$': return DOLLAR;
        case '%': return PERC;
        case '&': return AMP;
        case '\'': return APOST;
        case '(': return LPAREN;
        case ')': return RPAREN;
        case '*': return ASTER;
        case '+': return PLUS;
        case ',': return COMMA;
        case '-': return MINUS;
        case '.': return DOT;
        case '/': return SLASH;
        case ':': return COLON;
        case ';': return SEMI;
        case '<': return LT;
        case '=': return EQ;
        case '>': return GT;
        case '?': return QUEST;
        case '@': return AT;
        case '[': return LBRACK;
        case '\\': return BS;
        case ']': return RBRACK;
        case '^': return CARET;
        case '_': return UNDERSC;
        case '`': return GRAVE;
        case '{': return LBRACE;
        case '|': return PIPE;
        case '}': return RBRACE;
        case '~': return TILDE;
    }

    return SPACE;
}

static inline void itoa(int value, char* buffer) {
    char temp[12];
    int i = 0, j, is_negative = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (is_negative) temp[i++] = '-';

    for (j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}

static int prev_num_len = 0;
static int prev_num_x = 0;

static inline void kprintf(const char* str, int num, int x, int y, int color) {
    int cx = x;
    int cy = y;

    for (int i = 0; str[i]; i++) {
        if (str[i] == '%' && str[i + 1] == 'd') {
            char buf[12];
            itoa(num, buf);

            for (int px = 0; px < prev_num_len * FONT_WIDTH; px++) {
                for (int py = 0; py < FONT_HEIGHT; py++) {
                    SetPixel(prev_num_x + px, y + py, 0);
                }
            }

            prev_num_x = cx;
            int new_len = 0;
            for (int k = 0; buf[k]; k++) {
                int fi = char2int(buf[k]);
                draw_char(font[fi], 0, cx, y, color);
                cx += FONT_WIDTH;
                new_len++;
            }
            prev_num_len = new_len;
            i++;
        } else {
            int fi = char2int(str[i]);
            draw_char(font[fi], 0, cx, y, color);
            cx += FONT_WIDTH;
        }

        if (str[i] == '\n') {
            cx = x; 
            cy += 10;
            continue;
        }
    }
}

static inline int strlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

#endif // GRAPHICS_H
