#ifndef _dianwei_H
#define _dianwei_H

#include "main.h"

#define LCD_COL     0X0001
#define LCD_3E      0X0002
#define LCD_3G      0X0004
#define LCD_3F      0X0008
#define LCD_2A      0X0010
#define LCD_2B      0X0020
#define LCD_2C      0X0040
#define LCD_2D      0X0080

#define LCD_3D      0X0101
#define LCD_3C      0X0102
#define LCD_3B      0X0104
#define LCD_3A      0X0108
#define LCD_2F      0X0110
#define LCD_2G      0X0120
#define LCD_2E      0X0140

#define LCD_4E      0X0202
#define LCD_4G      0X0204
#define LCD_4F      0X0208
#define LCD_1A      0X0210
#define LCD_1B      0X0220
#define LCD_1C      0X0240
#define LCD_1D      0X0280

#define LCD_4D      0X0301
#define LCD_4C      0X0302
#define LCD_4B      0X0304
#define LCD_4A      0X0308
#define LCD_1F      0X0310
#define LCD_1G      0X0320
#define LCD_1E      0X0340

#define LCD_P1      0X0401
#define LCD_5D      0X0402
#define LCD_5E      0X0404
#define LCD_5C      0X0408
#define LCD_5G      0X0410
#define LCD_5F      0X0420
#define LCD_5B      0X0440
#define LCD_5A      0X0480

#define LCD_P2      0X0501
#define LCD_6D      0X0502
#define LCD_6E      0X0504
#define LCD_6C      0X0508
#define LCD_6G      0X0510
#define LCD_6F      0X0520
#define LCD_6B      0X0540
#define LCD_6A      0X0580

#define LCD_P3      0X0601
#define LCD_7D      0X0602
#define LCD_7E      0X0604
#define LCD_7C      0X0608
#define LCD_7G      0X0610
#define LCD_7F      0X0620
#define LCD_7B      0X0640
#define LCD_7A      0X0680

#define LCD_P4      0X0701
#define LCD_8D      0X0702
#define LCD_8E      0X0704
#define LCD_8C      0X0708
#define LCD_8G      0X0710
#define LCD_8F      0X0720
#define LCD_8B      0X0740
#define LCD_8A      0X0780

#define LCD_S1      0X0801
#define LCD_9D      0X0802
#define LCD_9E      0X0804
#define LCD_9C      0X0808
#define LCD_9G      0X0810
#define LCD_9F      0X0820
#define LCD_9B      0X0840
#define LCD_9A      0X0880

#define LCD_P5      0X0901
#define LCD_10D     0X0902
#define LCD_10E     0X0904
#define LCD_10C     0X0908
#define LCD_10G     0X0910
#define LCD_10F     0X0920
#define LCD_10B     0X0940
#define LCD_10A     0X0980

#define LCD_P6      0X0A01
#define LCD_11D     0X0A02
#define LCD_11E     0X0A04
#define LCD_11C     0X0A08
#define LCD_11G     0X0A10
#define LCD_11F     0X0A20
#define LCD_11B     0X0A40
#define LCD_11A     0X0A80

#define LCD_S7      0X0B01
#define LCD_12D     0X0B02
#define LCD_12E     0X0B04
#define LCD_12C     0X0B08
#define LCD_12G     0X0B10
#define LCD_12F     0X0B20
#define LCD_12B     0X0B40
#define LCD_12A     0X0B80

#define LCD_S12     0X0C01
#define LCD_S11     0X0C02
#define LCD_S9      0X0C04
#define LCD_S8      0X0C08

#define LCD_28B     0X0D01
#define LCD_28G     0X0D02
#define LCD_28C     0X0D04
#define LCD_S10     0X0D08
#define LCD_20B     0X0D10
#define LCD_20G     0X0D20
#define LCD_20C     0X0D40

#define LCD_28A     0X0E01
#define LCD_28F     0X0E02
#define LCD_28E     0X0E04
#define LCD_28D     0X0E08
#define LCD_20A     0X0E10
#define LCD_20F     0X0E20
#define LCD_20E     0X0E40
#define LCD_20D     0X0E80

#define LCD_27B     0X0F01
#define LCD_27G     0X0F02
#define LCD_27C     0X0F04
#define LCD_P18     0X0F08
#define LCD_19B     0X0F10
#define LCD_19G     0X0F20
#define LCD_19C     0X0F40
#define LCD_P12     0X0F80

#define LCD_27A     0X1001
#define LCD_27F     0X1002
#define LCD_27E     0X1004
#define LCD_27D     0X1008
#define LCD_19A     0X1010
#define LCD_19F     0X1020
#define LCD_19E     0X1040
#define LCD_19D     0X1080

#define LCD_26B     0X1101
#define LCD_26G     0X1102
#define LCD_26C     0X1104
#define LCD_P17     0X1108
#define LCD_18B     0X1110
#define LCD_18G     0X1120
#define LCD_18C     0X1140
#define LCD_P11     0X1180

#define LCD_26A     0X1201
#define LCD_26F     0X1202
#define LCD_26E     0X1204
#define LCD_26D     0X1208
#define LCD_18A     0X1210
#define LCD_18F     0X1220
#define LCD_18E     0X1240
#define LCD_18D     0X1280

#define LCD_T5      0X1301
#define LCD_T4      0X1302
#define LCD_T3      0X1304
#define LCD_T2      0X1308

#define LCD_S17     0X1401
#define LCD_S18     0X1402
#define LCD_S19     0X1404
#define LCD_T1      0X1408

#define LCD_S13     0X1501
#define LCD_S14     0X1502
#define LCD_S15     0X1504
#define LCD_S16     0X1508
#define LCD_S6      0X1510
#define LCD_S4      0X1520
#define LCD_S3      0X1540
#define LCD_S2      0X1580

#define LCD_25B     0X1601
#define LCD_25G     0X1602
#define LCD_25C     0X1604
#define LCD_S5      0X1608
#define LCD_17B     0X1610
#define LCD_17G     0X1620
#define LCD_17C     0X1640

#define LCD_25A     0X1701
#define LCD_25F     0X1702
#define LCD_25E     0X1704
#define LCD_25D     0X1708
#define LCD_17A     0X1710
#define LCD_17F     0X1720
#define LCD_17E     0X1740
#define LCD_17D     0X1780

#define LCD_24B     0X1801
#define LCD_24G     0X1802
#define LCD_24C     0X1804
#define LCD_P16     0X1808
#define LCD_16B     0X1810
#define LCD_16G     0X1820
#define LCD_16C     0X1840
#define LCD_P10     0X1880

#define LCD_24A     0X1901
#define LCD_24F     0X1902
#define LCD_24E     0X1904
#define LCD_24D     0X1908
#define LCD_16A     0X1910
#define LCD_16F     0X1920
#define LCD_16E     0X1940
#define LCD_16D     0X1980

#define LCD_23B     0X1A01
#define LCD_23G     0X1A02
#define LCD_23C     0X1A04
#define LCD_P15     0X1A08
#define LCD_15B     0X1A10
#define LCD_15G     0X1A20
#define LCD_15C     0X1A40
#define LCD_P9      0X1A80

#define LCD_23A     0X1B01
#define LCD_23F     0X1B02
#define LCD_23E     0X1B04
#define LCD_23D     0X1B08
#define LCD_15A     0X1B10
#define LCD_15F     0X1B20
#define LCD_15E     0X1B40
#define LCD_15D     0X1B80

#define LCD_22B     0X1C01
#define LCD_22G     0X1C02
#define LCD_22C     0X1C04
#define LCD_P14     0X1C08
#define LCD_14B     0X1C10
#define LCD_14G     0X1C20
#define LCD_14C     0X1C40
#define LCD_P8      0X1C80

#define LCD_22A     0X1D01
#define LCD_22F     0X1D02
#define LCD_22E     0X1D04
#define LCD_22D     0X1D08
#define LCD_14A     0X1D10
#define LCD_14F     0X1D20
#define LCD_14E     0X1D40
#define LCD_14D     0X1D80

#define LCD_21B     0X1E01
#define LCD_21G     0X1E02
#define LCD_21C     0X1E04
#define LCD_P13     0X1E08
#define LCD_13B     0X1E10
#define LCD_13G     0X1E20
#define LCD_13C     0X1E40
#define LCD_P7      0X1E80

#define LCD_21A     0X1F01
#define LCD_21F     0X1F02
#define LCD_21E     0X1F04
#define LCD_21D     0X1F08
#define LCD_13A     0X1F10
#define LCD_13F     0X1F20
#define LCD_13E     0X1F40
#define LCD_13D     0X1F80

#define Num_0  0xFC  //1111_1100//abcdef
#define Num_1  0X60 //0110_0000//bc
#define Num_2  0XDA //1101_1010//abged
#define Num_3  0XF2 //1111_0010//abcdg
#define Num_4  0X66  //0110_0110//fgbc
#define Num_5  0XB6  //1011_0110//afgcd
#define Num_6  0XBE  //1011_1110//afgcde
#define Num_7  0XE0 //1110_0000//abc
#define Num_8  0XFE //1111_1110//abcdefg
#define Num_9  0XF6 //1111_0110//afgbcd
#define Num_10   0x02      //0000_0010/g   ����
#define Num_11   0x01      //0000_0001/h   С��

#define LCD_DW_KPa_1        LCD_S6
#define LCD_DW_KPa_2        LCD_S4
#define LCD_DW_KPa_3        LCD_S2
#define LCD_DW_MPa_1        LCD_S5
#define LCD_DW_MPa_2        LCD_S3
#define LCD_DW_MPa_3        LCD_S1
#define LCD_DW_C_1          LCD_S12
#define LCD_DW_C_2          LCD_S10
#define LCD_DW_C_3          LCD_S8
#define LCD_DW_F_1          LCD_S11
#define LCD_DW_F_2          LCD_S9
#define LCD_DW_F_3          LCD_S7

#define LCD_BZ_GPS          LCD_S17     //GPS��λ��־
#define LCD_BZ_BL           LCD_S18     //������־
#define LCD_BZ_PW           LCD_S19     //����־

const uint16_t single_DB[4]={LCD_S16,LCD_S15,LCD_S14,LCD_S13};     //�ź�ǿ�� 1-4��
const uint16_t BAT_BF[4] = {LCD_T1,LCD_T2,LCD_T3,LCD_T4};         //��ص�����ʾ
const uint16_t CL_TB[2] = {LCD_COL,LCD_T5};       //��������


const   uint16_t  Num_A[28][8]=                       //ÿ��8�ֵĶ�Ӧ�ʻ�ֵ
{  
	LCD_1A,LCD_1B,LCD_1C,LCD_1D,LCD_1E,LCD_1F,LCD_1G,0X0000,    //ʱ	
	LCD_2A,LCD_2B,LCD_2C,LCD_2D,LCD_2E,LCD_2F,LCD_2G,0X0000,    //ʱ
	LCD_3A,LCD_3B,LCD_3C,LCD_3D,LCD_3E,LCD_3F,LCD_3G,0X0000,    //��
	LCD_4A,LCD_4B,LCD_4C,LCD_4D,LCD_4E,LCD_4F,LCD_4G,0X0000,    //��
	
	LCD_5A,LCD_5B,LCD_5C,LCD_5D,LCD_5E,LCD_5F,LCD_5G,LCD_P1,    //DATA3_P
	LCD_6A,LCD_6B,LCD_6C,LCD_6D,LCD_6E,LCD_6F,LCD_6G,LCD_P2,    //
	LCD_7A,LCD_7B,LCD_7C,LCD_7D,LCD_7E,LCD_7F,LCD_7G,LCD_P3,    //
	LCD_8A,LCD_8B,LCD_8C,LCD_8D,LCD_8E,LCD_8F,LCD_8G,LCD_P4,    //
	LCD_9A,LCD_9B,LCD_9C,LCD_9D,LCD_9E,LCD_9F,LCD_9G,0X0000,    //
	LCD_10A,LCD_10B,LCD_10C,LCD_10D,LCD_10E,LCD_10F,LCD_10G,LCD_P5,    //DATA3_T	
	LCD_11A,LCD_11B,LCD_11C,LCD_11D,LCD_11E,LCD_11F,LCD_11G,LCD_P6,   //
	LCD_12A,LCD_12B,LCD_12C,LCD_12D,LCD_12E,LCD_12F,LCD_12G,0X0000,   //
	
	LCD_13A,LCD_13B,LCD_13C,LCD_13D,LCD_13E,LCD_13F,LCD_13G,LCD_P7,   //DATA2_P
	LCD_14A,LCD_14B,LCD_14C,LCD_14D,LCD_14E,LCD_14F,LCD_14G,LCD_P8,   //
	LCD_15A,LCD_15B,LCD_15C,LCD_15D,LCD_15E,LCD_15F,LCD_15G,LCD_P9,   //
	LCD_16A,LCD_16B,LCD_16C,LCD_16D,LCD_16E,LCD_16F,LCD_16G,LCD_P10,   //
	LCD_17A,LCD_17B,LCD_17C,LCD_17D,LCD_17E,LCD_17F,LCD_17G,0X0000,   //
	LCD_18A,LCD_18B,LCD_18C,LCD_18D,LCD_18E,LCD_18F,LCD_18G,LCD_P11,   //DATA2_T
	LCD_19A,LCD_19B,LCD_19C,LCD_19D,LCD_19E,LCD_19F,LCD_19G,LCD_P12,   //
	LCD_20A,LCD_20B,LCD_20C,LCD_20D,LCD_20E,LCD_20F,LCD_20G,0X0000,   //
	
	LCD_21A,LCD_21B,LCD_21C,LCD_21D,LCD_21E,LCD_21F,LCD_21G,LCD_P13,   //DATA1_P
	LCD_22A,LCD_22B,LCD_22C,LCD_22D,LCD_22E,LCD_22F,LCD_22G,LCD_P14,   //
	LCD_23A,LCD_23B,LCD_23C,LCD_23D,LCD_23E,LCD_23F,LCD_23G,LCD_P15,   //
	LCD_24A,LCD_24B,LCD_24C,LCD_24D,LCD_24E,LCD_24F,LCD_24G,LCD_P16,   //
	LCD_25A,LCD_25B,LCD_25C,LCD_25D,LCD_25E,LCD_25F,LCD_25G,0X0000,   //
	LCD_26A,LCD_26B,LCD_26C,LCD_26D,LCD_26E,LCD_26F,LCD_26G,LCD_P17,   //DATA1_T
	LCD_27A,LCD_27B,LCD_27C,LCD_27D,LCD_27E,LCD_27F,LCD_27G,LCD_P18,   //
	LCD_28A,LCD_28B,LCD_28C,LCD_28D,LCD_28E,LCD_28F,LCD_28G,0X0000    //
};


const  uint16_t  Number_BUF[28][8]=                       //ÿ��8�ֵĶ�Ӧ�ʻ�ֵ
{  
	LCD_1A,LCD_1B,LCD_1C,LCD_1D,LCD_1E,LCD_1F,LCD_1G,0X0000,    //ʱ	
	LCD_2A,LCD_2B,LCD_2C,LCD_2D,LCD_2E,LCD_2F,LCD_2G,0X0000,    //ʱ
	LCD_3A,LCD_3B,LCD_3C,LCD_3D,LCD_3E,LCD_3F,LCD_3G,0X0000,    //��
	LCD_4A,LCD_4B,LCD_4C,LCD_4D,LCD_4E,LCD_4F,LCD_4G,0X0000,    //��

	0,0,0,0,0,0,0,0,    //DATA3_P
	0,0,0,0,0,0,0,0,    //
	0,0,0,0,0,0,0,0,    //
	0,0,0,0,0,0,0,0,    //
	0,0,0,0,0,0,0,0X0000,    //
	0,0,0,0,0,0,0,0,    //DATA3_T	
	0,0,0,0,0,0,0,0,   //
	0,0,0,0,0,0,0,0X0000,   //

	LCD_13A,LCD_13B,LCD_13C,LCD_13D,LCD_13E,LCD_13F,LCD_13G,LCD_P7,   //DATA2_P
	LCD_14A,LCD_14B,LCD_14C,LCD_14D,LCD_14E,LCD_14F,LCD_14G,LCD_P8,   //
	LCD_15A,LCD_15B,LCD_15C,LCD_15D,LCD_15E,LCD_15F,LCD_15G,LCD_P9,   //
	LCD_16A,LCD_16B,LCD_16C,LCD_16D,LCD_16E,LCD_16F,LCD_16G,LCD_P10,   //
	LCD_17A,LCD_17B,LCD_17C,LCD_17D,LCD_17E,LCD_17F,LCD_17G,0X0000,   //
	LCD_18A,LCD_18B,LCD_18C,LCD_18D,LCD_18E,LCD_18F,LCD_18G,LCD_P11,   //DATA2_T
	LCD_19A,LCD_19B,LCD_19C,LCD_19D,LCD_19E,LCD_19F,LCD_19G,LCD_P12,   //
	LCD_20A,LCD_20B,LCD_20C,LCD_20D,LCD_20E,LCD_20F,LCD_20G,0X0000,   //

	LCD_21A,LCD_21B,LCD_21C,LCD_21D,LCD_21E,LCD_21F,LCD_21G,LCD_P13,   //DATA1_P
	LCD_22A,LCD_22B,LCD_22C,LCD_22D,LCD_22E,LCD_22F,LCD_22G,LCD_P14,   //
	LCD_23A,LCD_23B,LCD_23C,LCD_23D,LCD_23E,LCD_23F,LCD_23G,LCD_P15,   //
	LCD_24A,LCD_24B,LCD_24C,LCD_24D,LCD_24E,LCD_24F,LCD_24G,LCD_P16,   //
	LCD_25A,LCD_25B,LCD_25C,LCD_25D,LCD_25E,LCD_25F,LCD_25G,0X0000,   //
	LCD_26A,LCD_26B,LCD_26C,LCD_26D,LCD_26E,LCD_26F,LCD_26G,LCD_P17,   //DATA1_T
	LCD_27A,LCD_27B,LCD_27C,LCD_27D,LCD_27E,LCD_27F,LCD_27G,LCD_P18,   //
	LCD_28A,LCD_28B,LCD_28C,LCD_28D,LCD_28E,LCD_28F,LCD_28G,0X0000    //
};

const uint8_t     Num_0_9[12]={Num_0,Num_1,Num_2,Num_3,Num_4,Num_5,Num_6,Num_7,Num_8,Num_9,Num_10,Num_11};
const uint16_t  DW_unit[6][2]=
{
	LCD_DW_KPa_1,LCD_DW_MPa_1,
	LCD_DW_C_1,LCD_DW_F_1,
	LCD_DW_KPa_2,LCD_DW_MPa_2,
	LCD_DW_C_2,LCD_DW_F_2,
	LCD_DW_KPa_3,LCD_DW_MPa_3,
	LCD_DW_C_3,LCD_DW_F_3
};

const uint16_t DIS_Buf[6][2]=
{
	LCD_DW_KPa_1,LCD_DW_MPa_1,
	LCD_DW_C_1,LCD_DW_F_1,
	LCD_DW_KPa_2,LCD_DW_MPa_2,
	LCD_DW_C_2,LCD_DW_F_2,
	0,0,
	0,0
};

const   uint16_t test_buf[4][6]=
{
	LCD_DW_KPa_1,LCD_DW_C_1,LCD_DW_MPa_1,LCD_DW_F_1,LCD_T1,LCD_S13,
	LCD_DW_KPa_2,LCD_DW_C_2,LCD_DW_MPa_2,LCD_DW_F_2,LCD_T2,LCD_S14,
	LCD_DW_KPa_3,LCD_DW_C_3,LCD_DW_MPa_3,LCD_DW_F_3,LCD_T3,LCD_S15,
	LCD_COL,LCD_T5,LCD_S17,LCD_S19,LCD_T4,LCD_S16
};

#endif
