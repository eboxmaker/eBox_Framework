import xml.etree.ElementTree as ET
from EthercatType import dictEthercatType
from PDOMapping import GeneratePdoMapping
from PDOMapping import GenerateRxPdoMapping
from PDOMapping import GenerateTxPdoMapping


def hexStrToEtg(strHex):
    if strHex[0:2] == "0x":
        result = "#x"+strHex[2:]
    else:
        result = strHex        
    return result


def intToHexLsb(intInput):
    hexMsb = "%08x"%intInput
    hexLsb = hexMsb[6:8]+hexMsb[4:6]+hexMsb[2:4]+hexMsb[0:2]
    return hexLsb.upper()

def searchElementDataType(eDataTypes,strDataType):
    for eDataType in eDataTypes:
        if eDataType[0].text == strDataType:
            return eDataType

def addDataType(eDataTypes,strDataType):
    if searchElementDataType(eDataTypes,strDataType)==None:
        eDataType = ET.SubElement(eDataTypes, 'DataType')
        eDataTypeName = ET.SubElement(eDataType, 'Name')
        eDataTypeName.text = strDataType
        eDataTypeNameSize = ET.SubElement(eDataType, 'BitSize')
        eDataTypeNameSize.text = str(dictEthercatType[strDataType][2])
    

def fillDictionaryString(eDataTypes,eObject,strContent):
    intStringLength = len(strContent)
    strStringType = "STRING("+str(len(strContent))+")"
    eObject[2].text = strStringType
    if searchElementDataType(eDataTypes,strStringType)==None:
        eStringType = ET.SubElement(eDataTypes, 'DataType')
        eStringTypeName = ET.SubElement(eStringType, 'Name')
        eStringTypeName.text = strStringType
        eStringTypeSize = ET.SubElement(eStringType, 'BitSize')
        eStringTypeSize.text = str(8*intStringLength)
    asciiString = strContent.encode('ascii')
    ordString = ("".join(["%02x"%(ord(ch)) for ch in asciiString])).upper()
    eObject[4][0].text = ordString
    return eDataTypes

def GeneratePdoObjects(root,listSlaveDict):
    eDataTypes = root[1][1][0][5][1][0]
    eObjects = root[1][1][0][5][1][1]
    eDT1C12ARR = eDataTypes[9]
    eDT1C12 = eDataTypes[10]
    eDT1C13ARR = eDataTypes[10]
    eDT1C13 = eDataTypes[11]
    eObj1C12 = eObjects[8]
    eObj1C13 = eObjects[9]
    #{PdoObj: [(Index,SI,length,intBitOffset)]}
    txPdoMapping = GenerateTxPdoMapping(listSlaveDict)
    rxPdoMapping = GenerateRxPdoMapping(listSlaveDict)
    #Generate SyncManager Assignment
    eDT1C12ARR = eDataTypes[9]
    eDT1C12 = eDataTypes[10]
    eDT1C13ARR = eDataTypes[11]
    eDT1C13 = eDataTypes[12]
    eObj1C12 = eObjects[8]
    eObj1C13 = eObjects[9]
    eDT1C12ARR[3][1].text = str(len(rxPdoMapping.keys()))
    eDT1C13ARR[3][1].text = str(len(txPdoMapping.keys()))
    eDT1C12ARR[2].text = str(len(rxPdoMapping.keys())*16)
    eDT1C13ARR[2].text = str(len(txPdoMapping.keys())*16)
    eDT1C12[1].text = str(len(rxPdoMapping.keys())*16+16)
    eDT1C13[1].text = str(len(txPdoMapping.keys())*16+16)
    eDT1C12[3][2].text = str(len(rxPdoMapping.keys())*16)
    eDT1C13[3][2].text = str(len(txPdoMapping.keys())*16)
    eObj1C12[3].text = eDT1C12[1].text
    eObj1C13[3].text = eDT1C13[1].text
    eObj1C12[4][0][1][0].text = intToHexLsb(len(rxPdoMapping.keys()))
    eObj1C13[4][0][1][0].text = intToHexLsb(len(txPdoMapping.keys()))
    intIdx = 1
    for strMappingObj in rxPdoMapping.keys():
        eSubItem = ET.SubElement(eObj1C12[4],"SubItem")
        eSubName = ET.SubElement(eSubItem, 'Name')
        eSubInfo = ET.SubElement(eSubItem, 'Info')
        eSubDefault = ET.SubElement(eSubInfo, 'DefaultData')
        eSubName.text = "SubIndex "+"%03d"%intIdx
        eSubDefault.text = intToHexLsb(int(strMappingObj,0))
        intIdx = intIdx+1
    intIdx = 1
    for strMappingObj in txPdoMapping.keys():
        eSubItem = ET.SubElement(eObj1C13[4],"SubItem")
        eSubName = ET.SubElement(eSubItem, 'Name')
        eSubInfo = ET.SubElement(eSubItem, 'Info')
        eSubDefault = ET.SubElement(eSubInfo, 'DefaultData')
        eSubName.text = "SubIndex "+"%03d"%intIdx
        eSubDefault.text = intToHexLsb(int(strMappingObj,0))
        intIdx = intIdx+1    
    #Generate Pdo Objects
    pdoMapping = txPdoMapping
    pdoMapping.update(rxPdoMapping)
    idxInsert = 7
    idxRxPdo = 0
    idxTxPdo = 0
    for pdoObject in pdoMapping.iteritems():
        #Generate DataType
        eDataType = ET.Element("DataType")
        eDTName = ET.Element("Name")
        eDTName.text = "DT"+pdoObject[0][2:]
        eDataType.append(eDTName)
        eDTSize = ET.SubElement(eDataType, 'BitSize')
        eDTSize.text = str(16+32*len(pdoObject[1]))
        eDTSubItem = ET.SubElement(eDataType, 'SubItem')
        eDTSubIdx = ET.SubElement(eDTSubItem, 'SubIdx')
        eDTSubIdx.text = "0"
        eDTSubName = ET.SubElement(eDTSubItem, 'Name')
        eDTSubName.text = "SubIndex 000"
        eDTSubType = ET.SubElement(eDTSubItem, 'Type')
        eDTSubType.text = "UINT"
        eDTSubSize = ET.SubElement(eDTSubItem, 'BitSize')
        eDTSubSize.text = "16"
        eDTBitOffs = ET.SubElement(eDTSubItem, 'BitOffs')
        eDTBitOffs.text = "0"
        eAccess = ET.Element("Access")
        eAccess.text = "ro"
        eFlags = ET.Element("Flags")
        eFlags.append(eAccess)
        eDTSubItem.append(eFlags)
        intBitOffs = int(16)
        intIdx = int(1)
        #Generate Object
        eObject = ET.Element("Object")
        eObjIndex = ET.SubElement(eObject, 'Index')
        eObjIndex.text = hexStrToEtg(pdoObject[0])
        eObjName = ET.SubElement(eObject, 'Name')
        if pdoObject[0][0:4]=="0x1A":
            eObjName.text = "Input mapping "+str(idxRxPdo)
            idxRxPdo = idxRxPdo+1
        else:
            eObjName.text = "Output mapping "+str(idxTxPdo)
            idxTxPdo = idxTxPdo+1
        eObjType = ET.SubElement(eObject, 'Type')
        eObjType.text = eDTName.text
        eObjSize = ET.SubElement(eObject, 'BitSize')
        eObjSize.text = eDTSize.text
        eObjInfo = ET.SubElement(eObject, 'Info')
        eSubItem = ET.SubElement(eObjInfo, 'SubItem')
        eSubName = ET.SubElement(eSubItem, 'Name')
        eSubName.text = "SubIndex 000"
        eSubInfo = ET.SubElement(eSubItem, 'Info')
        eSubDefault = ET.SubElement(eSubInfo, 'DefaultData')
        eSubDefault.text = intToHexLsb(len(pdoObject[1]))
        for pdoEntry in pdoObject[1]:
            eDTSubItem = ET.SubElement(eDataType, 'SubItem')
            eDTSubIdx = ET.SubElement(eDTSubItem, 'SubIdx')
            eDTSubIdx.text = str(intIdx)
            eDTSubName = ET.SubElement(eDTSubItem, 'Name')
            eDTSubName.text = "SubIndex "+"%03d"%intIdx
            eDTSubType = ET.SubElement(eDTSubItem, 'Type')
            eDTSubType.text = "UDINT"
            eDTSubSize = ET.SubElement(eDTSubItem, 'BitSize')
            eDTSubSize.text = "32"
            eDTBitOffs = ET.SubElement(eDTSubItem, 'BitOffs')
            eDTBitOffs.text = str(intBitOffs)
            eDTSubItem.append(eFlags)
            eSubItem = ET.SubElement(eObjInfo, 'SubItem')
            eSubName = ET.SubElement(eSubItem, 'Name')
            eSubName.text = eDTSubName.text
            eSubInfo = ET.SubElement(eSubItem, 'Info')
            eSubDefault = ET.SubElement(eSubInfo, 'DefaultData')
            eSubDefault.text = intToHexLsb(int(str(pdoEntry['Index'])\
                        +("%02d"%pdoEntry['SI'])+("%02x"%pdoEntry['Length']),0))
            intIdx = intIdx+1
            intBitOffs = intBitOffs+32
        eObjects.insert(idxInsert,eObject)
        eDataTypes.insert(idxInsert,eDataType)
        idxInsert = idxInsert+1
    #Genenrate Pdo Elements
    idxInsert = 13
    idxRxPdo = 0
    idxTxPdo = 0
    for pdoObject in pdoMapping.iteritems():
        ePdo = ET.Element("Pdo")
        ePdo.set('Fixed', 'true')
        ePdoIndex = ET.SubElement(ePdo, 'Index')
        ePdoIndex.text = hexStrToEtg(pdoObject[0])
        ePdoName = ET.SubElement(ePdo, 'Name')
        if pdoObject[0][0:4] == "0x1A":
            ePdo.tag = "TxPdo"
            ePdo.set('Sm','3')
            ePdoName.text = "Input mapping "+str(idxRxPdo)
            idxRxPdo = idxRxPdo+1
        elif pdoObject[0][0:4] == "0x16":
            ePdo.tag = "RxPdo"
            ePdo.set('Sm','2')
            ePdoName.text = "Output mapping "+str(idxTxPdo)
            idxTxPdo = idxTxPdo+1
        for pdoEntry in pdoObject[1]:
            ePdoEntry = ET.SubElement(ePdo, 'Entry')
            eEntryIdx = ET.SubElement(ePdoEntry, 'Index')
            eEntryIdx.text = hexStrToEtg(pdoEntry['Index'])
            eEntrySubIdx = ET.SubElement(ePdoEntry, 'SubIndex')
            eEntrySubIdx.text = str(pdoEntry['SI'])
            eEntryBitLen = ET.SubElement(ePdoEntry, 'BitLen')
            eEntryBitLen.text = str(pdoEntry['Length'])
            if int(pdoEntry['Index'],0)!=0:
                eEntryName = ET.SubElement(ePdoEntry, 'Name')
                eEntryName.text = pdoEntry['Name']
                eEntryDataType = ET.SubElement(ePdoEntry, 'DataType')
                eEntryDataType.text = pdoEntry['DataType']
        root[1][1][0].insert(idxInsert,ePdo)
        idxInsert = idxInsert+1
    return root
        
        
    
    
def XmlGenerator(strXmlFile,dictSlaveInfo,listSlaveDict):
    #Read template
    tree = ET.parse('SSC-Template.xml')
    root = tree.getroot()
    #General infomation
    root[0][0].text = hexStrToEtg(dictSlaveInfo['Vendor ID'])
    root[0][1].text = hexStrToEtg(dictSlaveInfo['Vendor Name'])
    root[1][1][0][0].set('ProductCode',hexStrToEtg(dictSlaveInfo['Product Code']))
    root[1][1][0][0].set('RevisionNo',hexStrToEtg(dictSlaveInfo['Revision Number']))
    root[1][1][0][0].text = dictSlaveInfo['Device Name']
    root[1][1][0][1].text = dictSlaveInfo['Device Name']
    root[1][1][0][2].text = dictSlaveInfo['Device Name']
    root[1][1][0][5][0][0].text = str(int(dictSlaveInfo['Device Profile']))
    root[1][0][0][0].text = dictSlaveInfo['Group Type']
    root[1][0][0][1].text = dictSlaveInfo['Group Name']
    root[1][0][0][2].text = dictSlaveInfo['Group Name']
    root[1][1][0][4].text = dictSlaveInfo['Group Type']
    #General Object Dictionary Existed Items
    eDataTypes = root[1][1][0][5][1][0]
    eObjects = root[1][1][0][5][1][1]
    #--Device Profile--
    eObjects[0][4][0].text = intToHexLsb(int(dictSlaveInfo['Device Profile']))
    #--Device Name--
    eDataTypes = fillDictionaryString(eDataTypes,eObjects[2],dictSlaveInfo['Device Name'])
    #--HW version--
    eDataTypes = fillDictionaryString(eDataTypes,eObjects[3],dictSlaveInfo['HW Version'])
    #--SW version--
    eDataTypes = fillDictionaryString(eDataTypes,eObjects[4],dictSlaveInfo['SW Version'])
    #--Identity--
    eObjects[5][4][1][1][0].text = intToHexLsb(int(dictSlaveInfo['Vendor ID'],0))
    eObjects[5][4][2][1][0].text = intToHexLsb(int(dictSlaveInfo['Product Code'],0))
    eObjects[5][4][3][1][0].text = intToHexLsb(int(dictSlaveInfo['Revision Number'],0))
    eObjects[5][4][4][1][0].text = intToHexLsb(int(dictSlaveInfo['Serial Number'],0))
    #General Application Dictionary 
    for dictSlaveInfo in listSlaveDict:
        #{Index, ObjectCode, [{SI,DataType,Name,Default,Access,PdoDir,Description}]}
        eObject = ET.SubElement(eObjects, 'Object')
        eObjIndex = ET.SubElement(eObject, 'Index')
        eObjIndex.text = hexStrToEtg(dictSlaveInfo['Index'])
        eObjName = ET.SubElement(eObject, 'Name')
        eObjName.text = dictSlaveInfo['Entry'][0]['Name']
        if eObjName.text=="":
            eObjName.text = "Obj"+dictSlaveInfo['Index'][2:]
        if dictSlaveInfo['ObjectCode']=="VARIABLE":
            eObjType = ET.SubElement(eObject, 'Type')
            eObjType.text = dictSlaveInfo['Entry'][0]['DataType']
            #If the data type does not exist, add it to datatype list
            addDataType(eDataTypes,eObjType.text)
            eObjBitSize = ET.SubElement(eObject, 'BitSize')
            eObjBitSize.text = str(dictEthercatType[dictSlaveInfo['Entry'][0]['DataType']][2])
            if dictSlaveInfo['Entry'][0]['Default'] != "" and \
                                dictSlaveInfo['Entry'][0]['Default'] != None:
                eObjInfo = ET.SubElement(eObject, 'Info')
                eObjDefault = ET.SubElement(eObjInfo, 'DefaultData')
                eObjDefault.text = intToHexLsb(int(dictSlaveInfo['Entry'][0]['Default']))          
        elif dictSlaveInfo['ObjectCode']=="RECORD":
            eObjType = ET.SubElement(eObject, 'Type')
            eObjType.text = "DT"+dictSlaveInfo['Index'][2:]
            eDataType = ET.SubElement(eDataTypes, 'DataType')
            eObjSize = ET.SubElement(eObject, 'BitSize')
            eDTName = ET.SubElement(eDataType, 'Name')
            eDTName.text = eObjType.text
            eDTSize = ET.SubElement(eDataType, 'BitSize')
            eSubInfo = ET.SubElement(eObject, 'Info')
            bitOffset = int(0)
            for dictEntry in dictSlaveInfo['Entry']:
                eDTSubItem = ET.SubElement(eDataType, 'SubItem')
                eSubIdx = ET.SubElement(eDTSubItem, 'SubIdx')
                eSubIdx.text = str(dictEntry['SI'])
                eSubName = ET.SubElement(eDTSubItem, 'Name')
                if dictEntry['Name']=="":
                    eSubName.text = "SubIndex "+"%03d"%dictEntry['SI']
                else:
                    eSubName.text = dictEntry['Name']
                eSubType = ET.SubElement(eDTSubItem, 'Type')
                eSubType.text = dictEntry['DataType']
                #If the data type does not exist, add it to datatype list
                addDataType(eDataTypes,eSubType.text)
                eSubSize = ET.SubElement(eDTSubItem, 'BitSize')
                intSubSize = dictEthercatType[dictEntry['DataType']][2]
                eSubSize.text = str(intSubSize)
                #16 bit padding
                if bitOffset%16!=0 and intSubSize<16 and (bitOffset%16+intSubSize>16):
                    bitOffset = (bitOffset/16+1)*16
                eSubBitOffset = ET.SubElement(eDTSubItem, 'BitOffs')
                eSubBitOffset.text = str(bitOffset)
                bitOffset = bitOffset+intSubSize
                eSubFlags = ET.SubElement(eDTSubItem, 'Flags')
                eSubAccess = ET.SubElement(eSubFlags, 'Access')
                eSubAccess.text = dictEntry['Access'].lower()
                eSubItem = ET.SubElement(eSubInfo, 'SubItem')
                eSubItemName = ET.SubElement(eSubItem, 'Name')
                eSubItemName.text = eSubName.text
                eSubItemInfo = ET.SubElement(eSubItem, 'Info')
                eSubDefault = ET.SubElement(eSubItemInfo, 'DefaultData')
                eSubDefault.text = intToHexLsb(int(dictEntry['Default']))
            eDTSize.text = str((bitOffset+7)/8*8)
            eObjSize.text = eDTSize.text
        elif dictSlaveInfo['ObjectCode']=="ARRAY":
            #Generate ARRAY TYPE
            eDataType = ET.SubElement(eDataTypes, 'DataType')
            eDTName = ET.SubElement(eDataType, 'Name')
            eDTName.text = "DT"+dictSlaveInfo['Index'][2:]+"ARR"
            eDTBaseType = ET.SubElement(eDataType, 'BaseType')
            eDTBaseType.text = dictSlaveInfo['Entry'][1]['DataType']
            #If the data type does not exist, add it to datatype list
            addDataType(eDataTypes,eDTBaseType.text)
            eDTSize = ET.SubElement(eDataType, 'BitSize')
            eDTSize.text = str(dictEthercatType[dictSlaveInfo['Entry'][1]['DataType']][2]\
                               *int(dictSlaveInfo['Entry'][0]['Default']))
            eDTArray = ET.SubElement(eDataType, 'ArrayInfo')
            eDTLBound = ET.SubElement(eDTArray, 'LBound')
            eDTLBound.text = "1"
            eDTElements = ET.SubElement(eDTArray, 'Elements')
            eDTElements.text = dictSlaveInfo['Entry'][0]['Default']
            #Generate Data TYPE
            eDataType = ET.SubElement(eDataTypes, 'DataType')
            eDTName = ET.SubElement(eDataType, 'Name')
            eDTName.text = "DT"+dictSlaveInfo['Index'][2:]
            eDTSize = ET.SubElement(eDataType, 'BitSize')
            eDTSize.text = str(dictEthercatType[dictSlaveInfo['Entry'][0]['DataType']][2]\
                               +dictEthercatType[dictSlaveInfo['Entry'][1]['DataType']][2]\
                               *int(dictSlaveInfo['Entry'][0]['Default']))
            eDTSubItem = ET.SubElement(eDataType, 'SubItem')
            eDTSubIdx = ET.SubElement(eDTSubItem, 'SubIdx')
            eDTSubIdx.text = "0"
            eDTName = ET.SubElement(eDTSubItem, 'Name')
            eDTName.text = "SubIndex 000"
            eDTType = ET.SubElement(eDTSubItem, 'Type')
            eDTType.text = dictSlaveInfo['Entry'][0]['DataType']
            addDataType(eDataTypes,eDTType.text)
            eDTSize = ET.SubElement(eDTSubItem, 'BitSize')
            eDTSize.text = str(dictEthercatType[dictSlaveInfo['Entry'][0]['DataType']][2])
            eDTBitOffs = ET.SubElement(eDTSubItem, 'BitOffs')
            eDTBitOffs.text = "0"
            eDTSubFlags = ET.SubElement(eDTSubItem, 'Flags')
            eDTSubAccess = ET.SubElement(eDTSubFlags, 'Access')
            eDTSubAccess.text = dictSlaveInfo['Entry'][0]['Access'].lower()
            eDTSubItem = ET.SubElement(eDataType, 'SubItem')
            eDTName = ET.SubElement(eDTSubItem, 'Name')
            eDTName.text = "Elements"
            eDTType = ET.SubElement(eDTSubItem, 'Type')
            eDTType.text = "DT"+dictSlaveInfo['Index'][2:]+"ARR"
            eDTSize = ET.SubElement(eDTSubItem, 'BitSize')
            eDTSize.text = str(dictEthercatType[dictSlaveInfo['Entry'][1]['DataType']][2]\
                               *int(dictSlaveInfo['Entry'][0]['Default']))
            eDTBitOffs = ET.SubElement(eDTSubItem, 'BitOffs')
            eDTBitOffs.text = "16"
            eDTSubFlags = ET.SubElement(eDTSubItem, 'Flags')
            eDTSubAccess = ET.SubElement(eDTSubFlags, 'Access')
            eDTSubAccess.text = dictSlaveInfo['Entry'][1]['Access'].lower()
            #Generate Object
            eObjType = ET.SubElement(eObject, 'Type')
            eObjType.text = "DT"+dictSlaveInfo['Index'][2:]
            eObjSize = ET.SubElement(eObject, 'BitSize')
            eObjSize.text = eDTSize.text
            eSubInfo = ET.SubElement(eObject, 'Info')
            eSubItem = ET.SubElement(eSubInfo, 'SubItem')
            eSubItemName = ET.SubElement(eSubItem, 'Name')
            eSubItemName.text = "SubIndex 000"
            eSubItemInfo = ET.SubElement(eSubItem, 'Info')
            eSubDefault = ET.SubElement(eSubItemInfo, 'DefaultData')
            eSubDefault.text = intToHexLsb(int(dictSlaveInfo['Entry'][0]['Default']))
            for idx in range(1,int(dictSlaveInfo['Entry'][0]['Default'])+1):
                eSubItem = ET.SubElement(eSubInfo, 'SubItem')
                eSubItemName = ET.SubElement(eSubItem, 'Name')
                eSubItemName.text = "SubIndex "+"%03d"%idx
                eSubItemInfo = ET.SubElement(eSubItem, 'Info')
                eSubDefault = ET.SubElement(eSubItemInfo, 'DefaultData')
                eSubDefault.text = intToHexLsb(int(dictSlaveInfo['Entry'][1]['Default']))
        eObjFlags = ET.SubElement(eObject, 'Flags')
        eObjAccess = ET.SubElement(eObjFlags, 'Access')
        eObjAccess.text = dictSlaveInfo['Entry'][0]['Access'].lower()
    #Generate Pdo Mapping
    root = GeneratePdoObjects(root,listSlaveDict)
            
    #Output Xml file
    tree.write(strXmlFile)
    
    
