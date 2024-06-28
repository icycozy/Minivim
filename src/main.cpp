#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "editor.hpp"

#define REG_COLOR_NUM 1
#define CUS_COLOR_NUM 2
#define ONE_COLOR_NUM 3
#define TWO_COLOR_NUM 4

void Normal();
void Command();
void Insert();

int n = 15, cnt[500000], fmode, x, y, fx, fy, supposed_y, ROWS = 1, rows;
int cntcmd = 0, pointcmd = 0, lengthcmd[10000] = {0};
int up, down, sl;
std::vector<char> content[500000];
std::vector<std::string> hiscmd;
std::string filename;
std::fstream file;
WINDOW *winf, *wini, *winc;
bool is_created = 0, is_edited = 0, quit = 0;

void FOpen()
{
    file.open(filename, std::ios::in);
    if (!file.is_open())
        file.open(filename, std::ios::out), is_created = 1;
    fx = 0, fy = 0;
    if (fmode == 1)
    {
        mvwprintw(winf, 0, 0, "1");
        x = 0, y = 4;
        file.close();
        return;
    }
    char fchar;
    while ((fchar = file.get()) != EOF)
    {
        if (fchar == '\n')
            ROWS++;
        else
        {
            content[ROWS - 1].push_back(fchar);
            cnt[ROWS - 1]++;
        }
    }
    if (fmode == 4 || fmode == 2)
    {
        for (int i = 0; i < std::min(ROWS, n); i++)
        {
            mvwprintw(winf, i, 0, "%d", i + 1);
            for (int j = 0; j < std::min(COLS - 1, cnt[i]); j++)
                mvwprintw(winf, i, j + 4, "%c", content[i][j]);
        }
    }
    else
    {
        int line = 0, col = 3;
        for (int i = 0; i < ROWS; i++)
        {
            mvwprintw(winf, line, 0, "%d", i + 1);
            if (line == n - 1)
                down = i;
            for (int j = 0; j < cnt[i]; j++)
            {
                if (line == n - 1)
                    down = i;
                col++;
                if (col == COLS - 1)
                {
                    line++;
                    col = 4;
                }
                if (line == n)
                    break;
                mvwprintw(winf, line, col, "%c", content[i][j]);
            }
            if (line == n)
                break;
            col = 3, line++;
        }
    }
    fx = fy = supposed_y = 0;
    x = 0;
    y = 4;
    file.close();
}
void Save()
{
    is_edited=0;
    file.open(filename, std::ios::out);
    file.clear();
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < cnt[i]; j++)
            file.put(content[i][j]);
        if(i<ROWS-1) file.put('\n');
    }
    file.close();
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "error" << std::endl;
        return -1;
    }
    if (!strcmp(argv[2], "-t"))
        fmode = 1;
    else if (!strcmp(argv[2], "-R"))
        fmode = 2;
    else if (!strcmp(argv[2], "-W"))
    {
        if (!strcmp(argv[3], "break"))
            fmode = 3;
        else
            fmode = 4;
    }
    filename = argv[argc - 1];
    // mvwprintw(winf, 0, 1, "%d", fmode);

    initscr(); /* Start curses mode */
    raw();
    noecho();
    keypad(stdscr, true); /*stdscr is the regular window*/

    // init color
    start_color();
    init_pair(REG_COLOR_NUM, COLOR_WHITE, COLOR_BLACK);
    init_pair(CUS_COLOR_NUM, COLOR_WHITE, COLOR_BLACK);
    init_pair(ONE_COLOR_NUM, COLOR_YELLOW, COLOR_CYAN);
    init_pair(TWO_COLOR_NUM, COLOR_GREEN, COLOR_BLUE);

    // set stdscr color
    wbkgd(stdscr, COLOR_PAIR(REG_COLOR_NUM)); // background
    wrefresh(stdscr);

    winf = newwin(n, COLS, 0, 0); /*create a 5*10 window in (0, 0)*/
    keypad(winf, true);
    wbkgd(winf, COLOR_PAIR(CUS_COLOR_NUM));
    wrefresh(winf);

    wini = newwin(1, COLS, n, 0);
    keypad(wini, true);
    wbkgd(wini, COLOR_PAIR(ONE_COLOR_NUM));
    wrefresh(wini);

    winc = newwin(1, COLS, n + 1, 0);
    keypad(winc, true);
    wbkgd(winc, COLOR_PAIR(TWO_COLOR_NUM));
    wrefresh(winc);

    FOpen();
    mvwprintw(wini, 0, 0, "Normal %s 0 0 0 0", filename.c_str());
    wmove(winf, 0, 4);
    wrefresh(wini);
    wrefresh(winf);

    hiscmd.push_back("");
    Normal();

    endwin();
    return 0;
}

void Normal()
{
    int ch = 0;
    while ((ch = getch()) != 27)
    {
        if (ch == KEY_UP)
        {
            if (fmode == 3)
                UPN_B();
            else
                UPN();
        }
        else if (ch == KEY_DOWN)
        {
            if (fmode == 3)
                DOWNN_B();
            else
                DOWNN();
        }
        else if (ch == KEY_LEFT)
        {
            if (fmode == 3)
                LEFT_B();
            else
                LEFT();
        }
        else if (ch == KEY_RIGHT)
        {
            if (fmode == 3)
                RIGHTN_B();
            else
                RIGHTN();
        }
        else if (ch == 48) // shortcut 0
        {
            if (fmode == 3)
                SHORT_O_B();
            else
                SHORT_O();
        }
        else if (ch == 36) // shortcut $
        {
            if (fmode == 3)
                SHORT_OO_B();
            else
                SHORT_OO();
        }
        else if (ch == 119) // shortcut w
        {
            if (fmode == 3)
                SHORT_w_B();
            else
                SHORT_w();
        }
        else if (ch == 98) // shortcut b
        {
            if (fmode == 3)
                SHORT_b_B();
            else
                SHORT_b();
        }
        else if (ch == 100) // shortcut dd
        {
            ch = getch();
            if (ch == 100 && fmode != 2)
            {
                is_edited = 1;
                if (fmode == 3)
                    SHORT_dd_B();
                else
                    SHORT_dd();
            }
        }
        else if (ch == 58)
        {
            Command();
            wdeleteln(wini);
            wdeleteln(winc);
            mvwprintw(wini, 0, 0, "Normal %s (x,y)=%d,%d (fx,fy)=%d,%d", filename.c_str(), x, y, fx, fy);
            wmove(winf, x, y);
            wrefresh(winc);
            wrefresh(wini);
            wrefresh(winf);
        }
        else if (ch == 105)
        {
            Insert();
            wdeleteln(wini);
            mvwprintw(wini, 0, 0, "Normal %s (x,y)=%d,%d (fx,fy)=%d,%d", filename.c_str(), x, y, fx, fy);
            wmove(winf, x, y);
            wrefresh(wini);
            wrefresh(winf);
        }

        if (quit)
            return;

        wdeleteln(wini);
        mvwprintw(wini, 0, 0, "Normal %s (x,y)=%d,%d (fx,fy)=%d,%d", filename.c_str(), x, y, fx, fy);
        wmove(winf, x, y);
        wrefresh(wini);
        wrefresh(winf);
    }
}

void Command()
{
    wdeleteln(wini);
    mvwprintw(wini, 0, 0, "Command %s (x,y)=%d,%d (fx,fy)=%d,%d", filename.c_str(), x, y, fx, fy);
    mvwprintw(winc, 0, 0, ":");
    wmove(winc, 0, 1);
    wrefresh(wini);
    wrefresh(winc);
    int ch = 0, cy = 1;
    while ((ch = getch()) != 27)
    {
        if (ch == KEY_UP) // command history
        {
            if (pointcmd)
            {
                pointcmd--;
                wclear(winc);
                mvwprintw(winc, 0, 0, ":%s", hiscmd[pointcmd].c_str());
                cy = lengthcmd[pointcmd] + 1;
                hiscmd[cntcmd] = hiscmd[pointcmd];
                lengthcmd[cntcmd] = lengthcmd[pointcmd];
            }
        }
        else if (ch == KEY_DOWN)
        {
            if (pointcmd < cntcmd - 1)
            {
                pointcmd++;
                wclear(winc);
                mvwprintw(winc, 0, 0, ":%s", hiscmd[pointcmd].c_str());
                cy = lengthcmd[pointcmd] + 1;
                hiscmd[cntcmd] = hiscmd[pointcmd];
                lengthcmd[cntcmd] = lengthcmd[pointcmd];
            }
        }
        else if (ch == 263) // backspace
        {
            hiscmd[cntcmd].pop_back();
            lengthcmd[cntcmd]--;
            cy--;
            wclear(winc);
            mvwprintw(winc, 0, 0, ":%s", hiscmd[cntcmd].c_str());
        }
        else if (ch == 10) // ENTER
        {
            if (hiscmd[cntcmd] == "w")
                Save();
            else if (hiscmd[cntcmd] == "q")
            {
                if (!is_edited)
                    quit = 1;
                else
                {
                    wclear(winc);
                    mvwprintw(winc, 0, 0, "WARNING: UNSAVED   Press any key to return.");
                    wrefresh(winc);
                    ch = getch();
                }
            }
            else if (hiscmd[cntcmd] == "wq")
            {
                Save();
                quit = 1;
            }
            else if (hiscmd[cntcmd] == "q!")
            {
                quit = 1;
                if (is_created)
                    remove(filename.c_str());
            }
            else if (hiscmd[cntcmd].substr(0, 3) == "jmp")
            {
                int line = 0;
                for (int i = 4; i < lengthcmd[cntcmd]; i++)
                    line = line * 10 + (hiscmd[cntcmd][i] - '0');
                if (line <= 0 || line > ROWS)
                {
                    wclear(winc);
                    mvwprintw(winc, 0, 0, "NO SUCH LINE    Press any key to return.");
                    wrefresh(winc);
                    ch = getch();
                }
                else
                {
                    if (fmode == 3)
                        Jump_B(line);
                    else
                        Jump(line);
                }
            }
            if (quit)
                return;
            hiscmd.push_back("");
            cntcmd++;
            pointcmd = cntcmd;
            cy = 1;
            wdeleteln(winc);
            mvwprintw(winc, 0, 0, ":");
        }
        else
        {
            mvwprintw(winc, 0, cy, "%c", ch);
            cy++;
            lengthcmd[cntcmd]++;
            hiscmd[cntcmd].push_back((char)ch);
        }
        wmove(winc, 0, cy);
        wrefresh(winc);
    }
}

void Insert()
{
    wdeleteln(wini);
    mvwprintw(wini, 0, 0, "Insert %s (x,y)=%d,%d (fx,fy)=%d,%d", filename.c_str(), x, y, fx, fy);
    mvwprintw(winc, 0, 0, ":");
    wmove(winf, x, y);
    wrefresh(wini);
    wrefresh(winf);
    int ch = 0;
    while ((ch = getch()) != 27)
    {
        if (ch == KEY_UP)
        {
            if (fmode == 3)
                UPI_B();
            else
                UPI();
        }
        else if (ch == KEY_DOWN)
        {
            if (fmode == 3)
                DOWNI_B();
            else
                DOWNI();
        }
        else if (ch == KEY_LEFT)
        {
            if (fmode == 3)
                LEFT_B();
            else
                LEFT();
        }
        else if (ch == KEY_RIGHT)
        {
            if (fmode == 3)
                RIGHTI_B();
            else
                RIGHTI();
        }
        else if (ch == 9)
        {
            if (fmode == 2)
                continue;
            is_edited = 1;
            for (int i = 1; i <= 4; i++)
            {
                if (fmode == 3)
                    Ins_B(32);
                else
                    Ins(32);
            }
        } // tab
        else if (ch == 10)
        {
            if (fmode == 2)
                continue;
            is_edited = 1;
            if (fmode == 3)
                ENTER_B();
            else
                ENTER();
        } // Enter
        else if (ch == 263)
        {
            if (fmode == 2)
                continue;
            is_edited = 1;
            if (fmode == 3)
                Del_B();
            else
                Del();
        } // Backspace->deleting
        else
        {
            if (fmode == 2)
                continue;
            is_edited = 1;
            if (fmode == 3)
                Ins_B(ch);
            else
                Ins(ch);
        }

        wdeleteln(wini);
        mvwprintw(wini, 0, 0, "Insert %s %d %d %d %d", filename.c_str(), x, y, fx, fy);
        wmove(winf, x, y);
        wrefresh(wini);
        wrefresh(winf);
    }
    if (fmode == 3)
        ItoN_B();
    else
        ItoN();
}