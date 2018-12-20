
strFileInput = "SSC-Device.xls"
strFileCodeOutput = "ethercat.ino"
strFileXmlOutput = "SlaveDevice.xml"

def parseParameters(params):
    global strFileInput,strFileCodeOutput,strFileXmlOutput
    for i in range(0, len(params)):
        if params[i] == "-h" or params[i] == "/h":
            print """Usage: SlaveGeneraor
    \t[-i Input FileName = "SSC-Device.xls"]
    \t[-c Code FileName = "ethercat.ino"]
    \t[-x Xml FileName=  "SlaveDevice.xml"]
If the command has no parameters, it check SlaveGenerator.cfg for parameters."""
            quit()
        elif params[i] == "-i":
            strFileInput = params[i+1]
        elif params[i] == "-c":
            strFileCodeOutput = params[i+1]
        elif params[i] == "-x":
            strFileXmlOutput = params[i+1]
     

#Read Input parameters
import os
if os.path.exists('SlaveGeneraor.cfg'):
    fConfig = open('SlaveGeneraor.cfg')
    params = fConfig.read()
    params = params.split()
    parseParameters(params)
    
import sys
parseParameters(sys.argv)
   
#Generate code
from SlaveReader import SlaveReader
from SlaveReader import SlaveInfoReader
from ODGenerator import ODGenerator
from PDOMapping  import GenerateRxPdoMappingProg
from PDOMapping  import GenerateTxPdoMappingProg
from XmlGenerator import XmlGenerator

print "Parsing ", strFileInput, "..."
listSlaveDict = SlaveReader(strFileInput)
dictSlaveInfo = SlaveInfoReader(strFileInput)
strApplicationObjDic = ODGenerator(listSlaveDict)
strOutputMapping = GenerateRxPdoMappingProg(listSlaveDict)
strInputMapping = GenerateTxPdoMappingProg(listSlaveDict)

f = open("ethercat_template.txt")
strFile = f.read()
f.close()
dictMapping = {\
    "ApplicationObjDic"     :strApplicationObjDic,
    "OutputMapping"         :strOutputMapping,
    "InputMapping"          :strInputMapping,
    }
dictMapping.update(dictSlaveInfo)
strFile = strFile%dictMapping
        
f = open(strFileCodeOutput,'w')
f.write(strFile)
f.close()
print 'Arduino code file "',strFileCodeOutput,'" generated.'

XmlGenerator(strFileXmlOutput,dictSlaveInfo,listSlaveDict)
print 'ESI file "',strFileXmlOutput,'" generated.'

print "Press ENTER to continue..."
raw_input()
