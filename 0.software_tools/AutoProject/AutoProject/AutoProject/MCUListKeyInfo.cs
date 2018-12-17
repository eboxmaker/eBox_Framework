using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutoProject
{
    class MCUListKeyInfo
    {
        public struct ToJsonMy
        {
            public string name { get; set; }  //属性的名字，必须与json格式字符串中的"key"值一样。
            public string ram { get; set; }
            public string flash { get; set; }
            public string frequency { get; set; }
            
        }

    }
}
