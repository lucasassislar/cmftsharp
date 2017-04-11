using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using cmftclr;

namespace CmftSharpGui
{
    public partial class MainForm : Form
    {
        private OpenFileDialog open;
        private CmftImage image;

        public MainForm()
        {
            InitializeComponent();

            open = new OpenFileDialog();
            open.Filter = "All files|*.*|EXR Files|*.exr";
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (open.ShowDialog() == DialogResult.OK)
            {
                string place = open.FileName;
                image = new CmftImage(place, TextureFormat.RGBA32F);

                if (!image.IsCubemap())
                {
                    if (image.IsLatLong())
                    {

                    }
                    else if (image.IsHorizontalStrip() ||
                        image.IsVerticalStrip())
                    {
                        image.ToCubemapFromStrip();
                    }
                    else if (image.IsOctant())
                    {

                    }
                }

                if (!image.IsCubemap())
                {
                    // conversion failed
                    
                }

                int size = image.Width();
                image.DoIrradianceFilterSh(size);
                image.Save(@"test.hdr", ImageFileType.HDR, OutputType.HCross, TextureFormat.RGBE);
            }
        }
    }
}
