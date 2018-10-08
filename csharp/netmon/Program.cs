using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.NetworkMonitor;

namespace netmon
{
    class Program
    {
        private static ParserCallbackDelegate pErrorCallBack = new ParserCallbackDelegate(ParserCallback);

        public static void ParserCallback(IntPtr pCallerContext, uint ulStatusCode, string lpDescription, NmCallbackMsgType ulType)
        {
            if (ulType == NmCallbackMsgType.Error)
            {
                throw new Exception("Error from parser callback: " + lpDescription);
            }
        }

        static void Main(string[] args)
        {
            // / / Initialize NetworkMonitor API

            NM_API_CONFIGURATION apiConfig = new NM_API_CONFIGURATION();
            apiConfig.Size = (ushort)(System.Runtime.InteropServices.Marshal.SizeOf(apiConfig));
            NetmonAPI.NmGetApiConfiguration(ref apiConfig);
            apiConfig.ThreadingMode = 0;
            NetmonAPI.NmApiInitialize(ref apiConfig);

            IntPtr nplPointer = IntPtr.Zero;
            NetmonAPI.NmLoadNplParser(null, NmNplParserLoadingOption.NmAppendRegisteredNplSets, pErrorCallBack, IntPtr.Zero, out nplPointer);


            // / / Initialize Frame parser
            IntPtr parserConfigPointer;
            NetmonAPI.NmCreateFrameParserConfiguration(nplPointer, pErrorCallBack, IntPtr.Zero, out parserConfigPointer);
            
            NetmonAPI.NmConfigConversation(parserConfigPointer, NmConversationConfigOption.None, true);

            IntPtr ParserPointer;
            NetmonAPI.NmCreateFrameParser(parserConfigPointer, out ParserPointer, NmFrameParserOptimizeOption.ParserOptimizeNone);


            // / / Parse capture file
            IntPtr captureFileHandle;
            NetmonAPI.NmOpenCaptureFile("auto.cap", out captureFileHandle);

            uint rawFrameCount;
            NetmonAPI.NmGetFrameCount(captureFileHandle, out rawFrameCount);

            uint frameNumber = 0;
            IntPtr rawFrame;
            NetmonAPI.NmGetFrame(captureFileHandle, frameNumber, out rawFrame);

            IntPtr parsedFrame;
            IntPtr insRawFrame;
            NetmonAPI.NmParseFrame(ParserPointer, rawFrame, frameNumber, NmFrameParsingOption.FieldDisplayStringRequired | NmFrameParsingOption.FieldFullNameRequired | NmFrameParsingOption.DataTypeNameRequired, out parsedFrame, out insRawFrame);

            uint fieldCount;
            NetmonAPI.NmGetFieldCount(parsedFrame, out fieldCount);

            uint BUFFER_SIZE = 1024;
            char[] name = new char[BUFFER_SIZE * 2];
            unsafe
            {
                fixed (char* pstr = name)
                {
                    NetmonAPI.NmGetFieldName(parsedFrame, 0, NmParsedFieldNames.NamePath, BUFFER_SIZE * 2, pstr);
                }
            }

            String fieldName = new String(name).Replace("\0", String.Empty);

            NetmonAPI.NmCloseHandle(captureFileHandle);
        }
    }
}
