using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace NEKCS.TestApp
{
    public partial class CameraEventListener : Form
    {
        private NEKCS.NikonCamera camera;
        private SynchronizationContext _syncContext;


        public CameraEventListener(string devicePath)
        {
            InitializeComponent();
            _syncContext = SynchronizationContext.Current;

            camera = new NEKCS.NikonCamera(devicePath);
            camera.OnMtpEvent += new NEKCS.MtpEventHandler(newCamEvent);
        }

        void newCamEvent(NEKCS.NikonCamera cam, NEKCS.MtpEvent e)
        {
            _syncContext.Post(_ => {
                this.EventList.Text += "Event: " + e.eventCode + "\n";
            }, null);
        }
    }
}
