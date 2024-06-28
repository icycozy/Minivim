#ifndef MINIVIM_EDITOR
#define MINIVIM_EDITOR

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cmath>

extern WINDOW *winf, *wini, *winc;
extern std::vector<char> content[500000];
extern int cnt[500000], n, ROWS;
extern int x, y, fx, fy, supposed_y;
extern int rows, up, down, sl;

// scroll
void Gety(int position, int &left, int &right)
{
    if (position >= left && position <= right)
        y = position - left + 4;
    else if (position < left)
    {
        left = position;
        right = left + COLS - 5;
        y = 4;
    }
    else
    {
        right = position;
        left = right - COLS + 5;
        y = COLS - 1;
    }
}
void PRINT1(int left, int right)
{
    wclear(winf);
    for (int i = fx - x; i < std::min(fx - x + n, ROWS); i++)
    {
        mvwprintw(winf, i - (fx - x), 0, "%d", i + 1);
        for (int j = left; j <= std::min(cnt[i] - 1, right); j++)
            mvwprintw(winf, i - (fx - x), j - left + 4, "%c", content[i][j]);
    }
}
void UPN()
{
    if (fx == 0)
        return;
    fx--;
    int left = fy - y + 4, right = fy - y + COLS - 1;
    int position = 0;
    if (cnt[fx])
        position = std::min(cnt[fx] - 1, supposed_y);
    Gety(position, left, right);
    if (x)
        x--;
    fy = position;
    PRINT1(left, right);
}
void UPI()
{
    if (fx == 0)
        return;
    fx--;
    int left = fy - y + 4, right = fy - y + COLS - 1;
    int position = 0;
    if (cnt[fx])
        position = std::min(cnt[fx], supposed_y);
    Gety(position, left, right);
    if (x)
        x--;
    fy = position;
    PRINT1(left, right);
}
void DOWNN()
{
    if (fx == ROWS - 1)
        return;
    fx++;
    int left = fy - y + 4, right = fy - y + COLS - 1;
    int position = 0;
    if (cnt[fx])
        position = std::min(cnt[fx] - 1, supposed_y);
    Gety(position, left, right);
    if (x < n - 1)
        x++;
    fy = position;
    PRINT1(left, right);
}
void DOWNI()
{
    if (fx == ROWS)
        return;
    fx++;
    int left = fy - y + 4, right = fy - y + COLS - 1;
    int position = 0;
    if (cnt[fx])
        position = std::min(cnt[fx], supposed_y);
    Gety(position, left, right);
    if (x < n - 1)
        x++;
    fy = position;
    PRINT1(left, right);
}
void LEFT()
{
    if (fy == 0)
        return;
    fy--;
    supposed_y = fy;
    if (y > 4)
        y--;
    PRINT1(fy - y + 4, fy - y + COLS - 1);
}
void RIGHTN()
{
    if (fy >= cnt[fx] - 1)
        return;
    fy++;
    supposed_y = fy;
    if (y < COLS - 1)
        y++;
    PRINT1(fy - y + 4, fy - y + COLS - 1);
}
void RIGHTI()
{
    if (fy >= cnt[fx])
        return;
    fy++;
    supposed_y = fy;
    if (y < COLS - 1)
        y++;
    PRINT1(fy - y + 4, fy - y + COLS - 1);
}
void SHORT_O()
{
    fy = 0;
    y = 4;
    supposed_y = 0;
    PRINT1(fy - y + 4, fy - y + COLS - 1);
}
void SHORT_OO()
{
    int right = std::max(COLS - 5, cnt[fx] - 1), left = right - COLS + 5;
    if (!cnt[fx])
        fy = 0, y = 4;
    else
    {
        fy = cnt[fx] - 1;
        y = std::min(COLS - 1, cnt[fx] + 3);
    }
    supposed_y = fy;
    PRINT1(left, right);
}
void SHORT_dd()
{
    content[fx].clear();
    cnt[fx] = 0;
    fy = 0;
    y = 4;
    supposed_y = 0;
    PRINT1(fy - y + 4, fy - y + COLS - 1);
}
void SHORT_w()
{
    bool flag = 0, found = 0;
    int position = fy;
    int left = fy - y + 4, right = fy - y + COLS - 1;
    if (!cnt[fx])
        flag = 1;
    while (position < cnt[fx])
    {
        if (content[fx][position] == ' ')
            flag = 1;
        if (flag && content[fx][position] != ' ')
        {
            found = 1;
            break;
        }
        position++;
    }
    if (!found)
    {
        while (fx < ROWS)
        {
            if (fx == ROWS - 1)
            {
                position = std::max(0, cnt[fx] - 1);
                break;
            }
            fx++;
            if (x < n - 1)
                x++;
            position = 0;
            while (position < cnt[fx] && content[fx][position] == ' ')
                position++;
            if (position < cnt[fx])
            {
                found = 1;
                break;
            }
        }
    }
    supposed_y = fy = position;
    Gety(position, left, right);
    PRINT1(left, right);
}
void SHORT_b()
{
    bool flag = 0, found = 0;
    int position = fy;
    int left = fy - y + 4, right = fy - y + COLS - 1;
    if (!cnt[fx])
        flag = 1;
    while (position >= 0)
    {
        if (!cnt[fx])
            break;
        if (content[fx][position] == ' ')
            flag = 1;
        if (flag && content[fx][position] != ' ')
        {
            found = 1;
            break;
        }
        position--;
    }
    if (!found)
    {
        while (fx >= 0)
        {
            if (fx == 0)
            {
                position = 0;
                break;
            }
            fx--;
            if (x)
                x--;
            position = cnt[fx] - 1;
            while (position >= 0 && content[fx][position] == ' ')
                position--;
            if (position >= 0)
            {
                found = 1;
                break;
            }
        }
    }
    supposed_y = fy = position;
    Gety(position, left, right);
    PRINT1(left, right);
}
void Ins(int ch)
{
    cnt[fx]++;
    content[fx].push_back((char)ch);
    for (int i = cnt[fx] - 1; i > fy; i--)
        content[fx][i] = content[fx][i - 1];
    content[fx][fy] = (char)ch;
    fy++;
    if (y < COLS - 1)
        y++;
    supposed_y = fy;
    PRINT1(fy - y + 4, fy - y + COLS - 1);
}
void Del()
{
    if (fx == ROWS)
        return;
    if (fy)
    {
        cnt[fx]--;
        for (int i = fy - 1; i < cnt[fx]; i++)
            content[fx][i] = content[fx][i + 1];
        content[fx].pop_back();
        fy--;
        supposed_y = fy;
        if (y > 5)
            y--;
        if (!fy)
            y = 4;
        PRINT1(fy - y + 4, fy - y + COLS - 1);
    }
    else
    {
        if (!fx)
            return;
        int left = fy - y + 4, right = fy - y + COLS - 1;
        fx--;
        x = std::min(fx, n - 2);
        for (int i = 0; i < cnt[fx + 1]; i++)
            content[fx].push_back(content[fx + 1][i]);
        supposed_y = fy = cnt[fx];
        cnt[fx] += cnt[fx + 1];
        ROWS--;
        for (int i = fx + 1; i < ROWS; i++)
        {
            content[i].clear();
            for (int j = 0; j < cnt[i + 1]; j++)
                content[i].push_back(content[i + 1][j]);
            cnt[i] = cnt[i + 1];
        }
        content[ROWS].clear();
        cnt[ROWS] = 0;
        Gety(fy, left, right);
        PRINT1(left, left + COLS - 5);
    }
}
void ENTER()
{
    if (fx == ROWS)
        return;
    for (int i = ROWS; i > fx + 1; i--)
    {
        content[i].clear();
        for (int j = 0; j < cnt[i - 1]; j++)
            content[i].push_back(content[i - 1][j]);
        cnt[i] = cnt[i - 1];
    }
    ROWS++;
    content[fx + 1].clear();
    for (int i = fy; i < cnt[fx]; i++)
        content[fx + 1].push_back(content[fx][i]);
    cnt[fx + 1] = cnt[fx] - fy;
    while (cnt[fx] > fy)
    {
        content[fx].pop_back();
        cnt[fx]--;
    }
    supposed_y = fy = 0;
    y = 4;
    fx++;
    if (x < n - 1)
        x++;
    PRINT1(fy - y + 4, fy - y + COLS - 1);
}
void Jump(int line)
{
    fy = 0;
    y = 4;
    supposed_y = 0;
    int up = fx - x + 1, down = std::min(fx - x + n, ROWS);
    if (line >= up && line <= down)
        x = line - up;
    else if (line < up)
    {
        x = 0;
        up = line;
        down = std::min(line + n - 1, ROWS);
    }
    else
    {
        x = n - 1;
        down = line;
        up = down - n + 1;
    }
    fx = line - 1;
    wclear(winf);
    for (int i = up - 1; i <= down - 1; i++)
    {
        mvwprintw(winf, i - up + 1, 0, "%d", i + 1);
        for (int j = 0; j < std::min(cnt[i], COLS - 4); j++)
            mvwprintw(winf, i - up + 1, j + 4, "%c", content[i][j]);
    }
    wmove(winf, x, y);
    wrefresh(winf);
}
void ItoN()
{
    if (fx == ROWS)
    {
        fx--;
        if (cnt[fx])
        {
            fy = cnt[fx] - 1;
            y = std::min(COLS - 1, cnt[fx] + 3);
        }
        else
            fy = 0, y = 4;
        supposed_y = fy;
        int right = std::max(COLS - 5, cnt[fx] - 1);
        wclear(winf);
        for (int i = fx - x; i < std::min(fx - x + n, ROWS); i++)
        {
            mvwprintw(winf, i - (fx - x), 0, "%d", i + 1);
            for (int j = std::min(right, cnt[i] - 1); j > right - COLS + 4; j--)
                mvwprintw(winf, i - (fx - x), j - (right - COLS + 1), "%c", content[i][j]);
        }
    }
    else if (fy == cnt[fx])
    {
        if (!cnt[fx])
            fy = 0, y = 4;
        else
        {
            fy--;
            y = std::min(COLS - 1, cnt[fx] + 3);
        }
        supposed_y = fy;
        int right = std::max(COLS - 5, cnt[fx] - 1);
        wclear(winf);
        for (int i = fx - x; i < std::min(ROWS, fx - x + n); i++)
        {
            mvwprintw(winf, i - (fx - x), 0, "%d", i + 1);
            for (int j = std::min(right, cnt[i] - 1); j > right - COLS + 4; j--)
                mvwprintw(winf, i - (fx - x), j - (right - COLS + 1), "%c", content[i][j]);
        }
    }
}

// break
int Getx()
{
    int sum = 0;
    for (int i = up; i < fx; i++)
    {
        if (cnt[i])
            sum += ((cnt[i] - 1) / (COLS - 5)) + 1;
        else
            sum += 1;
    }
    if (fx >= up)
        sum += (fy / (COLS - 5)) + 1;
    return sum - 1 - sl;
}
void PRINT2()
{
    wclear(winf);
    int line = 0, col = 3;
    mvwprintw(winf, line, 0, "%d", up + 1);
    for (int j = (COLS - 5) * sl; j < cnt[up]; j++)
    {
        if (line == n - 1)
            down = up;
        col++;
        if (col == COLS - 1)
        {
            line++;
            col = 4;
        }
        if (line == n)
            return;
        mvwprintw(winf, line, col, "%c", content[up][j]);
    }
    line++, col = 3;
    for (int i = up + 1; i < ROWS; i++)
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
                return;
            mvwprintw(winf, line, col, "%c", content[i][j]);
        }
        col = 3, line++;
    }
}
void UPN_B()
{
    if (fx == 0)
        return;
    fx--;
    if (cnt[fx])
        fy = std::min(cnt[fx] - 1, supposed_y);
    else
        fy = 0;
    x = Getx();
    if (x <= 0)
    {
        up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
        x = Getx();
    }
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void UPI_B()
{
    if (fx == 0)
        return;
    fx--;
    if (cnt[fx])
        fy = std::min(cnt[fx], supposed_y);
    else
        fy = 0;
    x = Getx();
    if (x <= 0)
    {
        up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
        x = Getx();
    }
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void DOWNN_B()
{
    if (fx == ROWS - 1)
        return;
    fx++;
    if (cnt[fx])
        fy = std::min(cnt[fx] - 1, supposed_y);
    else
        fy = 0;
    x = Getx();
    if (x > n - 1)
    {
        up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
        x = Getx();
    }
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void DOWNI_B()
{
    if (fx == ROWS)
        return;
    fx++;
    if (cnt[fx])
        fy = std::min(cnt[fx], supposed_y);
    else
        fy = 0;
    x = Getx();
    if (x > n - 1)
    {
        up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
        x = Getx();
    }
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void LEFT_B()
{
    if (fy == 0)
        return;
    fy--;
    supposed_y = fy;
    if (x == 0 && y == 4)
        if (fy)
            up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
    x = Getx();
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void RIGHTN_B()
{
    if (fy >= cnt[fx] - 1)
        return;
    fy++;
    supposed_y = fy;
    if (y == COLS - 2 && x == n - 1)
        if (fy <= cnt[fx] - 1)
            up = fx, sl = fy / (COLS - 5) - 1;
    x = Getx();
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void RIGHTI_B()
{
    if (fy == cnt[fx])
        return;
    if (x == n - 1 && y == COLS - 1)
    {
        if (fy < cnt[fx] - 1)
        {
            up = fx, sl = (fy + 1) / (COLS - 5) - 1;
        }
    }
    if (y == COLS - 2)
        y = COLS - 1;
    else
    {
        fy++;
        y = (fy % (COLS - 5)) + 4;
    }
    supposed_y = fy;
    x = Getx();
    PRINT2();
}
void SHORT_O_B()
{
    supposed_y = fy = 0;
    y = 4;
    x = Getx();
    if (x <= 0)
    {
        up = fx, sl = 0;
        x = 0;
    }
    PRINT2();
}
void SHORT_OO_B()
{
    supposed_y = fy = std::max(cnt[fx] - 1, 0);
    y = (fy % (COLS - 5)) + 4;
    x = Getx();
    if (x > n - 1)
    {
        up = fx, sl = std::max((cnt[fx] - 1) / (COLS - 5) + 1 - n, 0);
        x = Getx();
    }
    PRINT2();
}
void SHORT_dd_B()
{
    content[fx].clear();
    cnt[fx] = 0;
    supposed_y = fy = 0;
    y = 4;
    x = Getx();
    if (x <= 0)
    {
        up = fx, sl = 0;
        x = 0;
    }
    PRINT2();
}
void SHORT_w_B()
{
    bool flag = 0, found = 0;
    int position = fy;
    if (!cnt[fx])
        flag = 1;
    while (position < cnt[fx])
    {
        if (content[fx][position] == ' ')
            flag = 1;
        if (flag && content[fx][position] != ' ')
        {
            found = 1;
            break;
        }
        position++;
    }
    if (!found)
    {
        while (fx < ROWS)
        {
            if (fx == ROWS - 1)
            {
                position = std::max(cnt[fx] - 1, 0);
                break;
            }
            fx++;
            position = 0;
            while (position < cnt[fx] && content[fx][position] == ' ')
                position++;
            if (position < cnt[fx])
            {
                found = 1;
                break;
            }
        }
    }
    supposed_y = fy = position;
    if (fx >= down)
        up = fx;
    x = Getx();
    if (x > n - 1)
    {
        sl = fy / (COLS - 5) - n + 2;
        x = Getx();
    }
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void SHORT_b_B()
{
    bool flag = 0, found = 0;
    int position = fy;
    if (!cnt[fx])
        flag = 1;
    while (position >= 0)
    {
        if (!cnt[fx])
            break;
        if (content[fx][position] == ' ')
            flag = 1;
        if (flag && content[fx][position] != ' ')
        {
            found = 1;
            break;
        }
        position--;
    }
    if (!found)
    {
        while (fx >= 0)
        {
            if (fx == 0)
            {
                position = 0;
                break;
            }
            fx--;
            position = cnt[fx] - 1;
            while (position >= 0 && content[fx][position] == ' ')
                position--;
            if (position >= 0)
            {
                found = 1;
                break;
            }
        }
    }
    supposed_y = fy = position;
    x = Getx();
    if (x <= 0)
    {
        up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
        x = Getx();
    }
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void Ins_B(int ch)
{
    if (y == COLS - 1)
        fy++;
    cnt[fx]++;
    content[fx].push_back((char)ch);
    for (int i = cnt[fx] - 1; i > fy; i--)
        content[fx][i] = content[fx][i - 1];
    content[fx][fy] = (char)ch;
    fy++;
    supposed_y = fy;
    x = Getx();
    if (x > n - 1)
        fx = up, sl = std::max(0, (cnt[fx] - 1) / (COLS - 5) + 1 - n);
    y = (fy % (COLS - 5)) + 4;
    PRINT2();
}
void Del_B()
{
    if (fx == ROWS)
        return;
    if (fy)
    {
        cnt[fx]--;
        for (int i = fy - 1; i < cnt[fx]; i++)
            content[fx][i] = content[fx][i + 1];
        content[fx].pop_back();
        fy--;
        supposed_y = fy;
        if (x == 0 && y == 4 && fy)
        {
            up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
        }
        x = Getx();
        y = (fy % (COLS - 5)) + 4;
        PRINT2();
    }
    else
    {
        if (!fx)
            return;
        fx--;
        for (int i = 0; i < cnt[fx + 1]; i++)
            content[fx].push_back(content[fx + 1][i]);
        supposed_y = fy = cnt[fx];
        cnt[fx] += cnt[fx + 1];
        ROWS--;
        for (int i = fx + 1; i < ROWS; i++)
        {
            content[i].clear();
            for (int j = 0; j < cnt[i + 1]; j++)
                content[i].push_back(content[i + 1][j]);
            cnt[i] = cnt[i + 1];
        }
        content[ROWS].clear();
        cnt[ROWS] = 0;
        x = Getx();
        if (x <= 0)
        {
            up = fx, sl = 0;
            x = Getx();
            if (x > n - 1)
            {
                sl = fy / (COLS - 5) - n + 1;
                x = Getx();
            }
        }
        y = (fy % (COLS - 5)) + 4;
        PRINT2();
    }
}
void ENTER_B()
{
    if (fx == ROWS)
        return;
    for (int i = ROWS; i > fx + 1; i--)
    {
        content[i].clear();
        for (int j = 0; j < cnt[i - 1]; j++)
            content[i].push_back(content[i - 1][j]);
        cnt[i] = cnt[i - 1];
    }
    ROWS++;
    content[fx + 1].clear();
    for (int i = fy; i < cnt[fx]; i++)
        content[fx + 1].push_back(content[fx][i]);
    cnt[fx + 1] = cnt[fx] - fy;
    while (cnt[fx] > fy)
    {
        content[fx].pop_back();
        cnt[fx]--;
    }
    supposed_y = fy = 0;
    y = 4;
    fx++;
    x = Getx();
    if (x > n - 1)
    {
        up = fx - 1, x = 1;
        if (cnt[fx - 1] <= COLS - 5)
        {
            sl = 0;
        }
        else
            sl = (cnt[fx - 1] - 1) / (COLS - 5);
    }
    PRINT2();
}
void Jump_B(int line)
{
    up = line - 1, sl = 0;
    fx = line - 1;
    fy = 0;
    x = 0;
    y = 4;
    PRINT2();
    wmove(winf, x, y);
    wrefresh(winf);
}
void ItoN_B()
{
    if (fx == ROWS)
    {
        fx--;
        if (cnt[fx])
        {
            fy = cnt[fx] - 1;
            y = (fy % (COLS - 5)) + 4;
        }
        else
            fy = 0, y = 4;
        supposed_y = fy;
        x = Getx();
        if (x <= 0)
        {
            up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
            x = Getx();
        }
        PRINT2();
    }
    else if (fy == cnt[fx])
    {
        if (!cnt[fx])
            fy = 0, y = 4;
        else
        {
            fy--;
            y = (fy % (COLS - 5)) + 4;
        }
        supposed_y = fy;
        x = Getx();
        if (x <= 0)
        {
            up = fx, sl = std::max(fy / (COLS - 5) - n + 2, 0);
            x = Getx();
        }
        PRINT2();
    }
}

#endif