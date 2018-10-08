using System;
using System.Windows.Forms;
using Microsoft.VisualStudio.DebuggerVisualizers;

[assembly: System.Diagnostics.DebuggerVisualizer(
    typeof(JsonCppVisualizer.DebuggerSide),
    typeof(VisualizerObjectSource),
    Target = typeof(System.String),
    Description = "JsonCppVisualizer")
]

namespace JsonCppVisualizer
{
    public class DebuggerSide : DialogDebuggerVisualizer
    {
        override protected void Show(IDialogVisualizerService windowService, IVisualizerObjectProvider objectProvider)
        {
            DialogResult result = MessageBox.Show(objectProvider.GetObject().ToString());

            // TODO: Handle result errors
        }

        public static void TestShowVisualizer(object objectToVisualize)
        {
            VisualizerDevelopmentHost visualizerHost = new VisualizerDevelopmentHost(objectToVisualize, typeof(DebuggerSide));
            visualizerHost.ShowVisualizer();
        }
    }
}
