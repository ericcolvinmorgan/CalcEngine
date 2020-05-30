using System;
using System.Collections.Generic;
using System.Text;
using XLParser;

namespace ExcelParser.Excel
{
    public class Reference
    {
        private Reference() { }
        public Reference(string name, string locationString, string maxLocation, string minLocation, string worksheet, ReferenceType type)
        {
            Worksheet = string.IsNullOrEmpty(worksheet) ? "" : worksheet;
            NamedRange = string.IsNullOrEmpty(name) ? "" : name;
            ReferenceString = string.IsNullOrEmpty(locationString) ? "" : locationString;

            //Get min row/column detail
            MinLocation = string.IsNullOrEmpty(minLocation) ? "" : minLocation;

            bool minRowAbsolute = false;
            MinRow = Helpers.RowIntFromAddress(MinLocation, ref minRowAbsolute);
            MinRowAbsolute = minRowAbsolute;

            bool minColumnAbsolute = false;
            MinColumn = Helpers.ColumnIntFromAddress(MinLocation, ref minColumnAbsolute);
            MinColumnAbsolute = minColumnAbsolute;

            //Get max row/column detail
            MaxLocation = string.IsNullOrEmpty(maxLocation) ? "" : maxLocation;

            bool maxRowAbsolute = false;
            MaxRow = Helpers.RowIntFromAddress(MaxLocation, ref maxRowAbsolute);
            MaxRowAbsolute = maxRowAbsolute;

            bool maxColumnAbsolute = false;
            MaxColumn = Helpers.ColumnIntFromAddress(MaxLocation, ref maxColumnAbsolute);
            MaxColumnAbsolute = maxColumnAbsolute;

            ReferenceType = type;
        }

        public string Worksheet { get; private set; }
        public string ReferenceString { get; private set; }
        public string MinLocation { get; private set; }
        public int MinRow { get; private set; }
        public bool MinRowAbsolute { get; private set; } = false;
        public int MinColumn { get; private set; }
        public bool MinColumnAbsolute { get; private set; } = false;
        public string MaxLocation { get; private set; }
        public int MaxRow { get; private set; }
        public bool MaxRowAbsolute { get; private set; } = false;
        public int MaxColumn { get; private set; }
        public bool MaxColumnAbsolute { get; private set; } = false;
        public string NamedRange { get; private set; }
        public ReferenceType ReferenceType { get; private set; }

        public Reference Clone(int rowOffset, int columnOffset)
        {
            Reference returnRef = new Reference();
            returnRef.Worksheet = Worksheet;

            returnRef.MinRow = MinRow + (MinRowAbsolute ? 0 : rowOffset);
            returnRef.MinRowAbsolute = MinRowAbsolute;
            returnRef.MinColumn = MinColumn + (MinColumnAbsolute ? 0 : columnOffset);
            returnRef.MinColumnAbsolute = MinColumnAbsolute;

            returnRef.MaxRow = MaxRow + (MaxRowAbsolute ? 0 : rowOffset);
            returnRef.MaxRowAbsolute = MaxRowAbsolute;
            returnRef.MaxColumn = MaxColumn + (MaxColumnAbsolute ? 0 : columnOffset);
            returnRef.MaxColumnAbsolute = MaxColumnAbsolute;
            returnRef.NamedRange = NamedRange;
            returnRef.ReferenceType = ReferenceType;

            returnRef.MinLocation = Helpers.CreateCellReference(returnRef.MinColumn, returnRef.MinRow,
                returnRef.MinColumnAbsolute, returnRef.MinRowAbsolute);
            returnRef.MaxLocation = Helpers.CreateCellReference(returnRef.MaxColumn, returnRef.MaxRow,
                returnRef.MaxColumnAbsolute, returnRef.MaxRowAbsolute);

            switch (returnRef.ReferenceType)
            {
                case ReferenceType.Cell:
                    returnRef.ReferenceString = returnRef.MinLocation;
                    break;

                case ReferenceType.CellRange:
                    returnRef.ReferenceString = string.Format("{0}:{1}", returnRef.MinLocation, returnRef.MaxLocation);
                    break;

                //Other Types Not Currently Supported.
                default:
                    returnRef.ReferenceType = ReferenceType;
                    returnRef.MinLocation = "#N/A";
                    returnRef.MinRow = -1;
                    returnRef.MinColumn = -1;
                    returnRef.MaxLocation = "#N/A";
                    returnRef.MaxRow = -1;
                    returnRef.MaxColumn = -1;
                    returnRef.ReferenceString = "#N/A";
                    break;
            }

            return returnRef;
        }
    }
}