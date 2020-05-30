using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace ExcelParser.Tests
{
    //If a single cell is referenced, and that cell does not exist, how should we handle?    
    //Test array references to ensure adding to stack and marking as unprocessed is sound logic.

    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
            ExcelParser.Excel.Workbook excel = new Excel.Workbook();
            //Assert.IsTrue(excel.LoadExcel("../../../TestFiles/Basic1.xlsx"));
            Assert.IsTrue(excel.LoadExcel(@"C:\Users\ericm\TestingFiles\EPCalcTemplate-LessComplex.xlsx"));
            excel.ExtractCellDetail();
            excel.BuildCalculationChain();
            excel.SaveAsJSON();

            foreach(var detailItem in excel.CellDetails.Values)
            {
                Assert.IsTrue(detailItem.FormulaTreeValid);
            }
        }

        [TestMethod]
        public void ColumnIntFromAddress()
        {
            bool absolute = false;
            Assert.AreEqual(1, Excel.Helpers.ColumnIntFromAddress("$A$1", ref absolute));
            Assert.AreEqual(26, Excel.Helpers.ColumnIntFromAddress("$Z$1", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(16384, Excel.Helpers.ColumnIntFromAddress("$XFD$1", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(16384, Excel.Helpers.ColumnIntFromAddress("$xfd$1", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(16384, Excel.Helpers.ColumnIntFromAddress("$xFd$1", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(16384, Excel.Helpers.ColumnIntFromAddress("xFd1", ref absolute));
            Assert.IsFalse(absolute);
            Assert.AreEqual(16384, Excel.Helpers.ColumnIntFromAddress("xFd$1", ref absolute));
            Assert.IsFalse(absolute);
            Assert.AreEqual(46, Excel.Helpers.ColumnIntFromAddress("$AT$1", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(46, Excel.Helpers.ColumnIntFromAddress("AT$1", ref absolute));
            Assert.IsFalse(absolute);
            Assert.AreEqual(46, Excel.Helpers.ColumnIntFromAddress("$aT$1", ref  absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(46, Excel.Helpers.ColumnIntFromAddress("At1", ref absolute));
            Assert.IsFalse(absolute);
            Assert.AreEqual(46, Excel.Helpers.ColumnIntFromAddress("at$1", ref absolute));
            Assert.IsFalse(absolute);
            Assert.AreEqual(-1, Excel.Helpers.ColumnIntFromAddress("aBcD$1", ref absolute));
            Assert.IsFalse(absolute);
        }

        [TestMethod]
        public void ColumnAlphaFromInt()
        {
            Assert.AreEqual(null, Excel.Helpers.ColumnAlphaFromInt(0));
            Assert.AreEqual(null, Excel.Helpers.ColumnAlphaFromInt(16385));
            Assert.AreEqual("XFD", Excel.Helpers.ColumnAlphaFromInt(16384));
            Assert.AreEqual("A", Excel.Helpers.ColumnAlphaFromInt(1));
            Assert.AreEqual("CZP", Excel.Helpers.ColumnAlphaFromInt(2720));
            Assert.AreEqual("ZZ", Excel.Helpers.ColumnAlphaFromInt(702));
            Assert.AreEqual("AA", Excel.Helpers.ColumnAlphaFromInt(27));
        }

        [TestMethod]
        public void RowIntFromAddress()
        {
            bool absolute = false;
            Assert.AreEqual(1798, Excel.Helpers.RowIntFromAddress("$A1798", ref absolute));
            Assert.IsFalse(absolute);
            Assert.AreEqual(1798, Excel.Helpers.RowIntFromAddress("$A$1798", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(2, Excel.Helpers.RowIntFromAddress("$Z$2", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(16384, Excel.Helpers.RowIntFromAddress("$XFD$16384", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(1, Excel.Helpers.RowIntFromAddress("$xfd$1", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(163, Excel.Helpers.RowIntFromAddress("$xFd$163", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(1048576, Excel.Helpers.RowIntFromAddress("$xFd$1048576", ref absolute));
            Assert.IsTrue(absolute);
            Assert.AreEqual(-1, Excel.Helpers.RowIntFromAddress("$xFd$010", ref absolute));
            Assert.IsFalse(absolute);
        }
    }
}
