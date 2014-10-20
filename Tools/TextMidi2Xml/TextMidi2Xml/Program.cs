using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace TextMidi2Xml
{
    class Program
    {
        private static void PrintUsage()
        {
            Console.WriteLine("Usage: TextMidi2Xml.exe input_file output_file");
        }

        static void Main(string[] args)
        {
            if (args.Length != 2 || args[0].Length == 0 || args[1].Length == 0)
            {
                PrintUsage();
                return;
            }

            if (!File.Exists(args[0]))
            {
                Console.WriteLine("File '" + args[0] + "' doesn't exist");
                return;
            }

            TextMidiParser parser = new TextMidiParser();
            if (!parser.Load(args[0]))
            {
                Console.WriteLine("Couldn't parse input file. Error at line: " + parser.LineError.ToString());
                return;
            }

            if (!XmlWriter.WriteTracks(parser.Tracks, args[1]))
            {
                Console.WriteLine("Couldn't write xml file");
                return;
            }

            Console.WriteLine("Successfully finished");
        }
    }
}
