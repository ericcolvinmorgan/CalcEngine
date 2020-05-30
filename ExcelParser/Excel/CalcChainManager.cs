using System;
using System.Collections.Generic;
using System.Text;

namespace ExcelParser.Excel
{
    public class CalcChainManager
    {
        public enum Status { Unvisited, Visited, Finished }
        public class CalcChainItem
        {
            public Status ItemStatus { get; set; }
            public CellDetail CellDetail { get; set; }
            public List<Reference> UnprocessedReferences { get; set; } = new List<Reference>();
            public CalcChainItem(CellDetail cell)
            {
                CellDetail = cell;
                ItemStatus = Status.Unvisited;
                UnprocessedReferences.AddRange(cell.References);
            }
        }

        private Dictionary<string, CalcChainItem> CalcChainItems { get; set; } = new Dictionary<string, CalcChainItem>();
        private Stack<CalcChainItem> Processing { get; set; } = new Stack<CalcChainItem>();
        public Queue<CellDetail> ProcessedItems { get; private set; } = new Queue<CellDetail>();

        public CalcChainManager(Dictionary<string, CellDetail> cellDetails)
        {
            foreach(var entry in cellDetails)
            {
                CalcChainItem chainItem = new CalcChainItem(entry.Value);
                CalcChainItems.Add(entry.Key, chainItem);
            }
        }

        public void BuildCalcChain()
        {
            foreach (CalcChainItem cell in CalcChainItems.Values)
            {
                //Ensure item has not been previously processed.
                if (cell.ItemStatus == Status.Unvisited)
                {
                    cell.ItemStatus = Status.Visited;
                    Processing.Push(cell);
                    while (Processing.Count > 0)
                    {
                        //Get current item on top of stack
                        CalcChainItem currentCalc = Processing.Peek();

                        //Mark as visited if unvisited.
                        if(currentCalc.ItemStatus == Status.Unvisited)
                        {
                            currentCalc.ItemStatus = Status.Visited;
                        }

                        //Process Dependent Cells
                        if (currentCalc.UnprocessedReferences.Count > 0)
                        {
                            //Process Various Types of reference, this should be refactored out of this procedure.
                            Reference currentRef = currentCalc.UnprocessedReferences[0];
                            switch (currentRef.ReferenceType)
                            {
                                case XLParser.ReferenceType.Cell:
                                    {
                                        CalcChainItem refItem;
                                        if (CalcChainItems.TryGetValue(string.Format("{0}-{1}", currentRef.Worksheet, currentRef.ReferenceString), out refItem))
                                        {
                                            if (refItem.ItemStatus == Status.Unvisited)
                                            {
                                                Processing.Push(refItem);
                                            }
                                            if (refItem.ItemStatus == Status.Visited)
                                            {
                                                //IF VISITED THEN CIRCULAR
                                                int i = 0;
                                            }
                                        }
                                    }
                                    break;

                                case XLParser.ReferenceType.CellRange:
                                case XLParser.ReferenceType.HorizontalRange:
                                case XLParser.ReferenceType.VerticalRange:
                                    {
                                        for (int row = currentRef.MinRow; row <= currentRef.MaxRow; row++)
                                        {
                                            for (int col = currentRef.MinColumn; col <= currentRef.MaxColumn; col++)
                                            {
                                                CalcChainItem refItem;
                                                if (CalcChainItems.TryGetValue(string.Format("{0}-{1}{2}", currentRef.Worksheet, Helpers.ColumnAlphaFromInt(col), row), out refItem))
                                                {
                                                    if (refItem.ItemStatus == Status.Unvisited)
                                                    {
                                                        Processing.Push(refItem);
                                                    }

                                                    if(refItem.ItemStatus == Status.Visited)
                                                    {
                                                        //IF VISITED THEN CIRCULAR
                                                        int i = 0;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    break;
                            }
                            currentCalc.UnprocessedReferences.RemoveAt(0);
                        }
                        else //Process Current Calc Item
                        {
                            currentCalc.ItemStatus = Status.Finished;
                            Processing.Pop();
                            ProcessedItems.Enqueue(currentCalc.CellDetail);
                        }
                    }
                }
            }
        }
    }
}
