#!/usr/bin/python3
# -*- coding:utf-8 -*-
from grove.i2c import Bus
from enum import IntEnum
import logging
import time
import signal


class I2C_ADDR(IntEnum):
    SI115X = 0x53

class SI115X_REG(IntEnum):
    PART_ID = 0x00
    REV_ID = 0x01
    MFR_ID = 0x02
    INFO_0 = 0x03
    INFO_1 = 0x04
    HOSTIN_3 = 0x07
    HOSTIN_2 = 0x08
    HOSTIN_0 = 0x0A
    COMMAND = 0x0B
    IRQ_ENABLE = 0x0F
    RESPONSE_0 = 0x11
    RESPONSE_1 = 0x10
    IRQ_STATUS = 0x12
    HOSTOUT_0 = 0x13
    HOSTOUT_1 = 0x14
    HOSTOUT_2 = 0x15

class SI115X_CMD(IntEnum):
    RESET_CMD_CTR = 0x00
    RESET_SW = 0x01
    FORCE = 0x11
    PAUSE = 0x12
    START = 0x13
    PARAM_GET = 0x40
    PARAM_SET = 0x80

class SI115X_PARAM(IntEnum):
    I2C_ADDR = 0x00
    CHAN_LIST = 0x01
    ADCCONFIG_0 = 0x02
    ADCSENS_0 = 0x03
    ADCPOST_0 = 0x04
    MEASCONFIG_0 = 0x05
    ADCCONFIG_1 = 0x06
    ADCPOST_1 = 0x08
    ADCSENS_1 = 0x07
    MEASCONFIG_1 = 0x09
    ADCCONFIG_2 = 0x0A
    ADCSENS_2 = 0x0B
    ADCPOST_2 = 0x0C
    MEASCONFIG_2 = 0x0D
    ADCCONFIG_3 = 0x0E
    ADCSENS_3 = 0x0F
    ADCPOST_3 = 0x10
    MEASCONFIG_3 = 0x11
    ADCCONFIG_4 = 0x12
    ADCSENS_4 = 0x13
    ADCPOST_4 = 0x14
    MEASCONFIG_4 = 0x15
    ADCCONFIG_5 = 0x16
    ADCSENS_5 = 0x17
    ADCPOST_5 = 0x18
    MEASCONFIG_5 = 0x19
    MEASRATE_H = 0x1A
    MEASRATE_L = 0x1B
    MEASCOUNT_0 = 0x1C
    MEASCOUNT_1 = 0x1D
    MEASCOUNT_2 = 0x1E
    LED1_A = 0x1F
    LED1_B = 0x20
    LED2_A = 0x21
    LED2_B = 0x22
    LED3_A = 0x23
    LED3_B = 0x24
    THRESHOLD0_H = 0x25
    THRESHOLD0_L = 0x26
    THRESHOLD1_H = 0x27
    THRESHOLD1_L = 0x28
    THRESHOLD2_H = 0x29
    THRESHOLD2_L = 0x2A
    BURST = 0x2B


class grove_si115x(object):
    def __init__(self,address = I2C_ADDR.SI115X):
        self.bus = Bus()
        self.addr = address
        self._logger = logging.getLogger('grove_si115x')
        assert self.Begin() , "Please check if the I2C device insert in I2C of Base Hat"
    def __del__(self):
        self.bus.close()
    def __exit__(self):
        self.bus.close()

    def Begin(self):
        if self._ReadByte(SI115X_REG.PART_ID) != 0x51:
            return False
        self.SendCommand(SI115X_CMD.START)

        self.WriteParamData(SI115X_PARAM.CHAN_LIST, 0B000010)
        self.WriteParamData(SI115X_PARAM.MEASRATE_H, 0)
        self.WriteParamData(SI115X_PARAM.MEASRATE_L, 1)  # 1 for a base period of 800 us
        self.WriteParamData(SI115X_PARAM.MEASCOUNT_0, 5)
        self.WriteParamData(SI115X_PARAM.MEASCOUNT_1, 10)
        self.WriteParamData(SI115X_PARAM.MEASCOUNT_2, 10)
        self.WriteParamData(SI115X_PARAM.THRESHOLD0_L, 200)
        self.WriteParamData(SI115X_PARAM.THRESHOLD0_H, 0)

        self._WriteByte(SI115X_REG.IRQ_ENABLE, 0B000010)

        conf = [0B00000000, 0B00000010, 0B00000001, 0B11000001]
        # Configures channel 1
        self.WriteParamData(SI115X_PARAM.ADCCONFIG_1, conf[0])
        self.WriteParamData(SI115X_PARAM.ADCSENS_1, conf[1])
        self.WriteParamData(SI115X_PARAM.ADCPOST_1, conf[2])
        self.WriteParamData(SI115X_PARAM.MEASCONFIG_1, conf[3])

        return True

    #read param data
    def ReadParamData(self,Reg):
        self._WriteByte(SI115X_REG.COMMAND, Reg | SI115X_CMD.PARAM_GET)
        return self._ReadByte(SI115X_REG.RESPONSE_1)

    #writ param data
    def WriteParamData(self,Reg,Value):
        while True:
            resp_pre = self._ReadByte(SI115X_REG.RESPONSE_0)
            self._WriteByte(SI115X_REG.HOSTIN_0, Value)
            self._WriteByte(SI115X_REG.COMMAND, Reg | SI115X_CMD.PARAM_SET)
            if self._ReadByte(SI115X_REG.RESPONSE_0) > resp_pre:
                break

    @property
    def ReadVisible(self):
        self.SendCommand(SI115X_CMD.FORCE)
        byte_h = self._ReadByte(SI115X_REG.HOSTOUT_0)
        byte_m = self._ReadByte(SI115X_REG.HOSTOUT_1)
        byte_l = self._ReadByte(SI115X_REG.HOSTOUT_2)
        return abs((byte_h << 16) + (byte_m << 8) + byte_l)

    @property
    def ReadIR(self):
        self.SendCommand(SI115X_CMD.FORCE)
        byte_h = self._ReadByte(SI115X_REG.HOSTOUT_0)
        byte_m = self._ReadByte(SI115X_REG.HOSTOUT_1)
        byte_l = self._ReadByte(SI115X_REG.HOSTOUT_2)
        return abs(((byte_h << 16) + (byte_m << 8) + byte_l) / 3)

    @property
    def ReadUV(self):
        self.SendCommand(SI115X_CMD.FORCE)
        byte_h = self._ReadByte(SI115X_REG.HOSTOUT_0)
        byte_m = self._ReadByte(SI115X_REG.HOSTOUT_1)
        byte_l = self._ReadByte(SI115X_REG.HOSTOUT_2)
        return abs(((byte_h << 16) + (byte_m << 8) + byte_l) * 0.04)

    def SendCommand(self,Command):
        while True:
            cmmnd_ctr = self._ReadByte(SI115X_REG.RESPONSE_0)
            self._WriteByte(SI115X_REG.COMMAND, Command)
            self._ReadByte(SI115X_REG.RESPONSE_0)
            r = self._ReadByte(SI115X_REG.RESPONSE_0)
            if(r > cmmnd_ctr):
                break

    # Read 8 bit data from Reg
    def _ReadByte(self,Reg):
        try:
            read_data = self.bus.read_byte_data(self.addr,Reg)
        except OSError:
            raise  OSError("Please check if the I2C device insert in I2C of Base Hat")
        return read_data

    # Write 8 bit data to Reg
    def _WriteByte(self,Reg,Value):
        try:
            self.bus.write_byte_data(self.addr,Reg,Value)
        except OSError:
            raise OSError("Please check if the I2C device insert in I2C of Base Hat")

    # read 16 bit data from Reg
    def _ReadHalfWord(self,Reg):
        try:
            block = self.bus.read_i2c_block_data(self.addr,Reg, 2)
        except OSError:
            raise OSError("Please check if the I2C device insert in I2C of Base Hat")
        read_data = (block[0] & 0xff) | (block[1] << 8)
        return read_data


def handler(signalnum, handler):
    print("Please use Ctrl C to quit")

def main():
    signal.signal(signal.SIGTSTP, handler) # Ctrl-z
    signal.signal(signal.SIGQUIT, handler) # Ctrl-\
    SI1151 = grove_si115x()
    print("Please use Ctrl C to quit")
    
    while True:
        try:
            print('Visible %03d UV %.2f IR %03d' % (SI1151.ReadVisible , SI1151.ReadUV , SI1151.ReadIR), end=" ")
            print('\r', end='')
            time.sleep(0.5)
        except KeyboardInterrupt:
            break

if __name__ == "__main__":
    main()
    