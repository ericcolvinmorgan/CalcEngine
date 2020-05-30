using Irony.Parsing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ExcelParser.CalcTree
{
    public abstract class Node
    {
        public string NodeType { get; protected set; }
        public bool IsValid { get; protected set; } = false;
        public bool IsCell { get; protected set; }
        public bool IsConstant { get; protected set; }
        public bool IsFunction { get; protected set; }
        public string Token { get; protected set; }
        public Node LeftNode { get; protected set; } = null;
        public Node RightNode { get; protected set; } = null;

        public Node() { }
        public Node(ParseTreeNode parsedNode)
        { }

        public abstract Node Clone(int rowOffset = 0, int columnOffset = 0);

        protected virtual void Clone(Node newNode, int rowOffset = 0, int columnOffset = 0)
        {
            newNode.NodeType = NodeType;
            newNode.IsValid = IsValid;
            newNode.IsCell = IsCell;
            newNode.IsConstant = IsConstant;
            newNode.IsFunction = IsFunction;
            newNode.Token = Token;
            newNode.LeftNode = LeftNode?.Clone(rowOffset, columnOffset);
            newNode.RightNode = RightNode?.Clone(rowOffset, columnOffset);
        }

        public virtual IEnumerable<Node> ChildNodes()
        {
            if (LeftNode != null)
            {
                yield return LeftNode;
            }

            if (RightNode != null)
            {
                yield return RightNode;
            }
        }

        //Parse Tree Node Types Currently Supported:
        //Formula > Constant > Number
        //Formula > Excel Function Call
        //Formula > Function Call - Binary Operators - (* + / - & ^ > = < <> >= <=)
        //Formula > Function Call - Unary Operators - (%)
        //Formula > Reference > Cell
        //Formula > Reference > ReferenceFunction (:)
        public static Node CreateNode(ParseTreeNode parsedNode, ref bool valid, string defaultWorksheet, string level = "")
        {
            Node processedNode = null;

            //Determine current processing level
            if (parsedNode.Term?.Name == "Formula")
            {
                level = parsedNode.Term?.Name;
            }
            else
            {
                if (!level.EndsWith(parsedNode.Term?.Name))
                {
                    level = string.Format("{0} > {1}", level, parsedNode.Term?.Name);
                }
            }

            //Determine if current node should be processed.
            switch (level)
            {
                case "Formula > FunctionCall":
                    if (parsedNode.ChildNodes[0].Term?.Name == "FunctionName")
                    {
                        //Excel Function
                        processedNode = new ExcelFunctionNode(parsedNode, ref valid, defaultWorksheet);
                    }
                    else
                    {
                        processedNode = new FunctionNode(parsedNode, ref valid, defaultWorksheet);
                    }
                    break;

                case "ReferenceFunctionCall > Reference":
                case "Formula > Reference":
                    if(parsedNode.ChildNodes.Count == 2 
                        && parsedNode.ChildNodes[0].Term?.Name == "Prefix"
                        && (parsedNode.ChildNodes[0].ChildNodes.Count(item => item.Term?.Name == "SheetNameToken" || item.Term?.Name == "SheetNameQuotedToken") > 0)
                        && parsedNode.ChildNodes[1].Term?.Name == "Cell")
                    {
                        string sheetName = parsedNode.ChildNodes[0].ChildNodes
                            .Where(item => item.Term?.Name == "SheetNameToken" || item.Term?.Name == "SheetNameQuotedToken").First().Token.Text;
                        sheetName = sheetName.Replace("'", "").Replace("!", "");
                        processedNode = new CellNode(parsedNode.ChildNodes[1].ChildNodes[0], ref valid, sheetName);
                    }
                    else
                    {
                        goto default;
                    }
                    break;

                case "ReferenceFunctionCall > Reference > Cell":
                case "Formula > Reference > Cell":
                    processedNode = new CellNode(parsedNode.ChildNodes[0], ref valid, defaultWorksheet);
                    break;

                case "Formula > Reference > ReferenceFunctionCall":
                    processedNode = new ReferenceNode(parsedNode, ref valid, defaultWorksheet);
                    break;

                case "Formula > Constant > Number":
                    processedNode = new NumberNode(parsedNode.ChildNodes[0], ref valid);
                    break;

                default:
                    if (parsedNode.ChildNodes.Count == 1)
                    {
                        processedNode = CreateNode(parsedNode.ChildNodes[0], ref valid, defaultWorksheet, level);
                    }
                    //Default - Functionality currently doesn't exist to process
                    else
                    {
                        valid = false;
                    }
                    break;
            }
            return processedNode;
        }
    }
}