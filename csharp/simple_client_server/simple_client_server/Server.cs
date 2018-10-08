using System;
using System.Collections.Generic;
using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace Communication
{
    public class Server
    {
        private IPAddress           m_address;
        private int                 m_port;
        private TcpListener         m_listener;

        private List<ManualResetEvent> m_e_manualReset;

        public Server()
        {
            m_address = null;
            m_port = 0;
            m_listener = null;
            m_e_manualReset = new List<ManualResetEvent>();
        }

        public Server(IPAddress address, int port) : this()
        {
            m_address = address;
            m_port = port;
        }

        public Server(String address, int port) : this(IPAddress.Parse(address), port)
        {
        }

        public void Listen()
        {
            // check if we aren't listening already
            if (m_listener != null)
            {
                throw new Exception("Server is already listening");
            }

            // register interrupt signal handler
            Console.CancelKeyPress += new ConsoleCancelEventHandler(Cancel);

            // listen at the specified IP and port
            m_listener = new TcpListener(m_address, m_port);
            m_listener.Start();

            Console.WriteLine("Listening...");

            while (true)
            {
                // wait for incoming client request (blocking call)
                TcpClient client = m_listener.AcceptTcpClient();

                // setup manual reset event
                ManualResetEvent mre = new ManualResetEvent(false);
                m_e_manualReset.Add(mre);

                // create new client handling thread
                ServerThread thread = new ServerThread(client, mre);

                // queue thread in thread pool
                ThreadPool.QueueUserWorkItem(thread.Routine);
            }
        }

        public void Stop()
        {
            Console.WriteLine("Stopping server threads...");

            // stop listening
            m_listener.Stop();
            m_listener = null;

            // wait for threads
            if (m_e_manualReset.Count > 0)
            {
                WaitHandle.WaitAll(m_e_manualReset.ToArray());
            }
        }

        private void Cancel(object sender, ConsoleCancelEventArgs args)
        {
            Console.WriteLine("Interrupt signal received");
            Stop();
        }


        private class ServerThread
        {
            private TcpClient       m_client;
            private NetworkStream   m_stream;

            private Byte[]          m_request;
            private int             m_requestSize;

            private Byte[]          m_response;
            private int             m_responseSize;

            private ManualResetEvent m_manualResetEvent;

            public ServerThread(TcpClient client, ManualResetEvent manualResetEvent)
            {
                m_client = client;
                e_started = null;
                e_finished = null;
                m_manualResetEvent = manualResetEvent;
            }

            public void Routine(Object context)
            {
                StartedEvent();

                Receive();
                Handle();
                Respond();

                FinishedEvent();
                m_manualResetEvent.Set();
            }

            private void Receive()
            {
                // get the incoming data through a network stream
                m_stream = m_client.GetStream();

                // read incoming stream
                m_request = new Byte[m_client.ReceiveBufferSize];
                m_requestSize = m_stream.Read(m_request, 0, m_client.ReceiveBufferSize);
            }

            private void Handle()
            {
                // convert the data received into a string
                String text = System.Text.Encoding.ASCII.GetString(m_request, 0, m_requestSize);

                // DEBUG: respond with request
                m_response = m_request;
                m_responseSize = m_requestSize;
            }

            private void Respond()
            {
                // write back the text to the client
                m_stream.Write(m_response, 0, m_responseSize);
            }


            // class events
            public delegate void StartedEventHandler();
            public event StartedEventHandler e_started;
            public StartedEventHandler OnStarted(StartedEventHandler handler)
            {
                StartedEventHandler e = new StartedEventHandler(handler);
                e_started += e;
                return e;
            }
            private void StartedEvent()
            {
                if (e_started != null)
                {
                    e_started.Invoke();
                }
            }

            public delegate void FinishedEventHandler();
            public event FinishedEventHandler e_finished;
            public void OnFinished(FinishedEventHandler handler)
            {
                e_finished += new FinishedEventHandler(handler);
            }
            public void FinishedEvent()
            {
                if (e_finished != null)
                {
                    e_finished.Invoke();
                }
            }
        }
    }
}
