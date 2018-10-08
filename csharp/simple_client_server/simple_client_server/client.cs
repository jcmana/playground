using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace Communication
{
    public class Client
    {
        public void Send()
        {
            // data to send to the server
            string textToSend = DateTime.Now.ToString();

            // create a TCPClient object at the IP and port no
            TcpClient client = new TcpClient("127.0.0.1", 55000);
            NetworkStream nwStream = client.GetStream();
            byte[] bytesToSend = ASCIIEncoding.ASCII.GetBytes(textToSend);

            // send the text
            Console.WriteLine("Sending : " + textToSend);
            nwStream.Write(bytesToSend, 0, bytesToSend.Length);

            // read back the text
            byte[] bytesToRead = new byte[client.ReceiveBufferSize];
            int bytesRead = nwStream.Read(bytesToRead, 0, client.ReceiveBufferSize);
            Console.WriteLine("Received : " + Encoding.ASCII.GetString(bytesToRead, 0, bytesRead));
            Console.ReadLine();
            client.Close();
        }
    }
}
