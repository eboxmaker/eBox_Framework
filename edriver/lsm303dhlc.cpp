#include "lsm303dhlc.h"

void LSM303DLHC::begin(uint32_t _speed)
{
	speed = _speed;
	i2c->begin(speed);
	
	write_reg(LSM303_ACC_ADDR,LSM303A_CTRL_REG1,0x37);
	write_reg(LSM303_ACC_ADDR,LSM303A_CTRL_REG4,0x10);
	
	write_reg(LSM303_MAG_ADDR,LSM303M_CRB_REG,0x20);
	write_reg(LSM303_MAG_ADDR,LSM303M_MR_REG,0x00);
	
}
void LSM303DLHC::write_reg(u8 slave_addr,u8 reg,u8 value)
{
	i2c->take_i2c_right(speed);
	i2c->write_byte(slave_addr,reg,value);
	i2c->release_i2c_right();
}
uint8_t LSM303DLHC::read_reg(u8 slave_addr,u8 reg)
{
	u8 value;
	i2c->take_i2c_right(speed);
	i2c->read_byte(slave_addr,reg,&value);
	i2c->release_i2c_right();

	return value;
}
void LSM303DLHC::read_acc(void)
{
		u8 buf[6];

	acc_x = ((read_reg(LSM303_ACC_ADDR,LSM303A_OUT_X_H)<<8) + read_reg(LSM303_ACC_ADDR,LSM303A_OUT_X_L));
	acc_y = ((read_reg(LSM303_ACC_ADDR,LSM303A_OUT_Y_H)<<8) + read_reg(LSM303_ACC_ADDR,LSM303A_OUT_Y_L));
	acc_z = ((read_reg(LSM303_ACC_ADDR,LSM303A_OUT_Z_H)<<8) + read_reg(LSM303_ACC_ADDR,LSM303A_OUT_Z_L));
//	i2c->read_byte(LSM303_ACC_ADDR,LSM303A_MULTIPLE,buf,6);
//	mag_x = (buf[0]<<8) + buf[1];
//	mag_x = (buf[2]<<8) + buf[3];
//	mag_x = (buf[4]<<8) + buf[5];

}
void LSM303DLHC::read_mag(void)
{
	u8 buf[6];
	i2c->read_byte(LSM303_MAG_ADDR,LSM303M_MULTIPLE,buf,6);
	mag_x = (buf[0]<<8) + buf[1];
	mag_y = (buf[2]<<8) + buf[3];
	mag_z = (buf[4]<<8) + buf[5];
//	mag_x = ((read_reg(LSM303_MAG_ADDR,LSM303M_OUT_X_H)<<8) + read_reg(LSM303_ACC_ADDR,LSM303M_OUT_X_L));
//	mag_y = ((read_reg(LSM303_MAG_ADDR,LSM303M_OUT_Y_H)<<8) + read_reg(LSM303_ACC_ADDR,LSM303M_OUT_Y_L));
//	mag_z = ((read_reg(LSM303_MAG_ADDR,LSM303M_OUT_Z_H)<<8) + read_reg(LSM303_ACC_ADDR,LSM303M_OUT_Z_L));

}
