#include <SPI.h>
#include <RFID.h>

RFID rfid(10,5);    //D10--读卡器MOSI引脚、D5--读卡器RST引脚

//4字节卡序列号，第5字节为校验字节
unsigned char serNum[5];
//写卡数据
unsigned char writeDate[16] ={'G', 'e', 'e', 'k', '-', 'W', 'o', 'r', 'k', 'S', 'h', 'o', 'p', 0, 0, 0};
//原扇区A密码，16个扇区，每个扇区密码6Byte
unsigned char sectorKeyA[16][16] = {
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};
//新扇区A密码，16个扇区，每个扇区密码6Byte
unsigned char sectorNewKeyA[16][16] = {
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}

void loop()
{
  unsigned char i,tmp;
  unsigned char status;
  unsigned char str[MAX_LEN];
  unsigned char RC_size;
  unsigned char blockAddr;        //选择操作的块地址0～63

  //找卡
  rfid.isCard();
  //读取卡序列号
  if (rfid.readCardSerial())
  {
    Serial.print("The card's number is  : ");
    Serial.print(rfid.serNum[0],HEX);
    Serial.print(rfid.serNum[1],HEX);
    Serial.print(rfid.serNum[2],HEX);
    Serial.print(rfid.serNum[3],HEX);
    Serial.print(rfid.serNum[4],HEX);
    Serial.println(" ");
  }

  //选卡，返回卡容量（锁定卡片，防止多次读写）
  rfid.selectTag(rfid.serNum);
  
  //写数据卡
  blockAddr = 7;                //数据块7
  if (rfid.auth(PICC_AUTHENT1A, blockAddr, sectorKeyA[blockAddr/4], rfid.serNum) == MI_OK)  //认证
  {
    //写数据
    status = rfid.write(blockAddr, sectorNewKeyA[blockAddr/4]);
    Serial.print("set the new card password, and can modify the data of the Sector: ");
    Serial.println(blockAddr/4,DEC);
    //写数据
    blockAddr = blockAddr - 3 ; //数据块4
    status = rfid.write(blockAddr, writeDate);
    if(status == MI_OK)
    {
      Serial.println("Write card OK!");
    }
  }

  //读卡
  blockAddr = 7;                //数据块7
  status = rfid.auth(PICC_AUTHENT1A, blockAddr, sectorNewKeyA[blockAddr/4], rfid.serNum);
  if (status == MI_OK)  //认证
  {
    //读数据
    blockAddr = blockAddr - 3 ; //数据块4
    if( rfid.read(blockAddr, str) == MI_OK)
    {
      Serial.print("Read from the card ,the data is : ");
      Serial.println((char *)str);
    }
  }
  
  rfid.halt();
}
