from PDOMapping import GeneratePdoMapping

def GeneratePdoMappingString(listSlaveInfo,rxtx,strEntryPrefix,strPdoObjPrefix):
    dictPdoMapping = GeneratePdoMapping(listSlaveInfo,rxtx,strEntryPrefix,strPdoObjPrefix)
    strPdoMapping = ""
    strAppObjDic = ""
    #Generate PDO Mapping Object
    for itemPdoMapping in dictPdoMapping.iteritems():
        #Generate Remark
        strRemark = "/****************************************************\n"
        strRemark = strRemark+"** Object"+itemPdoMapping[0]+'\n'
        strRemark = strRemark+"****************************************************/\n"
        #Generate entry description
        strEntryDesc = 'OBJCONST TSDOINFOENTRYDESC OBJMEM '+'asEntryDesc'+itemPdoMapping[0]
        strEntryDesc = strEntryDesc+'[] = {\n'
        strEntryDesc = strEntryDesc+'\t{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ },\n'
        strEntryDesc = strEntryDesc+'\t{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},\n'*len(itemPdoMapping[1])
        strEntryDesc = strEntryDesc+'};'
        #Generate Obj name
        if rxtx == "rx":
            strObjName = 'OBJCONST UCHAR OBJMEM aName'+itemPdoMapping[0]+'[] = "Output Mapping '
        elif rxtx == "tx":
            strObjName = 'OBJCONST UCHAR OBJMEM aName'+itemPdoMapping[0]+'[] = "Input Mapping '
        strObjName = strObjName+itemPdoMapping[0][4:6]
        strObjName = strObjName+'\\000\\377";'        
        #Generate var declaration
        strVarDecl = "typedef struct OBJ_STRUCT_PACKED_START {\n"
        strVarDecl = strVarDecl+"\tUINT16   u16SubIndex0;\n"
        strVarDecl = strVarDecl+"\tUINT32   aEntries["+str(len(itemPdoMapping[1]))+"];\n"
        strVarDecl = strVarDecl+"} OBJ_STRUCT_PACKED_END\n"
        strVarDecl = strVarDecl+"TOBJ"+itemPdoMapping[0]+';\n'
        strVarDecl = strVarDecl+"TOBJ"+itemPdoMapping[0]+" Obj"+itemPdoMapping[0]+' __attribute__ ((aligned (2)))\n={'
        strVarDecl = strVarDecl+str(len(itemPdoMapping[1]))+', {'
        for dictEntry in itemPdoMapping[1]:
            strVarDecl = strVarDecl+str(dictEntry['Index'])+("%02d"%dictEntry['SI'])+\
                         ("%02x"%dictEntry['Length'])+','
        strVarDecl = strVarDecl+'}};'
        strPdoMapping = strPdoMapping+strRemark+strVarDecl+'\n'+strEntryDesc+'\n'+strObjName+'\n'
        #Generate AppObjDic of PDO mapping
        strAppObjDic = strAppObjDic+"\t{NULL,NULL,"+itemPdoMapping[0] 
        strAppObjDic = strAppObjDic+",{DEFTYPE_PDOMAPPING, 1 | (OBJCODE_ARR << 8)}, asEntryDesc"
        strAppObjDic = strAppObjDic+itemPdoMapping[0]+', aName'+itemPdoMapping[0]+', &Obj'+itemPdoMapping[0]
        strAppObjDic = strAppObjDic+', NULL, NULL, 0x0000 },\n'
    #Generate PDO Assignment
    #Generate Remark
    strRemark = "/****************************************************\n"
    if rxtx == "rx":
        strRemark = strRemark+"** Object0x1C12\n"
    elif rxtx == "tx":
        strRemark = strRemark+"** Object0x1C13\n"
    strRemark = strRemark+"****************************************************/\n"
    #Generate Obj name
    if rxtx == "rx":
        strPdoAssignIndex = '0x1C12'
        strPdoAssignVarName = 'sRxPDOassign'
    elif rxtx == "tx":
        strPdoAssignIndex = '0x1C13'
        strPdoAssignVarName = 'sTxPDOassign'
    strObjName = 'OBJCONST UCHAR OBJMEM aName'+strPdoAssignIndex+'[] = "'+rxtx.upper()+'PDO assign";'
    #Generate var declaration
    strVarDecl = "typedef struct OBJ_STRUCT_PACKED_START {\n"
    strVarDecl = strVarDecl+"\tUINT16   u16SubIndex0;\n"
    strVarDecl = strVarDecl+"\tUINT16   aEntries["+str(len(dictPdoMapping))+"];\n"
    strVarDecl = strVarDecl+"} OBJ_STRUCT_PACKED_END\n"
    strVarDecl = strVarDecl+"TOBJ"+strPdoAssignIndex+';\n'
    strVarDecl = strVarDecl+"TOBJ"+strPdoAssignIndex+' '+strPdoAssignVarName+' __attribute__ ((aligned (2))) \n={'
    strVarDecl = strVarDecl+str(len(dictPdoMapping))+', {'
    strVarDecl = strVarDecl+','.join(dictPdoMapping.keys())
    strVarDecl = strVarDecl+'}};'
    strPdoMapping = strPdoMapping+strRemark+strVarDecl+'\n'+strObjName+'\n'
    #Generate AppObjDic of PDO assignment
    strAppObjDic = strAppObjDic+"\t{NULL,NULL,"+strPdoAssignIndex 
    strAppObjDic = strAppObjDic+",{DEFTYPE_UNSIGNED16, "+str(len(dictPdoMapping))
    strAppObjDic = strAppObjDic+" | (OBJCODE_ARR << 8)}, asPDOAssignEntryDesc"
    strAppObjDic = strAppObjDic+', aName'+strPdoAssignIndex+', &'+strPdoAssignVarName
    strAppObjDic = strAppObjDic+', NULL, NULL, 0x0000 },\n'    
    return (strPdoMapping,strAppObjDic)



def GenerateTxPdoMappingString(listSlaveInfo):
    return GeneratePdoMappingString(listSlaveInfo,"tx","0x6","0x1A")

def GenerateRxPdoMappingString(listSlaveInfo):
    return GeneratePdoMappingString(listSlaveInfo,"rx","0x7","0x16")
    
            

def ODGenerator(listSlaveInfo):
    from EthercatType import dictEthercatType, dictAccessType, dictPdoDirType
    strDecl = ""
    #Generate PdoMappingString
    (strDeclTxPdo,strAppObjDicTxPdo) = GenerateTxPdoMappingString(listSlaveInfo);
    (strDeclRxPdo,strAppObjDicRxPdo) = GenerateRxPdoMappingString(listSlaveInfo);
    strDecl = strDecl+strDeclRxPdo;
    strDecl = strDecl+strDeclTxPdo;
    strAppObjDic = "\n/****************************************************\n"
    strAppObjDic = strAppObjDic+"** ApplicationObjDic\n"
    strAppObjDic = strAppObjDic+"****************************************************/\n"
    strAppObjDic = strAppObjDic+"TOBJECT    OBJMEM ApplicationObjDic[] = {\n"
    strAppObjDic = strAppObjDic+strAppObjDicTxPdo
    strAppObjDic = strAppObjDic+strAppObjDicRxPdo
    for dictItem in listSlaveInfo:
        if dictItem['ObjectCode'] == "VARIABLE":
            dictEntry = dictItem['Entry'][0]
            #Generate var declaration
            strVarDecl = dictEthercatType[dictEntry['DataType']][0]
            strVarDecl = strVarDecl+' '+"Obj"+dictItem['Index']+' __attribute__ ((aligned (2)))= '
            strVarDecl = strVarDecl+str(dictEntry['Default'])+';'
            #Generate entry description
            strEntryDesc = 'OBJCONST TSDOINFOENTRYDESC OBJMEM '+'asEntryDesc'+dictItem['Index']+ ' = {'
            strEntryDesc = strEntryDesc+dictEthercatType[dictEntry['DataType']][1]
            strEntryDesc = strEntryDesc+', '+str(dictEthercatType[dictEntry['DataType']][2])+', '
            strEntryDesc = strEntryDesc+dictAccessType[dictEntry['Access']]+'|'
            strEntryDesc = strEntryDesc+dictPdoDirType[dictEntry['PdoDirection']]
            strEntryDesc = strEntryDesc+'};'
            #Generate Obj name
            strObjName = 'OBJCONST UCHAR OBJMEM aName'+dictItem['Index']+'[] = "'+dictEntry['Name']+'";'
        elif dictItem['ObjectCode'] == "RECORD":
            #Generate entry description
            strEntryDesc = 'OBJCONST TSDOINFOENTRYDESC OBJMEM '+'asEntryDesc'+dictItem['Index']
            strEntryDesc = strEntryDesc+'['+str(len(dictItem['Entry']))+'] = {\n'
            for dictEntry in dictItem['Entry']:
                strEntryDesc = strEntryDesc+'\t{'
                strEntryDesc = strEntryDesc+dictEthercatType[dictEntry['DataType']][1]
                strEntryDesc = strEntryDesc+', '+str(dictEthercatType[dictEntry['DataType']][2])+', '
                strEntryDesc = strEntryDesc+dictAccessType[dictEntry['Access']]+'|'
                strEntryDesc = strEntryDesc+dictPdoDirType[dictEntry['PdoDirection']]
                strEntryDesc = strEntryDesc+'},\n'
            strEntryDesc = strEntryDesc+'};'    
            #Generate Obj name
            strObjName = 'OBJCONST UCHAR OBJMEM aName'+dictItem['Index']+'[] = "'
            for dictEntry in dictItem['Entry']:
                strObjName = strObjName+dictEntry['Name']+'\\000'
            strObjName = strObjName+'\\377";'
            #Generate var declaration
            strVarDecl = "typedef struct OBJ_STRUCT_PACKED_START {\n"
            bLastBitField = 0
            for dictEntry in dictItem['Entry']:
                strVarDecl = strVarDecl+"\t"+ dictEthercatType[dictEntry['DataType']][0]           
                strVarDecl = strVarDecl+' '+"SubIndex"+str(dictEntry['SI'])
                if dictEthercatType[dictEntry['DataType']][2]<16:
                    strVarDecl = strVarDecl+':'+str(dictEthercatType[dictEntry['DataType']][2])
                    if bLastBitField == 0:
                        strVarDecl = strVarDecl+' __attribute__ ((aligned (2)))'
                    bLastBitField = 1
                else:
                    strVarDecl = strVarDecl+' __attribute__ ((aligned (2)))'
                    bLastBitField = 0
                strVarDecl = strVarDecl+';\n'
            strVarDecl = strVarDecl+"} OBJ_STRUCT_PACKED_END\n"
            strVarDecl = strVarDecl+"TOBJ"+dictItem['Index']+';\n'
            strVarDecl = strVarDecl+"TOBJ"+dictItem['Index']+" Obj"+dictItem['Index']+'\n={'
            for dictEntry in dictItem['Entry']:
                strVarDecl = strVarDecl+str(dictEntry['Default'])+','
            strVarDecl = strVarDecl+'};'
        elif dictItem['ObjectCode'] == "ARRAY":
            #Generate entry description
            strEntryDesc = 'OBJCONST TSDOINFOENTRYDESC OBJMEM '+'asEntryDesc'+dictItem['Index']
            strEntryDesc = strEntryDesc+'['+str(len(dictItem['Entry']))+'] = {\n'
            for dictEntry in dictItem['Entry']:
                strEntryDesc = strEntryDesc+'\t{'
                strEntryDesc = strEntryDesc+dictEthercatType[dictEntry['DataType']][1]
                strEntryDesc = strEntryDesc+', '+str(dictEthercatType[dictEntry['DataType']][2])+', '
                strEntryDesc = strEntryDesc+dictAccessType[dictEntry['Access']]+'|'
                strEntryDesc = strEntryDesc+dictPdoDirType[dictEntry['PdoDirection']]
                strEntryDesc = strEntryDesc+'},\n'
            strEntryDesc = strEntryDesc+'};'    
            #Generate Obj name
            strObjName = 'OBJCONST UCHAR OBJMEM aName'+dictItem['Index']+'[] = "'
            strObjName = strObjName+dictItem['Entry'][0]['Name']+'";'
            #Generate var declaration
            strVarDecl = "typedef struct OBJ_STRUCT_PACKED_START {\n"
            strVarDecl = strVarDecl+"\t"+ dictEthercatType[dictItem['Entry'][0]['DataType']][0]
            strVarDecl = strVarDecl+' '+"SubIndex0"+' __attribute__ ((aligned (2)));\n'
            for intSI in range(1,int(dictItem['Entry'][0]['Default'])+1):
                strVarDecl = strVarDecl+"\t"+ dictEthercatType[dictItem['Entry'][1]['DataType']][0]\
                             +' '+"SubIndex"+str(intSI)
                if dictEthercatType[dictItem['Entry'][1]['DataType']][2]<16:
                    strVarDecl = strVarDecl+':'+str(dictEthercatType[dictItem['Entry'][1]['DataType']][2])
                if intSI==1 or dictEthercatType[dictItem['Entry'][1]['DataType']][2]>=16:
                    strVarDecl = strVarDecl+' __attribute__ ((aligned (2)))'
                strVarDecl = strVarDecl+';\n'
            strVarDecl = strVarDecl+"} OBJ_STRUCT_PACKED_END\n"
            strVarDecl = strVarDecl+"TOBJ"+dictItem['Index']+';\n'
            strVarDecl = strVarDecl+"TOBJ"+dictItem['Index']+" Obj"+dictItem['Index']+'\n={'
            strVarDecl = strVarDecl+str(dictItem['Entry'][0]['Default'])+','
            strVarDecl = strVarDecl+(str(dictItem['Entry'][1]['Default'])+',')*int(dictItem['Entry'][0]['Default'])
            strVarDecl = strVarDecl+'};'
        #Generate Remark
        strRemark = "/****************************************************\n"
        strRemark = strRemark+"** Object"+dictItem['Index']+'\n'
        strRemark = strRemark+"****************************************************/\n"
        strEntry = strRemark+strVarDecl+'\n'+strEntryDesc+'\n'+strObjName+'\n'
        #Append to declaration string
        strDecl = strDecl+strEntry
        #Generate ApplicationObjDic Item
        strObjDicItem = "\t{NULL, NULL, "+dictItem['Index']+', '
        if dictItem['ObjectCode'] == "VARIABLE":
            strObjDicItem = strObjDicItem+'{'+dictEthercatType[dictItem['Entry'][0]['DataType']][1]
            strObjDicItem = strObjDicItem+', 0|(OBJCODE_VAR<<8)}, '+'&'
        elif dictItem['ObjectCode'] == "RECORD":
            strObjDicItem = strObjDicItem+'{DEFTYPE_RECORD'
            strObjDicItem = strObjDicItem+', '+str(len(dictItem['Entry'])-1)+'|(OBJCODE_REC<<8)}, '
        elif dictItem['ObjectCode'] == "ARRAY":
            strObjDicItem = strObjDicItem+'{DEFTYPE_RECORD'
            strObjDicItem = strObjDicItem+', '+str(dictItem['Entry'][0]['Default'])+'|(OBJCODE_ARR<<8)}, '
        strObjDicItem = strObjDicItem+'asEntryDesc'+dictItem['Index']+', '
        strObjDicItem = strObjDicItem+'aName'+dictItem['Index']+', '
        strObjDicItem = strObjDicItem+'&Obj'+dictItem['Index']+', '
        strObjDicItem = strObjDicItem+'NULL , NULL , 0x0000 },\n'
        strAppObjDic = strAppObjDic+strObjDicItem
    strAppObjDic = strAppObjDic+"\t{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};\n"
    return strDecl+strAppObjDic

       



