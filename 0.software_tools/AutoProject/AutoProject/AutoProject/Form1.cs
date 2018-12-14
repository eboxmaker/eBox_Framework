using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.OleDb;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;


namespace AutoProject
{

    public partial class Form1 : Form
    {
        excleOpt excle = new excleOpt();
        DataTable dt = new DataTable();
        Target mcu = new Target();
        jsonHlep js = new jsonHlep();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

            string str = System.AppDomain.CurrentDomain.BaseDirectory;
            FileInfo file = new FileInfo("McuConfiglist.xlsx");
            string FileFullPath = file.FullName.ToString();

            dt =  excle.GetExcelToDataTableBySheet(FileFullPath,"Sheet1");
            for(int i = 0 ; i < dt.Rows.Count; i++)
            {
                comboBox1.Items.Add(dt.Rows[i][0]);
            }
            //string y = System.Text.Encoding.Default.GetString(Convert.FromBase64String(x));


            string jsonText = "{\"zone\":\"海淀\",\"zone_en\":\"haidian\"}";
            JObject jo = (JObject)JsonConvert.DeserializeObject(jsonText);
            string zone = jo["zone"].ToString();
            string FileBase64str = js.FileToBase64(FileFullPath);

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void btnUpdataConfigHeader_Click(object sender, EventArgs e)
        {
            mcu.updataMcuConfigHeader();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            textBox1.Text = comboBox1.SelectedItem.ToString();

            for (int i = 0; i < dt.Rows.Count; i++)
            {
                if (comboBox1.SelectedItem.ToString() == dt.Rows[i][0].ToString())
                {
                    int j = 0;
                    mcu.name = dt.Rows[i][j++].ToString();
                    mcu.Package = dt.Rows[i][j++].ToString();
                    mcu.FlashSzie = Convert.ToInt16(dt.Rows[i][j++]);
                    mcu.RAMSize = Convert.ToInt16(dt.Rows[i][j++]);
                    mcu.IoNum = Convert.ToInt16(dt.Rows[i][j++]);
                    mcu.Freq = Convert.ToInt16(dt.Rows[i][j++]);
                    mcu.PinNum = mcu.getPinNum();
                    mcu.FatherType = mcu.parserFatherType();

                    j = 1;
                    listBox2.Items.Clear();
                    for (int k = 1; k < dt.Columns.Count; k++)
                    {
                        listBox2.Items.Add(dt.Rows[i][j++].ToString());
                    }

                }
            }

        }



    }
}
