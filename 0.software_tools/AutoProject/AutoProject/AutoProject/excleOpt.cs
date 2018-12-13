using System;
using System.Collections.Generic;
using System.Data;
using System.Data.OleDb;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutoProject
{
    class excleOpt
    {

        public  DataTable GetExcelToDataTableBySheet(string FileFullPath, string SheetName)
        {

            //此连接只能操作Excel2007之前(.xls)文件

           // string strConn = "Provider=Microsoft.Jet.OleDb.4.0;" + "data source=" + FileFullPath + ";Extended Properties='Excel 8.0; HDR=NO; IMEX=1'";

            //此连接可以操作.xls与.xlsx文件

            string strConn = "Provider=Microsoft.Ace.OleDb.12.0;" + "data source=" + FileFullPath + ";Extended Properties='Excel 12.0; HDR=NO; IMEX=1'";


            OleDbConnection conn = new OleDbConnection(strConn);

            conn.Open();

            DataSet ds = new DataSet();

           // OleDbDataAdapter odda = new OleDbDataAdapter(string.Format("SELECT * FROM [{0}]", SheetName), conn);

            OleDbDataAdapter odda = new OleDbDataAdapter(string.Format("select * from [Sheet1$]", conn),conn);             　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　

            odda.Fill(ds, SheetName);

            conn.Close();

            return ds.Tables[0];

        }

        //根据Excel物理路径获取Excel文件中所有表名 

        public  String[] GetExcelSheetNames(string excelFile)
        {

            OleDbConnection objConn = null;

            System.Data.DataTable dt = null;

            try
            {

                //此连接只能操作Excel2007之前(.xls)文件

                //string strConn = "Provider=Microsoft.Jet.OleDb.4.0;" + "data source=" + excelFile + ";Extended Properties='Excel 8.0; HDR=NO; IMEX=1'";

                //此连接可以操作.xls与.xlsx文件

                string strConn = "Provider=Microsoft.Ace.OleDb.12.0;" + "data source=" + excelFile + ";Extended Properties='Excel 12.0; HDR=NO; IMEX=1'";


                objConn = new OleDbConnection(strConn);

                objConn.Open();

                dt = objConn.GetOleDbSchemaTable(OleDbSchemaGuid.Tables, null);

                if (dt == null)
                {

                    return null;

                }

                String[] excelSheets = new String[dt.Rows.Count];

                int i = 0;

                foreach (DataRow row in dt.Rows)
                {

                    excelSheets[i] = row["TABLE_NAME"].ToString();

                    i++;

                }

                return excelSheets;

            }

            catch
            {

                return null;

            }

            finally
            {

                if (objConn != null)
                {

                    objConn.Close();

                    objConn.Dispose();

                }

                if (dt != null)
                {

                    dt.Dispose();

                }

            }

        }
    }
}
