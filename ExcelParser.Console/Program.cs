using System;
using System.IO;
using System.Linq;

namespace ExcelParser.Console
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                System.Console.Error.WriteLine("Error: Path to a valid *.xlsx file is required.");
                return;
            }

            //Parse Options
            var options = new Options(args);
            if (options.Invalid)
            {
                System.Console.Error.WriteLine(options.InvalidText);
                return;
            }

            //Run options not requiring file validation.
            if (options.ShowHelp)
            {
                ShowHelp();
                return;
            }

            //Validate provided file exists and process.
            
            if (!System.IO.File.Exists(options.FilePath))
            {
                System.Console.Error.WriteLine("Error: The path provided does not exist.");
            }
            else
            {
                ExcelParser.Excel.Workbook excel = new Excel.Workbook();
                excel.LoadExcel(options.FilePath);
                excel.ExtractCellDetail();
                excel.BuildCalculationChain();
                
                Stream stream;
                if(options.OutputPathDefined)
                    stream = new FileStream(options.OutputPath, FileMode.Create);
                else
                    stream = System.Console.OpenStandardOutput();

                excel.SaveAsJSON(stream);
                stream.Dispose();
            }
        }

        static void ShowHelp()
        {
            System.Console.WriteLine("USAGE: ExcelParser.Console <FilePath> [options]");
            System.Console.WriteLine("AVAILABLE OPTIONS:");
            System.Console.WriteLine($"{"<FilePath>",-15} Path to the *.xlsx file to be processed.");
            System.Console.WriteLine($"{"-h -H",-15} Show the help screen (this screen).");
            System.Console.WriteLine($"{"-o -O <Path>",-15} Desired destination path for the JSON upon successful extraction.");
        }
    }
}