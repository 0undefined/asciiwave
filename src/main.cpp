#include <ncurses.h>
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

struct vector2d {
    int x, y;
};

int main() {

    srand(time(NULL));

    initscr(); noecho(); cbreak(); nodelay(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLUE,   COLOR_BLACK);
    init_pair(2, COLOR_WHITE,  COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_CYAN,   COLOR_BLACK);

    //setlocale(LC_ALL, "");
    int wave_size = 5;
    float wave_height = 1.4;
    float wave_width  = 4.f;

    int sun_r = 6;
    vector2d sun_pos = {COLS - 2*sun_r, 2*6};

    int seagull = 0;
    const size_t seagulls = 12;

    vector2d seagul_pos[seagulls];

    seagul_pos[0]  = {0, -1};
    seagul_pos[1]  = {2,  0};
    seagul_pos[2]  = {4,  0};
    seagul_pos[3]  = {7,  0};
    seagul_pos[4]  = {9,  1};
    seagul_pos[5]  = {11, 1};
    seagul_pos[6]  = {5,  1};
    seagul_pos[7]  = {7,  2};
    seagul_pos[8]  = {6,  2};
    seagul_pos[9]  = {4,  2};
    seagul_pos[10] = {7,  2};
    seagul_pos[11] = {8,  2};

    time_t start  = time(NULL);
    time_t passed = start;

    int i = 0;

    while(passed - start < 30) {

        // Sun
        attron(COLOR_PAIR(3));
        if(i % 48 == 0) {
            for(int y = sun_pos.y - sun_r; y < sun_pos.y + sun_r; y++)
                for(int x = sun_pos.x - sun_r; x < sun_pos.x + sun_r; x++)
                    mvprintw(y, x, " ");
            sun_pos.y++;
        }
        for(int y = sun_pos.y - sun_r; y < sun_pos.y + sun_r; y++) {
            for(int x = sun_pos.x - sun_r; x < sun_pos.x + sun_r; x++) {
                int a = (x - sun_pos.x) * (x - sun_pos.x);
                int b = (y - sun_pos.y) * (y - sun_pos.y);
                if(a + b - (sun_r*sun_r) < 1)
                    mvprintw(y, x, "#");

            }
        }

        // Seagulls
        attron(COLOR_PAIR(2));
        if(i % 3 == 0) {
            for(unsigned s = 0; s < seagulls; s++) {
                mvprintw(seagul_pos[s].y + 2, seagul_pos[s].x + seagull, " ");
                mvprintw(seagul_pos[s].y + 2, seagul_pos[s].x + seagull + 1, "%c", (s+seagull) % 2 ? 'M' : 'W');
            }

            seagull++;
            seagull %= COLS;
        }

        // Water
        attron(COLOR_PAIR(1));
        for(int y = LINES/2 - wave_size; y < LINES; y++) {
            for(int x = 0; x < COLS; x++) {

                // Do the wave!
                int wave = wave_height * cos((float)i + ((float)x / wave_width));
                if(wave + y > LINES/2) {
                    if(rand() % 50 == 0) {
                        attron(COLOR_PAIR(4));
                        mvprintw(y, x, "~");
                        attron(COLOR_PAIR(1));
                    } else
                    mvprintw(y, x, "~");
                } else {
                    mvprintw(y, x, " ");
                }
            }
        }

        passed = time(NULL);
        attron(COLOR_PAIR(2));
        mvprintw(0, 0, "%d", i);
        refresh();
        i++;
        usleep(100000);
        int ch = getch();
        if(ch != ERR) {
            ungetch(ch);
            break;
        }
    }

    endwin();
    return 0;
}
