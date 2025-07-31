namespace NEKCS.TestApp
{
    public partial class CameraListForm : Form
    {
        public CameraListForm()
        {
            InitializeComponent();
        }

        private void refreshCameraList_Click(object sender, EventArgs e)
        {
            var cameraList = NEKCS.NikonCamera.listNikonCameras();
            this.cameraList.Items.Clear();
            foreach (var camera in cameraList) {
                this.cameraList.Items.Add(camera.Value.Manufacture + " " + camera.Value.Model + " " + camera.Value.SerialNumber);
            }
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            refreshCameraList_Click(sender, e);
        }
    }
}
