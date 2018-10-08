using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace matrix
{
    class Program
    {
        static void Main(string[] args)
        {
            Matrix<bool> m = new Matrix<bool>(10, 10);

            m[0, 0] = true;
            m[0, 1] = true;

            Console.WriteLine(m.Count(false));

            Console.Read();
        }
    }
}

