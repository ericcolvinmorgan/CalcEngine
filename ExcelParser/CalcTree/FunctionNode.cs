using Irony.Parsing;
using System;
using System.Collections.Generic;
using System.Text;

namespace ExcelParser.CalcTree
{
    public class FunctionNode : Node
    {
        public FunctionNode() { }
        public FunctionNode(ParseTreeNode parsedNode, ref bool valid, string defaultWorksheet) : base(parsedNode)
        {
            try
            {
                NodeType = "FunctionNode";
                IsCell = false;
                IsConstant = false;
                IsFunction = true;

                //Handle
                if (parsedNode.ChildNodes.Count == 2)
                {
                    //Urary operator, add a zero number node to faciliate
                    LeftNode = new NumberNode("0");
                    Token = parsedNode.ChildNodes[0].Token.Text;
                    RightNode = Node.CreateNode(parsedNode.ChildNodes[1], ref valid, defaultWorksheet);
                }
                else if (parsedNode.ChildNodes.Count == 3)
                {
                    LeftNode = Node.CreateNode(parsedNode.ChildNodes[0], ref valid, defaultWorksheet);
                    Token = parsedNode.ChildNodes[1].Token.Text;
                    RightNode = Node.CreateNode(parsedNode.ChildNodes[2], ref valid, defaultWorksheet);
                }
                else
                {
                    valid = false;                    
                }
                IsValid = valid;
            }
            catch(Exception e)
            {
                valid = false;
                IsValid = false;
            }
        }

        public override Node Clone(int rowOffset = 0, int columnOffset = 0)
        {
            FunctionNode node = new FunctionNode();
            base.Clone(node, rowOffset, columnOffset);
            return node;
        }
    }
}
