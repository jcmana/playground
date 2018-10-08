using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Formats
{
    public class Ini
    {
        private Dictionary<String, Dictionary<String, String>> m_content;

        public Ini(String[] content)
        {
            m_content = Deserialize(content);
        }

        public override String[] ToString() 
        {
            return Serialize(m_content);
        }

        private String[] Serialize(Dictionary<String, Dictionary<String, String>> content)
        {
            String[] serialized;

            return serialized;
        }

        private Dictionary<String, Dictionary<String, String>> Deserialize(String[] content)
        {
            Dictionary<String, Dictionary<String, String>> deserialized = new Dictionary<String, Dictionary<String, String>>();

            return deserialized;
        }
    }
}
