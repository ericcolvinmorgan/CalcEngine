using Irony.Parsing;
using System;
using System.Collections.Generic;
using System.Text;

namespace ExcelParser.CalcTree
{
    public class NumberNode : Node
    {
        public NumberNode() { }        

        public NumberNode(string numberToken)
        {
            NodeType = "NumberNode";
            IsCell = false;
            IsConstant = true;
            IsFunction = false;
            Token = numberToken;
            IsValid = true;
        }

        public NumberNode(ParseTreeNode parsedNode, ref bool valid) : base(parsedNode)
        {
            NodeType = "NumberNode";
            IsCell = false;
            IsConstant = true;
            IsFunction = false;
            Token = parsedNode.Token.Text;
            IsValid = valid;
        }

        public override Node Clone(int rowOffset = 0, int columnOffset = 0)
        {
            NumberNode node = new NumberNode();
            base.Clone(node, rowOffset, columnOffset);
            return node;
        }
    }
}
