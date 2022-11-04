#ifndef _dianwei_H
#define _dianwei_H

#include "main.h"


#define	LCD_2A	0X1F08
#define	LCD_2B	0X1F04
#define	LCD_2C	0X1F02
#define	LCD_2D	0X1F01
#define	LCD_2E	0X2001
#define	LCD_2F	0X2004
#define	LCD_2G	0X2002

#define	LCD_3A	0X1D08
#define	LCD_3B	0X1D04
#define	LCD_3C	0X1D02
#define	LCD_3D	0X1D01
#define	LCD_3E	0X1E01
#define	LCD_3F	0X1E04
#define	LCD_3G	0X1E02

#define	LCD_4A	0X1B08
#define	LCD_4B	0X1B04
#define	LCD_4C	0X1B02
#define	LCD_4D	0X1B01
#define	LCD_4E	0X1C01
#define	LCD_4F	0X1C04
#define	LCD_4G	0X1C02

#define	LCD_5A	0X1908
#define	LCD_5B	0X1904
#define	LCD_5C	0X1902
#define	LCD_5D	0X1901
#define	LCD_5E	0X1A01
#define	LCD_5F	0X1A04
#define	LCD_5G	0X1A02

#define	LCD_6A	0X1708
#define	LCD_6B	0X1704
#define	LCD_6C	0X1702
#define	LCD_6D	0X1701
#define	LCD_6E	0X1801
#define	LCD_6F	0X1804
#define	LCD_6G	0X1802


#define	LCD_7A	0X1508
#define	LCD_7B	0X1504
#define	LCD_7C	0X1502
#define	LCD_7D	0X1501
#define	LCD_7E	0X1601
#define	LCD_7F	0X1604
#define	LCD_7G	0X1602


#define	LCD_8A	0X1308
#define	LCD_8B	0X1304
#define	LCD_8C	0X1302
#define	LCD_8D	0X1301
#define	LCD_8E	0X1401
#define	LCD_8F	0X1404
#define	LCD_8G	0X1402


#define	LCD_9A	0X1108
#define	LCD_9B	0X1104
#define	LCD_9C	0X1102
#define	LCD_9D	0X1101
#define	LCD_9E	0X1201
#define	LCD_9F	0X1204
#define	LCD_9G	0X1202

#define	LCD_10A	0X0F08
#define	LCD_10B	0X0F04
#define	LCD_10C	0X0F02
#define	LCD_10D	0X0F01
#define	LCD_10E	0X1001
#define	LCD_10F	0X1004
#define	LCD_10G	0X1002

#define	LCD_11A	0X0908
#define	LCD_11B	0X0904
#define	LCD_11C	0X0902
#define	LCD_11D	0X0901
#define	LCD_11E	0X0802
#define	LCD_11F	0X0808
#define	LCD_11G	0X0804

#define	LCD_12A	0X0708
#define	LCD_12B	0X0704
#define	LCD_12C	0X0702
#define	LCD_12D	0X0701
#define	LCD_12E	0X0602
#define	LCD_12F	0X0608
#define	LCD_12G	0X0604

#define	LCD_13A	0X0508
#define	LCD_13B	0X0504
#define	LCD_13C	0X0502
#define	LCD_13D	0X0501
#define	LCD_13E	0X0402    //0x04寄存器地址   0x03数据值
#define	LCD_13F	0X0408
#define	LCD_13G	0X0404

#define	LCD_14A	0X0308    //0x03
#define	LCD_14B	0X0304
#define	LCD_14C	0X0302
#define	LCD_14D	0X0301
#define	LCD_14E	0X0202    //0x02
#define	LCD_14F	0X0208
#define	LCD_14G	0X0204

#define	LCD_15A	0X0108    //0x01
#define	LCD_15B	0X0104
#define	LCD_15C	0X0102
#define	LCD_15D	0X0101
#define	LCD_15E	0X0002   //0x00
#define	LCD_15F	0X0008
#define	LCD_15G	0X0004


#define	LCD_T1	0X1E08
#define	LCD_T2	0X1C08
#define	LCD_T3	0X2008
#define	LCD_T4	0X1808
#define	LCD_T5	0X1A08
#define	LCD_T6	0X1608
#define	LCD_T7	0X1408
#define	LCD_T8	0X1008
#define	LCD_T9	0X1208
#define	LCD_T10	0X2108
#define	LCD_T11 0X2104
#define	LCD_T12	0X2102
#define	LCD_T13	0X2101
#define	LCD_T14	0X0A04
#define	LCD_T15	0X0A08
#define	LCD_T16	0X0B01
#define	LCD_T17	0X0E08
#define	LCD_T18 0X0D02
#define	LCD_T19	0X0D01
#define	LCD_T20	0X0E02
#define	LCD_T21	0X0E04
#define	LCD_T22	0X0D08
//#define	LCD_T23	0X70
//#define	LCD_T24	0X80
#define	LCD_T25	0X0D04
#define	LCD_T26	0X0C08
#define	LCD_T27	0X0C04
#define	LCD_T28	0X0C02
#define	LCD_T29	0X0C01
#define	LCD_T30	0X0B08
#define	LCD_T31	0X0B04
#define	LCD_T32	0X0B02
#define	LCD_T33	0X0E01

#define LCD_DP11 0X0801   //0x08
#define LCD_DP12 0X0601   //0x06
#define LCD_DP13 0X0401   //0x04
#define LCD_DP14 0X0201   //0x02

//显示单位标识符
#define LCD_DW_psf    LCD_T1    //psf
#define LCD_DW_psi    LCD_T2    //psi
#define LCD_DW_bar    LCD_T4    //bar
#define LCD_DW_Pa     LCD_T6    //Pa
#define LCD_DW_KPa    LCD_T7    //KPa
#define LCD_DW_MPa    LCD_T8    //MPa
#define LCD_DW_BF     LCD_T13   //%
#define LCD_DW_dp     LCD_T12   //dp
#define LCD_DW_F      LCD_T11   //F
#define LCD_DW_C      LCD_T10   //C
#define LCD_DW_M      LCD_T14   //M
#define LCD_DW_mm     LCD_T15   //mm
#define LCD_DW_mA     LCD_T16   //mA
#define LCD_DW_V      LCD_T32   //V
#define LCD_DW_ppb    LCD_T31   //ppb
#define LCD_DW_ppm    LCD_T30   //ppm
#define LCD_DW_CD     LCD_T18   //充电标志
#define LCD_DW_BT     LCD_T25   //BLUETOOTH  蓝牙
#define LCD_DW_DW     LCD_T22   //定位标志
#define LCD_DW_BAT    LCD_T17   //电池框标志

#define LCD_AA 0XAA
#define LCD_BB 0XBB

#define Num_0_4  0x0f  //1111_1100//abcdef
#define Num_0_8  0x0f  //1111_1100//abcdef

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
#define Sign_dec   0x02      //0000_0010g
#define Sign_dec_one  0x62        //0110_0010  bcg

#define Sign_        0x10         //0001_0000   d
#define Sign_o       0x3a         //0011_1010   d
#define Sign_F       0x8e         //1000_1110   d
#define Sign_i       0x20         //0010_0000
#define Sign_d       0x7a         //0111_1010
#define Sign_S        0XB6  //1011_0110//afgcd

const uint16_t single_DB[4]={LCD_T29,LCD_T28,LCD_T27,LCD_T26};     //信号强度
const uint16_t BAT_BF[4] = {LCD_T19,LCD_T20,LCD_T21,LCD_T33};      //电池电量显示
const uint16_t LCD_DW[16] =                              //单位数组
{
	LCD_DW_MPa,LCD_DW_KPa,LCD_DW_Pa,LCD_DW_bar,LCD_DW_psi,LCD_DW_psf,
	LCD_DW_ppm,LCD_DW_ppb,LCD_DW_V,LCD_DW_mA,LCD_DW_mm,LCD_DW_M,
	LCD_DW_C,LCD_DW_F,LCD_DW_dp,LCD_DW_BF
};
const uint16_t ZT_TB[4] = {LCD_DW_CD,LCD_DW_BT,LCD_DW_DW,0x0000};    //状态指示图标
const uint16_t CL_TB[4] = {LCD_T3,LCD_T5,LCD_T9,LCD_T17};       //常亮内容
	                               

//const LCD_SCREEN[34][4]
const   uint16_t  Num_A[14][7]=                       //每个8字的对应笔画值
{                                                     
  LCD_2A,LCD_2B,LCD_2C,LCD_2D,LCD_2E,LCD_2F,LCD_2G,    //0
  LCD_3A,LCD_3B,LCD_3C,LCD_3D,LCD_3E,LCD_3F,LCD_3G,
  LCD_4A,LCD_4B,LCD_4C,LCD_4D,LCD_4E,LCD_4F,LCD_4G,
  LCD_5A,LCD_5B,LCD_5C,LCD_5D,LCD_5E,LCD_5F,LCD_5G,
  LCD_6A,LCD_6B,LCD_6C,LCD_6D,LCD_6E,LCD_6F,LCD_6G,
  LCD_7A,LCD_7B,LCD_7C,LCD_7D,LCD_7E,LCD_7F,LCD_7G,
  LCD_8A,LCD_8B,LCD_8C,LCD_8D,LCD_8E,LCD_8F,LCD_8G,
  LCD_9A,LCD_9B,LCD_9C,LCD_9D,LCD_9E,LCD_9F,LCD_9G,
	LCD_10A,LCD_10B,LCD_10C,LCD_10D,LCD_10E,LCD_10F,LCD_10G,    //8
	
	LCD_11A,LCD_11B,LCD_11C,LCD_11D,LCD_11E,LCD_11F,LCD_11G,   //9中间数值区域 最右侧
  LCD_12A,LCD_12B,LCD_12C,LCD_12D,LCD_12E,LCD_12F,LCD_12G,
  LCD_13A,LCD_13B,LCD_13C,LCD_13D,LCD_13E,LCD_13F,LCD_13G,
  LCD_14A,LCD_14B,LCD_14C,LCD_14D,LCD_14E,LCD_14F,LCD_14G,
  LCD_15A,LCD_15B,LCD_15C,LCD_15D,LCD_15E,LCD_15F,LCD_15G,   //中间数值区域 最左侧
};

//数码0-9-abcdefg对应显示的位。
const uint8_t     Num_0_9[10]={Num_0,Num_1,Num_2,Num_3,Num_4,Num_5,Num_6,Num_7,Num_8,Num_9};
const uint8_t   Signal[2]={Sign_dec,Sign_dec_one};//负号：-，-1
uint8_t F_flash_flag;
uint8_t F_flash_cnt;


#endif
