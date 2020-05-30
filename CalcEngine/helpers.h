#pragma once
#include <string>

const int MAX_COL = 16384;
const int MAX_ROW = 1048576;

static std::string column_alpha_from_int(int col_num)
{
    if (col_num > 0 && col_num <= MAX_COL)
    {
        std::string col_add = "";
        while (col_num > 0)
        {
            int col_mod = col_num % 26;
            col_mod = col_mod == 0 ? 26 : col_mod;
            col_add.insert(0, 1, ('A' + col_mod - 1));
            col_num = (col_num - col_mod) / 26;
        }
        return col_add;
    }
    return "";
}