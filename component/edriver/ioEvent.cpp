#include "ioEvent.h"
#include "ebox_mem.h"
IoChang::IoChang()
{
    lastState = 0;
}
void IoChang::add(IoNode *node)
{
    node->pin->mode(INPUT_PU);
    _list.add(node);
}
void IoChang::attach(myCallbackFunction fun)
{
    _clickFunc = fun;
}

void IoChang::loop()
{
    
    uint32_t newState = 0;
    for (int i = 0; i < _list.size(); i++)
    {
        bitWrite(newState, i, _list[i]->pin->read());
    }
    if(newState != lastState)
    {
        lastState = newState;
        if (_clickFunc)
        {
            char buf[32];
            memset(buf, 0, 32);
            snprintf(buf,32,"%02X %02X %02X %02X %02X %02X",cpu.chip_id[0],cpu.chip_id[1],cpu.chip_id[2],cpu.chip_id[3],cpu.chip_id[4],cpu.chip_id[5]);
            String macString = buf;
            /* Our array of "records": */
            root = cJSON_CreateObject();
            cJSON *fld = NULL;
            cJSON_AddItemToObject(root, "mac", cJSON_CreateString(macString.c_str()));
            cJSON *array = cJSON_CreateArray();
            cJSON_AddItemToObject(root, "skill", array);

            for (int i = 0; i < _list.size(); i++)
            {
                cJSON_AddItemToArray(array, fld = cJSON_CreateObject());
                cJSON_AddStringToObject(fld, "name", _list[i]->name.c_str());
                if (bitRead(newState, i) == 1)
                {
                    cJSON_AddStringToObject(fld, "state", "1");
                }
                else
                {
                    cJSON_AddStringToObject(fld, "state", "0");
                }

            }
            char *out = NULL;  
            out = cJSON_Print(root);
            _clickFunc(out);
            cJSON_Delete(root);
            ebox_free(out);
        }
    }
    
    
    
}


