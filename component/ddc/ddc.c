#include "ddc.h"
DdcNode_t list_send;
DdcNode_t list_wait_ack;
DdcCallBack_t ddc_callback_ch[255] = {0};
//帧结构
//|--|--|--|--|----|---|--|--|---n---|--|--|
//|head |--ID-| ch |ack|-len-|payload|-crc-|

enum
{
    DDC_HEAD,
    DDC_ID,
    DDC_CH,
    DDC_ACK,
    DDC_LEN,
    DDC_PAYLOAD,
    DDC_CRC,

} ddc_recv_state;

static uint16_t ddc_id;//通讯帧ID发生器

DdcFrame_t recv_frame;//通讯处理缓冲帧


void ddc_attach_chx(uint8_t ch, DdcCallBack_t callback)
{
    ddc_callback_ch[ch] = callback;
}


void ddc_loop()
{
    int i;
    ddc_send_list(&list_send);
    ddc_check_ack_list();
    for(i = 0; i < 50; i++)
        ddc_recv_process();
}


void ddc_recv_process()
{
    uint8_t ch;
    uint8_t len;
    static uint16_t counter = 0;
    static uint8_t head[2];
    uint8_t buf[DDC_MAX_FRAME_LENGTH];

    len = ebox_fifo_get(fifo_recv, &ch, 1);
    if(len == 1)
    {
        switch((uint8_t)ddc_recv_state)
        {
        case DDC_HEAD:

            head[0] = head[1];
            head[1] = ch;
            if((head[0] == 0x55) && (head[1] == 0xaa))
            {
                recv_frame.head[0] = head[0];
                recv_frame.head[1] = head[1];

                ddc_recv_state = DDC_ID;
                recv_frame.head[1] = 0x00;
            }
#if DDC_DEBUG
            ebox_printf("head->");
#endif
            break;
        case DDC_ID:
            recv_frame.id.byte[counter++] = ch;
            if(counter == 2)
            {
                ddc_recv_state = DDC_CH;
                counter = 0;
            }
            break;
        case DDC_CH:
            recv_frame.ch = ch;
            ddc_recv_state = DDC_ACK;
            break;
        case DDC_ACK:
            recv_frame.ack = (DdcAck_t)ch;
            ddc_recv_state = DDC_LEN;
            break;
        case DDC_LEN:
            recv_frame.payload_len.byte[counter++] = ch;
            if(counter == 2)
            {
                if(recv_frame.payload_len.value <= DDC_MAX_PAYLOAD_LENGTH)
                {
                    ddc_recv_state = DDC_PAYLOAD;
                }
                else
                {
                    ddc_recv_state = DDC_HEAD;
                }
                counter = 0;
            }
#if DDC_DEBUG
            ebox_printf("len->");
#endif
            break;
        case DDC_PAYLOAD:
            recv_frame.payload[counter++] = ch;
            if(counter == recv_frame.payload_len.value)
            {
                ddc_recv_state = DDC_CRC;
                counter = 0;
            }
#if DDC_DEBUG
            ebox_printf("payload->");
#endif
            break;
        case DDC_CRC:
#if DDC_DEBUG
            ebox_printf("crc->");
#endif
            recv_frame.crc.byte[counter++] = ch;
            if(counter == 2)
            {
                ddc_frame_to_buf(buf, &recv_frame);
                if(crc16(buf, (recv_frame.payload_len.value + 8)) == recv_frame.crc.value) //crc ok
                {
                    //方法1：
                    //将该段数据添加到应用数据链表中。被用户查询并执行
                    //方法2：
                    //通过analyz程序回调函数
                    ddc_analyze_frame(&recv_frame);
                    ddc_recv_state = DDC_HEAD;

                }
                else
                {
#if DDC_DEBUG
                    ebox_printf("\r\ncrc = 0x%04x \r\n", recv_frame.crc.value);
                    ebox_printf("\r\ncrc = 0x%04x \r\n", crc16(buf, recv_frame.payload_len.value + 8));
#endif
                    ddc_recv_state = DDC_HEAD;

                }
                counter = 0;

            }
            break;

        default:
            ddc_recv_state = DDC_HEAD;
            break;
        }

    }
}
void ddc_analyze_frame(DdcFrame_t *frame)
{
    uint8_t buf[12];
    DataU16_t id;
    id.byte[0] = frame->payload[0];
    id.byte[1] = frame->payload[1];
    switch(frame->ch)
    {
    case 255:
#if DDC_DEBUG
        ebox_printf("\r\n接收到回复:id = %d\r\n", id.value);
#endif
        //接收到回复帧，删除等待回复链表中的对应节点
        list_delete_by_val(&list_wait_ack, id.value);
        break;
    default :
        if(ddc_callback_ch[frame->ch] != 0)
            ddc_callback_ch[frame->ch](recv_frame.payload, recv_frame.payload_len.value );
        break;
    }
    if(frame->ack == 1)
    {
        //创建回复帧，并插入发送队列
        ddc_make_ack_frame(buf, frame->id.value);
        ddc_add_to_list(buf);

    }
#if DDC_DEBUG
    ebox_printf("ch:%d\r\n", frame->ch);
#endif

}

uint16_t ddc_frame_to_buf(uint8_t *dst, DdcFrame_t *frame)
{
    uint16_t i = 0;
    uint16_t j = 0;
    dst[i++] = 0x55;
    dst[i++] = 0xAA;

    dst[i++] = frame->id.byte[0];
    dst[i++] = frame->id.byte[1];

    dst[i++] = frame->ch;// ch

    dst[i++] = frame->ack;//ack no need

    dst[i++] = frame->payload_len.byte[0];
    dst[i++] = frame->payload_len.byte[1];

    for(j = 0; j < frame->payload_len.value; j++)
        dst[i++] = frame->payload[j];

    dst[i++] = frame->crc.byte[0];
    dst[i++] = frame->crc.byte[1];

    return i;
}
uint16_t ddc_buf_to_frame(DdcFrame_t *frame, uint8_t *src)
{
    uint16_t i = 0;
    uint16_t j = 0;
    frame->head[0]      = src[i++];
    frame->head[1]      = src[i++];


    frame->id.byte[0]   = src[i++];
    frame->id.byte[1]   = src[i++];

    frame->ch           = src[i++];// ch

    frame->ack          = (DdcAck_t)src[i++];//ack no need

    frame->payload_len.byte[0] = src[i++];
    frame->payload_len.byte[1] = src[i++];

    for(j = 0; j < frame->payload_len.value; j++)
        frame->payload[j] = src[i++];

    frame->crc.byte[0]  = src[i++];
    frame->crc.byte[1]  = src[i++];
    return i;
}
uint16_t ddc_get_id()
{
    return ddc_id++;
}
uint8_t *ddc_nonblocking(uint8_t *data, uint16_t data_len, DdcAck_t ack, uint8_t ch )
{
    uint16_t i = 0, k = 0;
    uint8_t *dst = (uint8_t *)ebox_malloc(data_len + 10);

    DataU16_t payload_len;
    DataU16_t frame_id;
    DataU16_t crc;

    payload_len.value = data_len;
    frame_id.value = ddc_get_id();


    dst[i++] = 0x55;
    dst[i++] = 0xAA;

    dst[i++] = frame_id.byte[0];
    dst[i++] = frame_id.byte[1];

    dst[i++] = ch;

    dst[i++] = ack;

    dst[i++] = payload_len.byte[0];
    dst[i++] = payload_len.byte[1];


    for( k = 0; k < data_len; k++ )
        dst[i++] = *data++;

    crc.value = crc16(dst, payload_len.value + 8);

    dst[i++] = crc.byte[0];
    dst[i++] = crc.byte[1];

    ddc_add_to_list(dst);

    return dst;
}

uint16_t ddc_make_frame(uint8_t *dst, uint8_t *data, uint16_t data_len, DdcAck_t ack, uint8_t ch )
{
    uint16_t i = 0, k = 0;


    DataU16_t payload_len;
    DataU16_t frame_id;
    DataU16_t crc;

    payload_len.value = data_len;
    frame_id.value = ddc_get_id();


    dst[i++] = 0x55;
    dst[i++] = 0xAA;

    dst[i++] = frame_id.byte[0];
    dst[i++] = frame_id.byte[1];

    dst[i++] = ch;

    dst[i++] = ack;

    dst[i++] = payload_len.byte[0];
    dst[i++] = payload_len.byte[1];


    for( k = 0; k < data_len; k++ )
        dst[i++] = *data++;

    crc.value = crc16(dst, payload_len.value + 8);

    dst[i++] = crc.byte[0];
    dst[i++] = crc.byte[1];
    return i;
}
uint16_t ddc_make_ack_frame(uint8_t *dst, uint16_t id)
{
    uint16_t i = 0;
    //    uint16_t frame_len = 2 + 10;
    DataU16_t payload_len ;
    DataU16_t frame_id;//帧本身的id
    DataU16_t crc;
    DataU16_t data;
    data.value = id;//需要回复的id

    payload_len.value = 2 ;
    frame_id.value = ddc_get_id();

    dst[i++] = 0x55;
    dst[i++] = 0xAA;

    dst[i++] = frame_id.byte[0];
    dst[i++] = frame_id.byte[1];

    dst[i++] = 255;

    dst[i++] = 0;

    dst[i++] = payload_len.byte[0];
    dst[i++] = payload_len.byte[1];


    dst[i++] = data.byte[0];
    dst[i++] = data.byte[1];


    crc.value = crc16(dst, payload_len.value + 8);

    dst[i++] = crc.byte[0];
    dst[i++] = crc.byte[1];
    return i;

}
void ddc_add_to_list(uint8_t *buf)
{

    DdcFrame_t frame;

    ddc_buf_to_frame(&frame, buf);



    list_insert(&list_send, buf);
#if DDC_DEBUG
    ebox_printf("<<<<add frame to [send list] ok ;id = %d\tget free = %d\r\n", frame.id.value, ebox_get_free());
#endif
    if(frame.ack == 1)
    {
        uint8_t *ack_buf = (uint8_t *)ebox_malloc(PAY2FRAME_LEN(frame.payload_len.value));
        ebox_memcpy(ack_buf, buf, PAY2FRAME_LEN(frame.payload_len.value));
        list_insert(&list_wait_ack, ack_buf);
        ebox_printf("<<<<add frame to [WaitAck list] ok ;id = %d\tget free = %d \r\n", frame.id.value, ebox_get_free());

    }

}
void ddc_retry(DdcNode_t *pframe)
{
    DdcFrame_t frame;
    uint8_t *buf ;
    ddc_buf_to_frame(&frame, pframe->p);
    buf = (uint8_t *)ebox_malloc(PAY2FRAME_LEN(frame.payload_len.value));


    //    uint16_t payload_len = get_len_frome_frame(p->p);
    //    uint16_t id = get_id_frome_frame(p->p);
    //    uint8_t ack = get_ack_frome_frame(p->p);
    //    uint8_t ch = get_type_frome_frame(p->p);

    //    print_frame(p->p);
    ddc_make_frame(buf, frame.payload, frame.payload_len.value, frame.ack, frame.ch);
    ddc_add_to_list(buf);
    no_interrupts();//原子操作
    list_delete_by_val(&list_wait_ack, frame.id.value);
    interrupts();
}
void ddc_send_list(DdcNode_t *p)
{

    //    DdcFrame_t frame;
    uint16_t id, len;
    if(p->next != NULL)
        while(p->next != NULL)
        {
            p = p->next;
            id = get_frame_id(p->p);
            len = get_frame_payload_len(p->p);
            //        ddc_buf_to_frame(&frame,p->p);

            //        ebox_printf("len = %d,id=%d\r\n",len,id);
            //        print_frame(p->p);

            ddc_send_buf(p->p, len + 10);
#if DDC_DEBUG
            ebox_printf(">>>>send frame ok:id = %d;\t get free:%d\r\n", id, ebox_get_free());
#endif
            no_interrupts();//原子操作
            list_delete_by_val(&list_send, id);
            interrupts();
            //        ebox_printf(">>>>delete frame:id = %d;\t get free:%d\r\n",id,ebox_get_free());
        }
}

static uint64_t ack_timer ;
void ddc_check_ack_list(void)
{
    uint16_t num = 0;
    DdcNode_t *temp = &list_wait_ack;
    //    uint16_t len,id;
    if(temp->next == NULL) return;
    //调试用
    do
    {
        num++;
        temp = temp->next;
        // ebox_printf("等待回复列表:p->timer = %d;id = %d\r\n",temp->time,get_id_frome_frame(temp->p));
    }
    while (temp->next != NULL) ;
    ebox_printf("超时连接数量：%d\r\n", num);
    temp = &list_wait_ack;
    ///////////////////////////////////
    if(millis() - ack_timer > 2000)
    {
        ack_timer = millis();
        do
        {
            temp = temp->next;
            temp->time++;
            if(temp->time >= 20)
            {


                //               print_frame(temp->p);
                //ebox_printf("超时列表:p->timer = %d;id = %d\r\n",temp->time,get_id_frome_frame(temp->p));
                //ebox_printf("创建重新发送帧，并插入发送链表\r\n",temp->time,get_id_frome_frame(temp->p));

                ddc_retry(temp);
            }
            //            ebox_printf("【】check ack:p->timer = %d\r\n",temp->time);
        }
        while (temp->next != NULL) ;


    }


}
uint16_t get_frame_payload_len(uint8_t *frame)
{
    DataU16_t temp;
    temp.byte[0] = frame[6];
    temp.byte[1] = frame[7];
    return temp.value;
}

uint16_t get_frame_id(uint8_t *frame)
{
    DataU16_t temp;
    temp.byte[0] = frame[2];
    temp.byte[1] = frame[3];
    return temp.value;
}
uint8_t get_frame_ack(uint8_t *frame)
{
    return frame[5];
}
uint8_t get_frame_ch(uint8_t *frame)
{
    return frame[4];
}
void print_frame(uint8_t *frame)
{
    int i;
    uint8_t *p = frame;
    uint16_t len = get_frame_payload_len(p) + 10;
    for(i = 0; i < len; i++)
        ebox_printf("0X%02X ", *p++);
    ebox_printf("\r\n");
}
void print_list(DdcNode_t *list)
{
    uint16_t len, id;
    DdcNode_t *p = list;
    if(p->next != NULL)
        while(p->next != NULL)
        {
            p = p->next;
            id = get_frame_id(p->p);
            len = get_frame_payload_len(p->p) + 10;


            ebox_printf("len = %d,id=%d\r\n", len, id);
            print_frame(p->p);

        }

}


