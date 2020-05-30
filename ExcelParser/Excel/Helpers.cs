using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ExcelParser.Excel
{
    public static class Helpers
    {
        const int MAX_COL = 16384;
        const int MAX_ROW = 1048576;

        public static int ColumnIntFromAddress(string address)
        {
            Match reg = Regex.Match(address.ToUpper(), @"^\$?([A-Z]{1,})\$?([1-9]{1}[0-9]{0,})");
            if (reg.Success && reg.Groups.Count == 3)
            {
                int colNum = 0;
                string col = reg.Groups[1].Value;
                foreach (char c in col)
                {
                    colNum *= 26;
                    colNum = colNum + (c - 'A') + 1;
                }

                if (colNum > MAX_COL)
                {
                    colNum = -1;
                }

                return colNum;
            }
            return -1;
        }

        public static int ColumnIntFromAddress(string address, ref bool absolute)
        {
            Match reg = Regex.Match(address.ToUpper(), @"^(\$?)([A-Z]{1,})(\$?)([1-9]{1}[0-9]{0,})");
            if (reg.Success && reg.Groups.Count == 5)
            {
                int colNum = 0;
                string col = reg.Groups[2].Value;
                foreach (char c in col)
                {
                    colNum *= 26;
                    colNum = colNum + (c - 'A') + 1;
                }

                if (colNum > MAX_COL)
                {
                    colNum = -1;
                }

                absolute = reg.Groups[1].Length > 0;
                return colNum;
            }
            absolute = false;
            return -1;
        }

        public static string ColumnAlphaFromInt(int colNum)
        {
            if (colNum > 0 && colNum <= MAX_COL)
            {
                string colAdd = "";
                while (colNum > 0)
                {
                    int colMod = colNum % 26;
                    colMod = colMod == 0 ? 26 : colMod;
                    colAdd = Char.ConvertFromUtf32(('A' + colMod - 1)) + colAdd;
                    colNum = (colNum - colMod) / 26;
                }
                return colAdd;
            }
            return null;
        }

        public static int RowIntFromAddress(string address)
        {
            Match reg = Regex.Match(address.ToUpper(), @"^\$?([A-Z]{1,})\$?([1-9]{1}[0-9]{0,})");
            if (reg.Success && reg.Groups.Count == 3)
            {
                int row = -1;
                if(int.TryParse(reg.Groups[2].Value, out row))
                {
                    return row > 0 && row <= MAX_ROW ? row : -1;
                }
            }
            return -1;
        }

        public static int RowIntFromAddress(string address, ref bool absolute)
        {
            Match reg = Regex.Match(address.ToUpper(), @"^(\$?)([A-Z]{1,})(\$?)([1-9]{1}[0-9]{0,})");
            if (reg.Success && reg.Groups.Count == 5)
            {
                int row = -1;
                if (int.TryParse(reg.Groups[4].Value, out row))
                {
                    absolute = reg.Groups[3].Length > 0;
                    return row > 0 && row <= MAX_ROW ? row : -1;
                }
            }
            absolute = false;
            return -1;
        }

        public static string CreateCellReference(int columnNumber, int rowNumber, bool columnAbsolute = false, bool rowAbsolute = false)
        {
            return string.Format("{0}{1}{2}{3}", (columnAbsolute ? "$" : ""), ColumnAlphaFromInt(columnNumber), (rowAbsolute ? "$" : ""), rowNumber);
        }
    }
}
