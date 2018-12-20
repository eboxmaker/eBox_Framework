
dictEthercatType = {
    'BOOL': ('UINT16',       'DEFTYPE_BOOLEAN',      1),
    'BYTE': ('UINT16',       'DEFTYPE_UNSIGNED8',    1),
    'BIT1': ('UINT16',       'DEFTYPE_BIT1',         1),
    'BIT2': ('UINT16',       'DEFTYPE_BIT2',         2),
    'BIT3': ('UINT16',       'DEFTYPE_BIT3',         3),
    'BIT4': ('UINT16',       'DEFTYPE_BIT4',         4),
    'BIT5': ('UINT16',       'DEFTYPE_BIT5',         5),
    'BIT6': ('UINT16',       'DEFTYPE_BIT6',         6),
    'BIT7': ('UINT16',       'DEFTYPE_BIT7',         7),
    'BIT8': ('UINT16',       'DEFTYPE_BIT8',         8),
    'SINT': ('INT16',        'DEFTYPE_INTEGER8',     8),
    'INT' : ('INT16',       'DEFTYPE_INTEGER16',    16),
    'INT24':('INT32',       'DEFTYPE_INTEGER24',    24),
    'DINT': ('INT32',       'DEFTYPE_INTEGER32',    32),
    'INT40':('long long',   'DEFTYPE_INTEGER40',    40),
    'INT48':('long long',   'DEFTYPE_INTEGER48',    48),
    'INT56':('long long',   'DEFTYPE_INTEGER56',    56),
    'LINT': ('long long',   'DEFTYPE_INTEGER64',    64),
    'USINT': ('UINT16',      'DEFTYPE_UNSIGNED8',    8),
    'UINT' : ('UINT16',     'DEFTYPE_UNSIGNED16',   16),
    'UINT24':('UINT32',     'DEFTYPE_UNSIGNED24',   24),
    'UDINT': ('UINT32',     'DEFTYPE_UNSIGNED32',   32),
    'UINT40':('unsinged long long','DEFTYPE_UNSIGNED40',   40),
    'UINT48':('unsinged long long','DEFTYPE_UNSIGNED48',   48),
    'UINT56':('unsinged long long','DEFTYPE_UNSIGNED56',   56),
    'ULINT': ('unsinged long long','DEFTYPE_UNSIGNED64',   64),
    'REAL': ('float',       'DEFTYPE_REAL32',   32),
    'LREAL':('double',      'DEFTYPE_REAL64',   64),
    }
#The variable length types are not supported. Eg.STRING(n)

dictAccessType = {
    'RO'    :   'ACCESS_READ',
    'RW'    :   'ACCESS_READ|ACCESS_WRITE',
    };

dictPdoDirType = {
    ''      :   'OBJACCESS_NOPDOMAPPING',
    'rx'    :   'OBJACCESS_RXPDOMAPPING',
    'tx'    :   'OBJACCESS_TXPDOMAPPING',
    };
