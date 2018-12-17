using System;
using System.Xml.Linq;
using System.Linq;

namespace serial
{
    class XMLReadWrite
    {
        static string xmlName = "d://config.xml";
        XDocument xmlDoc = XDocument.Load(xmlName);

        public void LodeFile(string path)
        {
            xmlName = path;

        }
        /// <summary>  
        /// 返回XMl文件指定元素的指定属性值  
        /// </summary>  
        /// <param name="xmlElement">指定元素</param>  
        /// <param name="xmlAttribute">指定属性</param>  
        /// <returns></returns>  
        public  string getXmlValue(string xmlElement, string xmlAttribute)
        {

                var results = from c in xmlDoc.Descendants(xmlElement)
                              select c;
                string s = "";
                foreach (var result in results)
                {
                    s = result.Attribute(xmlAttribute).Value.ToString();
                }
                return s;
        }


        /// <summary>  
        /// 设置XMl文件指定元素的指定属性的值  
        /// </summary>  
        /// <param name="xmlElement">指定元素</param>  
        /// <param name="xmlAttribute">指定属性</param>  
        /// <param name="xmlValue">指定值</param>  
        public  void setXmlValue(string xmlElement, string xmlAttribute, string xmlValue)  
        {  
            xmlDoc.Element("Soft").Element(xmlElement).Attribute(xmlAttribute).SetValue(xmlValue);  
            xmlDoc.Save(xmlName);  
        }

        internal void getXmlValue()
        {
            throw new NotImplementedException();
        }
        /*
        public void CreateXmlFile()
        {
            XmlDocument xmlDoc = new XmlDocument();
            //创建类型声明节点  
            XmlNode node = xmlDoc.CreateXmlDeclaration("1.0", "utf-8", "");
            xmlDoc.AppendChild(node);
            //创建根节点  
            XmlNode root = xmlDoc.CreateElement("Soft");
            xmlDoc.AppendChild(root);
            CreateNode(xmlDoc, root, "LoginConfig ABM=\"2\" ACM=\"2\" ADM=\"2\" AOM=\"2\" BCM=\"2\" BDM=\"2\" BOM=\"2\" CDM=\"2\" COM=\"2\" DOM=\"2\"", "");

            try
            {
                xmlDoc.Save("d://XMLFile1.xml");
            }
            catch (Exception e)
            {
                //显示错误信息  
                Console.WriteLine(e.Message);
            }
            //Console.ReadLine();  

        }
        /// <summary>    
        /// 创建节点    
        /// </summary>    
        /// <param name="xmldoc"></param>  xml文档  
        /// <param name="parentnode"></param>父节点    
        /// <param name="name"></param>  节点名  
        /// <param name="value"></param>  节点值  
        ///   
        public void CreateNode(XmlDocument xmlDoc, XmlNode parentNode, string name, string value)
        {
            XmlNode node = xmlDoc.CreateNode(XmlNodeType.Element, name, null);
            node.InnerText = value;
            parentNode.AppendChild(node);
        } 
         * */
    }
}
