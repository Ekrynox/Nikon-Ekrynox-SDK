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
    public partial class CameraShootingForm : Form
    {
        private NEKCS.NikonCamera camera;
        private SynchronizationContext _syncContext;
        private uint _sdramHandle = 0;

        public CameraShootingForm(NEKCS.NikonCamera camera)
        {
            InitializeComponent();
            _syncContext = SynchronizationContext.Current;

            this.camera = camera;
            camera.OnMtpEvent += new NEKCS.MtpEventHandler(cameraEvent);

            NEKCS.MtpParams param = new NEKCS.MtpParams();
            param.addUint32(0);
            NEKCS.MtpResponse result = camera.SendCommandAndRead((ushort)NEKCS.NikonMtpOperationCode.DeleteImagesInSdram, param);
        }


        private void cameraEvent(NEKCS.NikonCamera cam, NEKCS.MtpEvent e)
        {
            if (e.eventCode == (ushort)NEKCS.NikonMtpEventCode.ObjectAddedInSdram)
            {
                if (e.eventParams.Count > 0 && e.eventParams[0] != 0)
                {
                    _sdramHandle = e.eventParams[0];
                }
                else
                {
                    _sdramHandle = 0xFFFF0001;
                }

                NEKCS.MtpParams param = new NEKCS.MtpParams();
                param.addUint32(_sdramHandle);
                NEKCS.MtpResponse result = camera.SendCommandAndRead((ushort)NEKCS.NikonMtpOperationCode.GetObject, param);
                if (result.responseCode == (ushort)NEKCS.NikonMtpResponseCode.OK)
                {

                }
            }
        }

        private void capture_Click(object sender, EventArgs e)
        {
            NEKCS.MtpParams param = new NEKCS.MtpParams();
            param.addUint32(0xFFFFFFFF);
            NEKCS.MtpResponse result = camera.SendCommand((ushort)NEKCS.NikonMtpOperationCode.InitiateCaptureRecInSdram, param);
        }
    }
}
