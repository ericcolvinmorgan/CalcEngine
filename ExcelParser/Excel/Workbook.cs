using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DocumentFormat.OpenXml;
using DocumentFormat.OpenXml.Packaging;
using DocumentFormat.OpenXml.Spreadsheet;
using ExcelParser.CalcTree;
using Newtonsoft.Json;

namespace ExcelParser.Excel
{
    public class Workbook
    {
        private SpreadsheetDocument _spreadsheetDocument = null;
        public Dictionary<string, CellDetail> CellDetails { get; private set; }        
        public Queue<CellDetail> ProcessOrder { get; private set; }
        public bool LoadExcel(string fileLocation)
        {
            try
            {
                _spreadsheetDocument = SpreadsheetDocument.Open(fileLocation, false);
            }
            catch
            {
                return false;
            }
            return true;
        }

        public bool ExtractCellDetail()
        {
            CellDetails = new Dictionary<string, CellDetail>();
            var sheets = _spreadsheetDocument.WorkbookPart.Workbook.Descendants<Sheet>();
            
            //This needs a check as this itme doesn't always exist.
            OpenXmlElement[] stringTable = _spreadsheetDocument.WorkbookPart.GetPartsOfType<SharedStringTablePart>().FirstOrDefault()?.SharedStringTable.ChildElements.ToArray();
            
            Dictionary<string, CellDetail> sharedCells = new Dictionary<string, CellDetail>();

            //Extract cell detail from worksheets, pre-process master shared cells.
            foreach (Sheet sheet in sheets)
            {
                // Retrieve a reference to the worksheet part.
                string sheetName = sheet.Name;
                WorksheetPart wsPart = (WorksheetPart)(_spreadsheetDocument.WorkbookPart.GetPartById(sheet.Id));
                var worksheet = wsPart.Worksheet;
                Parallel.ForEach(wsPart.Worksheet.Elements<SheetData>().First().Elements<Row>(), r =>
                {
                    Parallel.ForEach(r.Elements<Cell>(), cell =>
                    {
                        if (cell.CellValue != null)
                        {
                            CellDetail cellDetail = new CellDetail(sheetName, cell, stringTable);

                            if (cellDetail.Type == CellDetail.CellType.Formula && cellDetail.Formula.IsMasterShared)
                            {
                                cellDetail.ParseFormulas();
                                lock (sharedCells)
                                {
                                    sharedCells.Add(string.Format("{0}-{1}", sheetName, cell.CellFormula.SharedIndex), cellDetail);
                                }
                            }

                            lock (CellDetails)
                            {
                                CellDetails.Add(cellDetail.WorksheetCell, cellDetail);
                            }
                        }
                    });
                });
            }

            //Process remainder of cells
            Parallel.ForEach(CellDetails.Values, cell =>
            {
                if (cell.Type == CellDetail.CellType.Formula
                && cell.Formula.Type == CellDetail.FormulaDetail.FormulaType.SharedFormula)
                {
                    if (!cell.Formula.IsMasterShared)
                    {
                        cell.CreateFormulaTreeFromShared(sharedCells);
                    }
                }
                else
                {
                    cell.ParseFormulas();
                }
            });

            return true;
        }
        public bool BuildCalculationChain()
        {
            //Find all nodes that have dependencies, that are not dependencies of other nodes.
            CalcChainManager chainManager = new CalcChainManager(CellDetails);
            chainManager.BuildCalcChain();
            ProcessOrder = chainManager.ProcessedItems;
            return true;
        }

        public bool SaveAsJSON()
        {
            using (var sw = new StreamWriter(@"C:\Users\ericm\Documents\source\repos\CalcEngine\ExcelParser.Tests\TestFiles\TestFile.json"))
            using (var writer = new JsonTextWriter(sw))
            {
                var ser = new JsonSerializer();

                writer.WriteStartObject();
                writer.WritePropertyName("CellDetails");
                writer.WriteStartObject();

                foreach (var item in this.CellDetails)
                {
                    writer.WritePropertyName(item.Key);
                    ser.Serialize(writer, new
                    {
                        WorksheetCell = item.Value.WorksheetCell,
                        Worksheet = item.Value.Worksheet,
                        Cell = item.Value.Cell,
                        Row = item.Value.Row,
                        Column = item.Value.Column,
                        Formula = item.Value.Formula,
                        Value = item.Value.Value,
                        Type = item.Value.Type,
                        FormulaTreeValid = item.Value.FormulaTreeValid,
                        FormulaTree = item.Value.FormulaTree
                    });
                    writer.Flush();
                }
                writer.WriteEndObject();

                writer.WritePropertyName("ProcessOrder");
                writer.WriteStartArray();

                foreach(var item in this.ProcessOrder)
                {
                    ser.Serialize(writer, new { WorksheetCell = item.WorksheetCell });
                    writer.Flush();
                }

                writer.WriteEnd();
                writer.WriteEndObject();
                writer.Flush();
            }

            return true;
        }
    }
}