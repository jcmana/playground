using System;
using System.Runtime.InteropServices;

namespace ps_cpp_dll_loader
{
    public class Loader
    {
        [DllImport("avglngx.dll", EntryPoint = "lmc1_Initial2", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
    }
}
