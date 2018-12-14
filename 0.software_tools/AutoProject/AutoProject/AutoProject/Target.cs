using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace AutoProject
{
    class Target
    {

        public string FatherType;
        public string name;
        public string Package;
        public int FlashSzie;
        public int RAMSize;
        public int PinNum;
        public int IoNum;
        public int Freq;

        public int getPinNum()
        {
            string result = System.Text.RegularExpressions.Regex.Replace(Package, @"[^0-9]+", "");
            PinNum = Convert.ToInt16(result);
            return PinNum;
        }
        public int parserStringToInt(string str)
        {

            string result = System.Text.RegularExpressions.Regex.Replace(str, @"[^0-9]+", "");
            int value = Convert.ToInt16(result);
            return value;
        }
        public int getMcuSeries()
        {
            string series = name.Substring(6,3);
            int ret = Convert.ToInt16(series);
            return ret;
        }
        public string parserFatherType()
        {
            if (getMcuSeries() == 105 || getMcuSeries() == 107)
            {
                FatherType = "STM32F10X_CL";
            }
            else if (getMcuSeries() == 100)
            {
                if(FlashSzie < 64)
                {
                    FatherType = "STM32F10X_LD_VL";
                }
                else if(FlashSzie ==64 || FlashSzie == 128)
                {
                    FatherType = "STM32F10X_MD_VL";
                }
                else
                {
                    FatherType = "STM32F10X_HD_VL";
                }
            }
            else if(FlashSzie < 64)
            {
                FatherType = "STM32F10X_LD";
            }
            else if(FlashSzie ==64 || FlashSzie == 128)
            {
                FatherType = "STM32F10X_MD";
            }
            else 
            {
                FatherType = "STM32F10X_HD";
            }
            return FatherType;
        }
        public void updataMcuConfigHeader()
        {
            int config_begin_line = 0;
            int config_end_line = 0;

            FileInfo file = new FileInfo("mcu_config.h");
            string FileFullPath = file.FullName.ToString();


            string[] filelist = File.ReadAllLines(FileFullPath, Encoding.Default);//Path，文件路径
            for (int line_num = filelist.Length - 1; line_num >= 0; line_num--)
            {
                int index = filelist[line_num].IndexOf("/* MCU CONFIG BEGIN */");
                if (index > -1)//key,要查找的字符
                {
                    config_begin_line = line_num;
                    //做相应操作，如textbox.Text=filelist[linenum];
                    break;
                }
            }


            for (int line_num = filelist.Length - 1; line_num >= 0; line_num--)
            {
                int index = filelist[line_num].IndexOf("/* MCU CONFIG END */");
                if (index > -1)//key,要查找的字符
                {
                    config_end_line = line_num;
                    //做相应操作，如textbox.Text=filelist[linenum];
                    break;
                }
            }
            int config_line = config_begin_line ;
            filelist[config_line++] = "/* MCU CONFIG BEGIN */";
            filelist[config_line++] = "#define STM32_TYPE    " + name;
            filelist[config_line++] = "#define STM32_PINS    " + PinNum.ToString();
            filelist[config_line++] = "#define STM32_FLASH   " + FlashSzie.ToString() ;
            filelist[config_line++] = "#define STM32_RAM1    " + RAMSize.ToString() ;
            filelist[config_line++] = "#define STM32_COMPANY \"ST\\0\"";
            filelist[config_line++] = "#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */";
            filelist[config_line++] = "#define " + FatherType;
            filelist[config_line++] = "/* MCU CONFIG END */";

            
            File.WriteAllLines(FileFullPath,filelist);

        }
        void writeToFile(string[] str,string path)
        {

        }
    }
}
