using Communication;

namespace test
{
    class Program
    {
        static void Main(string[] args)
        {
            Server s = new Server("127.0.0.1", 55000);
            s.Listen();
        }
    }
}
