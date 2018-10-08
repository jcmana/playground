using System;

namespace xml
{
    class Program
    {
        static void Main(string[] args)
        {
            System.Xml.XmlDocument doc_xml = new System.Xml.XmlDocument();
            System.Xml.Linq.XDocument doc_linq_xml = new System.Xml.Linq.XDocument();

            doc_xml.Load("hovno.xml");

            var list = doc_xml["chromiumaliases"]["alias"];
            foreach(var node in list)
            {
                Console.WriteLine(1);
                Console.WriteLine(node);
            }
            Console.ReadKey();
        }
    }
}
