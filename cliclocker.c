/*
 * Thanks to xorg62 for tty-clock
 * And thanks to nbyouri <https://github.com/nbyouri> for the original cliclock
 */
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    bool running;
    int bg;
    struct {
        int color;
        long delay;
    } option;
    struct {
        int x, y, w, h;
    } geo;
    struct {
        unsigned int hour[2];
        unsigned int minute[2];
        unsigned int second[2];
    } date;

    struct tm *tm;
    time_t lt;

    WINDOW *framewin;

} cliclocker_t;

static void init(void);
static void update_hour(void);
static void draw_number(int n, int x, int y);
static void draw_clock(void);
static void clock_move(int x, int y);
static void signal_handler(int signal);

cliclocker_t *cliclocker;

static const bool number[][15] = {
    {1,1,1,1,0,1,1,0,1,1,0,1,1,1,1}, /* 0 */
    {0,0,1,0,0,1,0,0,1,0,0,1,0,0,1}, /* 1 */
    {1,1,1,0,0,1,1,1,1,1,0,0,1,1,1}, /* 2 */
    {1,1,1,0,0,1,1,1,1,0,0,1,1,1,1}, /* 3 */
    {1,0,1,1,0,1,1,1,1,0,0,1,0,0,1}, /* 4 */
    {1,1,1,1,0,0,1,1,1,0,0,1,1,1,1}, /* 5 */
    {1,1,1,1,0,0,1,1,1,1,0,1,1,1,1}, /* 6 */
    {1,1,1,0,0,1,0,0,1,0,0,1,0,0,1}, /* 7 */
    {1,1,1,1,0,1,1,1,1,1,0,1,1,1,1}, /* 8 */
    {1,1,1,1,0,1,1,1,1,0,0,1,1,1,1}, /* 9 */
};

static void init(void)
{
    struct sigaction sig;
    cliclocker->bg = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    start_color();
    curs_set(false);
    clear();
    if(use_default_colors() == OK)
        cliclocker->bg = -1;
    /* Init color pair */
    init_pair(0, cliclocker->bg, cliclocker->bg);
    init_pair(1, cliclocker->bg, cliclocker->option.color);
    init_pair(2, cliclocker->option.color, cliclocker->bg);

    refresh();

    /* Init signal handler */
    sig.sa_handler = signal_handler;
    sig.sa_flags   = 0;
    sigaction(SIGWINCH, &sig, NULL);
    sigaction(SIGTERM,  &sig, NULL);

    /* Init global struct */ 
    cliclocker->running = true;
    if(!cliclocker->geo.x)
        cliclocker->geo.x = 0;
    if(!cliclocker->geo.y)
        cliclocker->geo.y = 0;
    cliclocker->geo.w = 54;
    cliclocker->geo.h = 7;
    cliclocker->tm = localtime(&(cliclocker->lt));
    cliclocker->lt = time(NULL);
    update_hour();

    /* Create clock win */ 
    cliclocker->framewin = newwin(cliclocker->geo.h,
            cliclocker->geo.w,
            cliclocker->geo.x,
            cliclocker->geo.y);
    clock_move((LINES / 2 - (cliclocker->geo.h / 2)),
            (COLS  / 2 - (cliclocker->geo.w / 2)));
    wborder(cliclocker->framewin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    nodelay(stdscr, true);
    wrefresh(cliclocker->framewin);

    return;
}

static void signal_handler(int signal)
{
    switch(signal) {
        case SIGWINCH: /* window resize signal */
            endwin();
            init();
            break;
        case SIGTERM:
            cliclocker->running = false; /* interruption signal */
    }
    return;
}

static void update_hour(void)
{
    int ihour;
    cliclocker->tm = localtime(&(cliclocker->lt));
    cliclocker->lt = time(NULL);
    ihour = cliclocker->tm->tm_hour;
    /* Set hour */
    cliclocker->date.hour[0] = ihour / 10;
    cliclocker->date.hour[1] = ihour % 10;
    /* Set minutes */
    cliclocker->date.minute[0] = cliclocker->tm->tm_min / 10;
    cliclocker->date.minute[1] = cliclocker->tm->tm_min % 10;
    /* Set seconds */
    cliclocker->date.second[0] = cliclocker->tm->tm_sec / 10;
    cliclocker->date.second[1] = cliclocker->tm->tm_sec % 10;
    return;
}

static void draw_number(int n, int x, int y)
{
    int i, sy = y;

    for(i = 0; i < 30; ++i, ++sy) {
        if(sy == y + 6) {
            sy = y;
            ++x;
        }
        wbkgdset(cliclocker->framewin, COLOR_PAIR(number[n][i/2]));
        mvwaddch(cliclocker->framewin, x, sy, ' ');
    }
    wrefresh(cliclocker->framewin);

    return;
}

static void draw_clock(void)
{
    /* Draw hour numbers */
    draw_number(cliclocker->date.hour[0], 1, 1);
    draw_number(cliclocker->date.hour[1], 1, 8);
    /* 2 dot for number separation */
    wbkgdset(cliclocker->framewin, COLOR_PAIR(1));
    mvwaddstr(cliclocker->framewin, 2, 16, "  ");
    mvwaddstr(cliclocker->framewin, 4, 16, "  ");
    /* Draw minute numbers */
    draw_number(cliclocker->date.minute[0], 1, 20);
    draw_number(cliclocker->date.minute[1], 1, 27);
    /* 2 dot for number separation */
    wbkgdset(cliclocker->framewin, COLOR_PAIR(1));
    mvwaddstr(cliclocker->framewin, 2, 35, "  ");
    mvwaddstr(cliclocker->framewin, 4, 35, "  ");
    /* Draw second numbers */
    draw_number(cliclocker->date.second[0], 1, 39);
    draw_number(cliclocker->date.second[1], 1, 46);
}

static void clock_move(int x, int y)
{
    mvwin(cliclocker->framewin, (cliclocker->geo.x = x), (cliclocker->geo.y = y));
    return;
}

static void key_event(void)
{
    int c;
    struct timespec length = { 0, cliclocker->option.delay };
    switch(c = wgetch(stdscr)) {
        case 'q':
            cliclocker->running = false;
            break;
        default:
            nanosleep(&length, NULL);
            break;
    }
    return;
}

static void help(void)
{
    printf("cliclocker: A simple CLI clock\n");
    printf("------------------------------\n");
    printf("Usage: cliclocker [arguments] [color]\n\n");

    printf("Arguments:\n");
    printf("----------\n");
    printf("  -h || --help\t\tShow the help menu\n");
    printf("  -c || --color\t\tChange foreground color\n\n");

    printf("Colors:\n");
    printf("-------\n");
    printf("  blue (default)\n");
    printf("  black\n");
    printf("  red\n");
    printf("  green\n");
    printf("  yellow\n");
    printf("  magenta\n");
    printf("  cyan\n");
    printf("  white\n\n");

    printf("Cliclocker is a fork of cliclock from Youri \"nbyouri\" Mouton\n");

    return;
}

static void cliclocker_main(int chcolor)
{
    int color = chcolor;
    cliclocker = malloc(sizeof(cliclocker_t));
    cliclocker->option.color = color;
    cliclocker->option.delay = 40000000;
    init();
    while(cliclocker->running) {
        update_hour();
        draw_clock();
        key_event();
    }
    free(cliclocker);
    endwin();

    return;
}

int main(int argc, char *argv[])
{
    int chcolor;

    if (argv[1] == NULL) {
        chcolor = COLOR_BLUE;
        cliclocker_main(chcolor);
    }
    else if (0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help")) {
        help();
    }
    else if (0 == strcmp(argv[1], "-c") || 0 == strcmp(argv[1], "--color")) {
        if (argv[2] == NULL) {
            chcolor = COLOR_BLUE;
            cliclocker_main(chcolor);
        }
        else {
            if (0 == strcmp(argv[2], "black")) {
                chcolor = COLOR_BLACK;
                cliclocker_main(chcolor);
            }
            else if (0 == strcmp(argv[2], "red")) {
                chcolor = COLOR_RED;
                cliclocker_main(chcolor);
            }
            else if (0 == strcmp(argv[2], "green")) {
                chcolor = COLOR_GREEN;
                cliclocker_main(chcolor);
            }
            else if (0 == strcmp(argv[2], "yellow")) {
                chcolor = COLOR_YELLOW;
                cliclocker_main(chcolor);
            }
            else if (0 == strcmp(argv[2], "blue")) {
                chcolor = COLOR_BLUE;
                cliclocker_main(chcolor);
            }
            else if (0 == strcmp(argv[2], "magenta")) {
                chcolor = COLOR_MAGENTA;
                cliclocker_main(chcolor);
            }
            else if (0 == strcmp(argv[2], "cyan")) {
                chcolor = COLOR_CYAN;
                cliclocker_main(chcolor);
            }
            else if (0 == strcmp(argv[2], "white")) {
                chcolor = COLOR_WHITE;
                cliclocker_main(chcolor);
            }
        }
    }
    return 0;
}
