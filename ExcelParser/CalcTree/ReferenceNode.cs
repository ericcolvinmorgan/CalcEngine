using DocumentFormat.OpenXml.Drawing.Charts;
using Irony.Parsing;
using System;
using System.Collections.Generic;
using System.Text;

namespace ExcelParser.CalcTree
{
    public class ReferenceNode : Node
    {
        public ReferenceNode() { }
        public ReferenceNode(ParseTreeNode parsedNode, ref bool valid, string worksheet) : base(parsedNode)
        {
            NodeType = "ReferenceNode";
            IsCell = false;
            IsConstant = false;
            IsFunction = true;
            Token = parsedNode.ChildNodes[1].Token.Text;
            LeftNode = Node.CreateNode(parsedNode.ChildNodes[0], ref valid, worksheet, "ReferenceFunctionCall");

            //Update worksheet reference if left node shows a different worksheet.
            if(LeftNode.NodeType == "CellNode")
            {
                worksheet = ((CellNode)(LeftNode)).Worksheet;
            }

            RightNode = Node.CreateNode(parsedNode.ChildNodes[2], ref valid, worksheet, "ReferenceFunctionCall");
            IsValid = valid;
        }
        public override Node Clone(int rowOffset = 0, int columnOffset = 0)
        {
            ReferenceNode node = new ReferenceNode();
            base.Clone(node, rowOffset, columnOffset);
            return node;
        }
    }
}