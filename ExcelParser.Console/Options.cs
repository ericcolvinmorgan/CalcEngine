using System;
using System.IO;
using System.Linq;

namespace ExcelParser.Console
{
    public class Options
    {
        public bool Invalid { get; set; } = false;
        public string InvalidText {get;set;} = "";
        public string FilePath { get; set; } = "";
        public bool ShowHelp { get; set; } = false;
        public bool OutputPathDefined { get; set; } = false;
        public string OutputPath { get; set; } = "";
        public Options(string[] args)
        {
            //Determine if help menu should be printed.
            if (args.Any(entry => entry.ToUpper() == "-H"))
            {
                ShowHelp = true;
                return;
            }

            //Process Args
            FilePath = args[0];
            for (int a = 1; a < args.Length; a++)
            {
                switch (args[a].ToUpper())
                {
                    case "-O":
                        OutputPathDefined = true;
                        a++;
                        if(a < args.Length && !args[a].StartsWith('-'))
                            OutputPath = args[a];
                        else
                        {
                            Invalid = true;
                            InvalidText = $"Error: Invalid Option: -O - No output path was provided.";
                            return;
                        }
                        break;

                    default:
                        Invalid = true;
                        InvalidText = $"Error: Invalid Option: {args[a]}";
                        return;
                }
            }
        }
    }
}