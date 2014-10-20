using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TextMidi2Xml
{
    class Track
    {
        private string m_name;
        private IList<float> m_soundTimes = new List<float>();

        public string Name
        {
            get { return m_name; }
            set { m_name = value; }
        }

        public IList<float> SoundTimes
        {
            get { return m_soundTimes; }
        }

        public Track()
        {
        }
    }
}
