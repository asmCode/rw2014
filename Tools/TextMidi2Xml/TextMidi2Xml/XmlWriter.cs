using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TextMidi2Xml
{
    class XmlWriter
    {
        public static bool WriteTracks(IList<Track> tracks, string filename)
        {
            string xml = "<SynchEvents>\n";

            foreach (var track in tracks)
            {
                foreach (var time in track.SoundTimes)
                {
                    xml += string.Format("\t<Event name=\"{0}\" time=\"{1}\" />\n", track.Name, time);
                }
            }

            xml += "</SynchEvents>\n";

            File.WriteAllText(filename, xml);

            return true;
        }
    }
}
