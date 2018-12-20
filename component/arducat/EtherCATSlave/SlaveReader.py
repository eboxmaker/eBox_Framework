def SlaveReader(filename):
    #Read the xlsx file
    import xlrd
    from EthercatType import dictEthercatType
    book = xlrd.open_workbook(filename)
    sh = book.sheet_by_index(0)
    print filename, " has", sh.nrows,"rows and", sh.ncols, "cols"


    #Initialize the dictionary list to return
    listSlaveInfo = [];

    #Parsing
    for row in range(17,sh.nrows):
        #Extract info
        strIndex = sh.cell_value(rowx=row, colx=1)
        strObjectCode = sh.cell_value(rowx=row, colx=2)
        intSI = sh.cell_value(rowx=row, colx=3)
        strDataType = sh.cell_value(rowx=row, colx=4)
        strName = sh.cell_value(rowx=row, colx=5)
        strDefault = sh.cell_value(rowx=row, colx=6)
        strAccess = sh.cell_value(rowx=row, colx=11)
        strPdoDir = sh.cell_value(rowx=row, colx=12)
        strDescription = sh.cell_value(rowx=row, colx=14)
        #Skip the empty items    
        if (len(strIndex)<4 or strIndex[0:2]!="0x") and intSI=="":
            continue;
        #Generate empty dictionary
        dictSlaveInfo = {};
        #Check Index
        if len(strIndex)<4:
            if len(listSlaveInfo)==0:
                print "Error: Row", row, ". Index should not be empty."
                return [];
            else:
                strIndex = listSlaveInfo[-1]['Index'];
        #Check Object Code
        if intSI!="" and listSlaveInfo!=[]:
            strObjectCode = listSlaveInfo[-1]['ObjectCode'];#Inherit the main index
        if len(strIndex)>=4 and (strObjectCode!="VARIABLE") and (strObjectCode!="ARRAY") and (strObjectCode!="RECORD"):
            print "Error: Row", row, ". Wrong object code."
            return [];
        #Check SubIndex
        if strObjectCode == "VARIABLE":
            intSI==""
        elif strObjectCode == "RECORD":
            if len(listSlaveInfo)==0:
                intSI = 0;
            elif listSlaveInfo[-1]['Index'] != strIndex:
                intSI = 0;
            elif listSlaveInfo[-1]['Entry'][-1]['SI']+1 != intSI:
                print "Error: Row", row, ". Wrong sub-index."
                return [];
            intSI = int(intSI)
        elif strObjectCode == "ARRAY":
            if len(listSlaveInfo)==0:
                intSI = 0;
            elif listSlaveInfo[-1]['Index'] != strIndex:
                intSI = 0;
            print "Note: Row", row, "The length of array is determined by the default value of subindex 0"
        #Check Type
        if strDataType not in dictEthercatType:
            print "Error: Row", row, "Unrecognized data type ",strDataType 
            return [];
        #Check Default
        if strDefault == "":
            strDefault = "0"
        if strObjectCode == "ARRAY" and strDefault<=0 and intSI==0:
            print "Error: Row", row, ". Wrong array length."
        #Check Access
        if strAccess != "RO" and strAccess != "RW":
            print "Error: Row", row, ". Access code should be either RO or RW."
            return [];
        #Check PDO direction
        if strPdoDir != "" and strPdoDir != "rx" and strPdoDir != "tx":
            print "Error: Row", row, ". PDO direction should be empty, rx or tx."
            return [];

        dictEntry = {
            'SI'        :   intSI,
            'DataType'  :   strDataType,
            'Name'      :   strName,
            'Default'   :   strDefault,
            'Access'    :   strAccess,
            'PdoDirection': strPdoDir,
            'Description':  strDescription
            };

        if strObjectCode== 'VARIABLE' or intSI == 0:
            dictSlaveInfo['Index'] = strIndex
            dictSlaveInfo['ObjectCode'] = strObjectCode
            dictSlaveInfo['Entry'] = [dictEntry]
            listSlaveInfo.append(dictSlaveInfo)
        else:
            listSlaveInfo[-1]['Entry'].append(dictEntry)
            if strObjectCode== 'RECORD':
                listSlaveInfo[-1]['Entry'][0]['Default'] = str(intSI)
    return listSlaveInfo

def SlaveInfoReader(filename):
    #Read the xlsx file
    import xlrd
    from EthercatType import dictEthercatType
    book = xlrd.open_workbook(filename)
    sh = book.sheet_by_index(0)

    dictInfo = {}
    dictInfo['Device Profile'] = sh.cell_value(rowx=0, colx=2)
    dictInfo['Module Profile'] = sh.cell_value(rowx=1, colx=2)
    dictInfo['Vendor ID'] = sh.cell_value(rowx=2, colx=2)
    dictInfo['Vendor Name'] = sh.cell_value(rowx=3, colx=2)
    dictInfo['Product Code'] = sh.cell_value(rowx=4, colx=2)
    dictInfo['Revision Number'] = sh.cell_value(rowx=5, colx=2)
    dictInfo['Serial Number'] = sh.cell_value(rowx=6, colx=2)
    dictInfo['Device Name'] = sh.cell_value(rowx=7, colx=2)
    dictInfo['Device Name Len'] = len(sh.cell_value(rowx=7, colx=2))
    dictInfo['HW Version'] = sh.cell_value(rowx=8, colx=2)
    dictInfo['HW Version Len'] = len(sh.cell_value(rowx=8, colx=2))
    dictInfo['SW Version'] = sh.cell_value(rowx=9, colx=2)
    dictInfo['SW Version Len'] = len(sh.cell_value(rowx=9, colx=2))
    dictInfo['Group Type'] = sh.cell_value(rowx=10, colx=2)
    dictInfo['Group Name'] = sh.cell_value(rowx=11, colx=2)

    return dictInfo
#Test bench
#listSlaveInfo = SlaveReader("SSC-Device.xlsx")
#for item in listSlaveInfo:
#    print item        
