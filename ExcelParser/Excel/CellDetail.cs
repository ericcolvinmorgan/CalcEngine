using DocumentFormat.OpenXml;
using DocumentFormat.OpenXml.Spreadsheet;
using ExcelParser.CalcTree;
using Irony.Parsing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using XLParser;

namespace ExcelParser.Excel
{
    public class CellDetail
    {
        public class FormulaDetail
        {
            public enum FormulaType { Formula, SharedFormula }

            public string Formula { get; private set; }
            public FormulaType Type { get; private set; }
            public int SharedIndex { get; private set; } = -1;
            public bool IsMasterShared { get; private set; } = false;
            public FormulaDetail(string formula)
            {
                Formula = formula;
                Type = FormulaType.Formula;
                SharedIndex = -1;
            }

            //NO UNIT TEST CURRENTLY WRITTEN
            public FormulaDetail(string formula, UInt32Value sharedIndex)
            {
                Formula = formula;
                Type = FormulaType.SharedFormula;
                SharedIndex = !sharedIndex.HasValue ? -1 : (int)sharedIndex.Value;
                IsMasterShared = (formula != "");
            }
        }

        public enum CellType { Formula, Value, String }

        public string WorksheetCell { get; private set; }
        public string Worksheet { get; private set; }
        public string Cell { get; private set; }
        public int Row { get; private set; }
        public int Column { get; private set; }
        public FormulaDetail Formula { get; set; }
        public string Value { get; set; }
        public CellType Type { get; set; }
        public List<Reference> References { get; private set; } = new List<Reference>();
        public bool FormulaTreeValid { get; private set; }
        public Node FormulaTree { get; private set; }
        public CellDetail(string workSheet, Cell cell, OpenXmlElement[] stringTable)
        {
            Worksheet = workSheet;
            Cell = cell.CellReference;
            WorksheetCell = string.Format("{0}-{1}", workSheet, cell.CellReference);
            bool rowAbsolute = false;
            Row = Helpers.RowIntFromAddress(Cell);
            Column = Helpers.ColumnIntFromAddress(Cell);

            Value = cell.CellValue.InnerText;
            
            //Determine If Shared Formula
            if (cell.CellFormula != null)
            {
                Type = CellDetail.CellType.Formula;
                if (cell.CellFormula?.FormulaType?.Value == DocumentFormat.OpenXml.Spreadsheet.CellFormulaValues.Shared)
                {
                    Formula = new FormulaDetail(cell.CellFormula?.InnerText, cell.CellFormula?.SharedIndex);
                }
                else
                {
                    Formula = new FormulaDetail(cell.CellFormula?.InnerText);
                }
            }
            else
            {
                Type = CellDetail.CellType.Value;
                if (cell.DataType != null)
                {
                    switch (cell.DataType.Value)
                    {
                        case CellValues.SharedString:
                            if (stringTable != null)
                            {
                                Value = stringTable[int.Parse(Value)].InnerText;
                                Type = CellDetail.CellType.String;
                            }
                            break;
                    }
                }
            }
        }

        //!!!!! - FOLLOWING NOT CURRENTLY SUPPORTED:
        //1 - WE SHOULD NOT ALLOW WORKBOOKS WITH REFERENCES TO OTHER WORKBOOKS
        //2 - Named ranges
        //3 - Ranges across multiple sheets
        //4 - Horizontal and vertical ranges
        public void ParseFormulas()
        {
            if (Type == CellType.Formula)
            {
                if (Formula.Type == FormulaDetail.FormulaType.SharedFormula
                    && !Formula.IsMasterShared)
                {
                    //Mark non-master shared cells as valid.
                    FormulaTree = null;
                    FormulaTreeValid = true;
                }
                else
                {
                    //Build Calculation Tree
                    ParseTreeNode parsedFormula = null;
                    parsedFormula = ExcelFormulaParser.Parse(Formula.Formula);
                    
                    if (parsedFormula != null)
                    {
                        FormulaAnalyzer analyzer = new XLParser.FormulaAnalyzer(parsedFormula);

                        //Build References List
                        List<ParserReference> references = analyzer.ParserReferences().ToList();
                        references.ForEach(item => References.Add(new Reference(
                            item.Name,
                            item.LocationString,
                            item.MaxLocation,
                            item.MinLocation,
                            string.IsNullOrEmpty(item.Worksheet) ? Worksheet : item.Worksheet,
                            item.ReferenceType)));

                        //Build BST Calc Tree
                        bool valid = true;
                        Node cellNode = Node.CreateNode(parsedFormula, ref valid, Worksheet);
                        FormulaTree = cellNode;
                        FormulaTreeValid = valid;
                    }
                }
            }
            else //Value Only
            {
                FormulaTreeValid = true;
            }
        }

        public void CreateFormulaTreeFromShared(Dictionary<string, CellDetail> sharedCells)
        {
            CellDetail masterCell = sharedCells[string.Format("{0}-{1}", Worksheet, Formula.SharedIndex)];

            //Determine Offset
            int rowOffset = Row - masterCell.Row;
            int columnOffset = Column - masterCell.Column;

            FormulaTree = masterCell.FormulaTree.Clone(rowOffset, columnOffset);
            FormulaTreeValid = masterCell.FormulaTreeValid;

            //Copy References
            foreach(Reference reference in masterCell.References)
            {
                References.Add(reference.Clone(rowOffset, columnOffset));
            }

            //Check validity
            Stack<Node> toAnalyze = new Stack<Node>();
            Stack<Node> toProcess = new Stack<Node>();

            //Determine post-order transversal
            toAnalyze.Push(FormulaTree);
            while (toAnalyze.Count > 0)
            {
                Node currentNode = toAnalyze.Pop();
                toProcess.Push(currentNode);
                foreach (Node child in currentNode.ChildNodes())
                {
                    toAnalyze.Push(child);
                }
            }

            //Check Childnodes
            while (toProcess.Count > 0)
            {
                Node currentNode = toProcess.Pop();
                if(!currentNode.IsValid)
                {
                    this.FormulaTreeValid = false;
                }
            } 
        }
    }
}