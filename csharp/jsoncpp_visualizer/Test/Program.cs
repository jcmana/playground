using System;
using JsonCppVisualizer;

namespace JsonCppVisualizerTest
{
    class Program
    {
        static void Main(string[] args)
        {
            String myString = "Hello, World";
            DebuggerSide.TestShowVisualizer(myString);
        }
    }
}
