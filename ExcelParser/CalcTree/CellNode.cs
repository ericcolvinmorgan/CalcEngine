using ExcelParser.Excel;
using Irony.Parsing;
using System;
using System.Collections.Generic;
using System.Text;

namespace ExcelParser.CalcTree
{
    class CellNode : Node
    {
        public string Worksheet { get; private set; }
        public int Row { get; private set; } = 0;
        public int Column { get; private set; } = 0;
        public CellNode() { }
        public CellNode(ParseTreeNode parsedNode, ref bool valid, string sheet = null) : base(parsedNode)
        {
            NodeType = "CellNode";
            IsCell = true;
            IsConstant = false;
            IsFunction = false;
            Worksheet = sheet;
            Token = parsedNode.Token.Text;
            bool columnAbsolute = false;
            Column = Helpers.ColumnIntFromAddress(Token, ref columnAbsolute);
            bool rowAbsolute = false;
            Row = Helpers.RowIntFromAddress(Token, ref rowAbsolute);
            IsValid = true;
        }

        public override Node Clone(int rowOffset = 0, int columnOffset = 0)
        {
            CellNode node = new CellNode();
            base.Clone(node, rowOffset, columnOffset);
            node.Worksheet = Worksheet;

            //Apply offset
            bool columnAbsolute = false;
            int columnNum = Helpers.ColumnIntFromAddress(Token, ref columnAbsolute);
            if (!columnAbsolute)
            {
                columnNum += columnOffset;
            }

            bool rowAbsolute = false;
            int rowNum = Helpers.RowIntFromAddress(Token, ref rowAbsolute);
            if(!rowAbsolute)
            {
                rowNum += rowOffset;
            }

            if(columnNum > 0 && rowNum > 0)
            {
                node.Token = string.Format("{0}{1}{2}{3}", 
                    (columnAbsolute ? "$" : ""), Helpers.ColumnAlphaFromInt(columnNum), 
                    (rowAbsolute ? "$" : ""), rowNum);
            }
            else
            {
                node.IsValid = false;
                node.Token = "#REF!";
            }

            return node;
        }
    }
}