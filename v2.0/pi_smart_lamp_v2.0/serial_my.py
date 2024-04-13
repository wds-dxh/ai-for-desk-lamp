import serial
import struct



# 串口配置
def init_serial(baudrate=9600,port='/dev/ttyS0'):       #默认波特率9600，端口/dev/ttyS0
    global ser
    ser = serial.Serial(
        port = port,  # 串口设备文件路径，可能根据你的硬件设置而有所不同
        baudrate=baudrate,      # 波特率，需要与设备一致

        parity=serial.PARITY_NONE,  # 奇偶校验位，根据设备配置
        stopbits=serial.STOPBITS_ONE,  # 停止位，根据设备配置
        bytesize=serial.EIGHTBITS  # 数据位，根据设备配置
    )

# ser = serial.Serial(
#     port='/dev/ttyS0',  # 串口设备文件路径，可能根据你的硬件设置而有所不同
#     baudrate=9600,      # 波特率，需要与设备一致
#     parity=serial.PARITY_NONE,  # 奇偶校验位，根据设备配置
#     stopbits=serial.STOPBITS_ONE,  # 停止位，根据设备配置
#     bytesize=serial.EIGHTBITS  # 数据位，根据设备配置
# )


def sending_data(cx = 0,cy = 0,cw = 0,ch = 0):
    global ser
    pack = struct.pack('<BBBBBBB', #格式为俩个字符俩个短整型(2 字节)
        0x2C, #帧头1
        0x12, #帧头2
        int(cx), # up sample by 4 #数据1
        int(cy), # up sample by 4 #数据2
        int(cw), # up sample by 4 #数据1
        int(ch), # up sample by 4 #数据2
        0x5B)
    ser.write(pack);
