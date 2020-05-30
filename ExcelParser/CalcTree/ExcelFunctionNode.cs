using Irony.Parsing;
using System;
using System.Collections.Generic;
using System.Text;

namespace ExcelParser.CalcTree
{
    public class ExcelFunctionNode : Node
    {
        public List<Node> Arguments { get; private set; } = new List<Node>();

        public ExcelFunctionNode() { }

        public ExcelFunctionNode(ParseTreeNode parsedNode, ref bool valid, string defaultWorksheet) : base(parsedNode)
        {
            try
            {
                NodeType = "ExcelFunctionNode";
                IsCell = false;
                IsConstant = false;
                IsFunction = true;

                //Function Name
                Token = parsedNode.ChildNodes[0].ChildNodes[0].Token.Text.Replace("(", "");
                //Arguments
                //Left/Right Nodes will always be NULL for this node type, handled by arguments list.
                foreach (ParseTreeNode argument in parsedNode.ChildNodes[1].ChildNodes)
                {
                    Node argumentNode = Node.CreateNode(argument.ChildNodes[0], ref valid, defaultWorksheet);
                    Arguments.Add(argumentNode);
                }
                IsValid = valid;
            }
            catch (Exception e)
            {                
                valid = false;
                IsValid = false;
            }
        }

        public override Node Clone(int rowOffset = 0, int columnOffset = 0)
        {
            ExcelFunctionNode node = new ExcelFunctionNode();
            base.Clone(node, rowOffset, columnOffset);
            foreach(Node argNode in Arguments)
            {
                node.Arguments.Add(argNode.Clone(rowOffset, columnOffset));
            }
            return node;
        }

        public override IEnumerable<Node> ChildNodes()
        {
            return Arguments;
        }
    }
}