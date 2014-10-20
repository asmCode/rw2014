using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace TextMidi2Xml
{
    class TextMidiParser
    {
        private float m_division;
        private float m_tempo;
        private int m_lineError;
        private bool m_insideTrack;

        Track m_currentTrack;

        private IList<Track> m_tracks;

        public IList<Track> Tracks
        {
            get { return m_tracks; }
        }

        public int LineError
        {
            get { return m_lineError; }
        }

        public bool Load(string file)
        {
            StreamReader reader = File.OpenText(file);
            if (reader == null)
                return false;

            m_tracks = new List<Track>();

            string line;

            while (true)
            {
                m_lineError++;

                line = reader.ReadLine();
                if (line == null)
                    break;

                //string[] values = line.Split(' ');
                string[] values = Regex.Matches(line, @"[\""].+?[\""]|[^ ]+")
                    .Cast<Match>()
                    .Select(m => m.Value)
                    .ToArray<string>();

                for (int i = 0; i < values.Length; i++)
                {
                    values[i] = values[i].Trim();
                }

                if (!ParseLine(values))
                {
                    return false;
                }
            }

            return true;
        }

        private bool ParseLine(string[] values)
        {
            if (values.Length == 0)
                return true;

            if (values[0] == "MFile")
                return ParseMFile(values);
            if (values[0] == "MTrk")
                return ParseMTrk(values);
            if (values[0] == "TrkEnd")
                return ParseTrkEnd(values);

            if (m_insideTrack)
                return ParseTrackElement(values);

            return false;
        }

        private bool ParseMFile(string[] values)
        {
            if (values.Length != 4)
                return false;

            m_division = float.Parse(values[3]);

            return true;
        }

        private bool ParseMTrk(string[] values)
        {
            if (values.Length != 1)
                return false;

            m_currentTrack = new Track();

            m_insideTrack = true;

            return true;
        }

        private bool ParseTrkEnd(string[] values)
        {
            if (values.Length != 1)
                return false;

            m_tracks.Add(m_currentTrack);

            m_currentTrack = null;

            m_insideTrack = false;

            return true;
        }

        private bool ParseTrackElement(string[] values)
        {
            if (values.Length < 2)
                return false;

            float ticks = float.Parse(values[0]);
            string type = values[1];

            if (type == "Meta")
            {
                string metaType = values[2];

                if (metaType == "TrkName")
                    m_currentTrack.Name = values[3].Substring(1, values[3].Length - 2);
            }

            if (type == "Tempo")
                m_tempo = float.Parse(values[2]);

            if (type == "On")
            {
                m_currentTrack.SoundTimes.Add(CalculateRealTime(ticks));
            }

            return true;
        }

        private float CalculateRealTime(float ticks)
        {
            return (60 / ((60000000 / m_tempo) * m_division)) * ticks;
        }
    }
}
