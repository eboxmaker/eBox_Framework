using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutoProject
{
    class jsonHlep
    {

        public string FileToBase64(string FilePath)
        {
            FileStream fs = new FileStream(FilePath,FileMode.Open,FileAccess.Read);
            int nBytes = (int)fs.Length;
            byte[] byteArray = new byte[nBytes];
            int nByteReaded = fs.Read(byteArray, 0, nBytes);
            string x = Convert.ToBase64String(byteArray);
            return x;
        }
    }
}
