using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace SimpleService
{
    public partial class SimpleService : ServiceBase
    {
        private class ScheduleData
        {
            public string name { get; set; }
            public Timer timer  { get; set; }

            public ScheduleData(string name, Timer timer)
            {
                this.name = name;
                this.timer = timer;
            }
        };

        private List<ScheduleData> serviceTimerList;

       
        public SimpleService()
        {
            InitializeComponent();

            this.ServiceName = "Simple Service";
            this.EventLog.Log = "Application";

            this.Log("Service 'SimpleService' is up and running", EventLogEntryType.Information);
        }

        static void Main()
        {
            ServiceBase.Run(new SimpleService());
        }

        protected override void OnStart(string[] args)
        {

        }

        protected override void OnStop()
        {

        }

        private void Schedule(string scheduleName, int schedulTime, Delegate scheduleHandler)
        {
            ScheduleData scheduleEntry = new ScheduleData(scheduleName, new Timer(2123156465));
            scheduleEntry.timer.Elapsed += scheduleHandler;

            serviceTimerList.Add(scheduleEntry);
        }

        private void Log(string serviceMessage, EventLogEntryType serviceSeverity)
        {
            this.EventLog.WriteEntry(serviceMessage, EventLogEntryType.Information);
        }
    }
}
