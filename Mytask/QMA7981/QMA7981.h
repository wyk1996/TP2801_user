#ifndef _QMA7981_H_
#define _QMA7981_H_

#define QMA7981_ADDRESS    0X12     //传感器地址
#define QMA7981_CHIP_ID    0X00     //传感器唯一ID
#define QMA7981_DATA_ADD   0X01      //传感器数据ID
#define QMA7981_ACC_X      0X01    //0X01~0X02   x轴加速度
#define QMA7981_ACC_Y      0X03    //0X03~0X04   y轴加速度
#define QMA7981_ACC_Z      0X05    //0X05~0X06   z轴加速度
#define QMA7981_STEP_CNT   0X07    //0X07~0X08   计步器数据

#define QMA7981_INT_STO    0X09    // 加速度中断使能
#define QMA7981_INT_STO_DEF        0X00
#define QMA7981_NO_MOT             0X80
#define QMA7981_ANY_MOT_SIGN       0X08
#define QMA7981_ANY_MOT_FIRST_Z    0X04
#define QMA7981_ANY_MOT_FIRST_Y    0X02
#define QMA7981_ANY_MOT_FIRST_X    0X01

#define QMA7981_FSR        0X0F     //默认2G
#define QMA7981_FSR_DEF    0XF0     //初始值
#define QMA7981_FSR_2G     0XF1
#define QMA7981_FSR_4G     0XF2
#define QMA7981_FSR_8G     0XF4
#define QMA7981_FSR_16G    0XF8
#define QMA7981_FSR_32G    0XFF

#define QMA7981_BW         0X10

#define QMA7981_PM         0X11     //  工作模式设置
#define QMA7981_PM_DEF     0X40    //初始值
#define QMA7981_PM_ACTIVE     0X80   //正常工作模式
#define QMA7981_PM_STANDBY    0X00    //standby mode
#define QMA7981_PM_RSTB_8     0X30    //Reset clock setting
#define QMA7981_PM_RSTB_6     0X20
#define QMA7981_PM_RSTB_4     0X10
#define QMA7981_PM_RSTB_3     0X00
#define QMA7981_PM_MCLK_500   0X00     //master clock   KHz
#define QMA7981_PM_MCLK_333   0X01
#define QMA7981_PM_MCLK_200   0X02
#define QMA7981_PM_MCLK_100   0X03
#define QMA7981_PM_MCLK_50    0X04
#define QMA7981_PM_MCLK_25    0X05
#define QMA7981_PM_MCLK_12_5  0X06
#define QMA7981_PM_MCLK_5     0X07

#define QMA7981_INT_EN0       0X16      //中断输出使能设置0
#define QMA7981_INT_EN0_DEF   0XB1
#define QMA7981_SIG_STEP_IEN      0X40
#define QMA7981_STEP_IEN          0X08
#define QMA7981_HD_EN             0X04
#define QMA7981_RAISE_EN          0X02

#define QMA7981_INT_EN1       0X17    //数据中断输出使能设置1
#define QMA7981_INT_EN1_DEF   0XE0
#define QMA7981_INT_DATA_EN   0X10

#define QMA7981_INT_EN2       0X18    //震动中断输出使能设置2
#define QMA7981_INT_EN2_DEF   0X18
#define QMA7981_NO_MOT_EN_Z   0X80    //无运动中断使能
#define QMA7981_NO_MOT_EN_Y   0X40
#define QMA7981_NO_MOT_EN_X   0X20
#define QMA7981_ANY_MOT_EN_Z  0X04    //运动中断使能
#define QMA7981_ANY_MOT_EN_Y  0X02
#define QMA7981_ANY_MOT_EN_X  0X01

#define QMA7981_INT_MAP0         0X19
#define QMA7981_INT_MAP0_DEF     0XB0
#define QMA7981_INT1_SIG_STEP    0X40
#define QMA7981_INT1_STEP        0X08
#define QMA7981_INT1_HD          0X04
#define QMA7981_INT1_RAISE       0X02
#define QMA7981_INT1_SIG_MOT     0X01

#define QMA7981_INT_MAP1         0X1A
#define QMA7981_INT_MAP1_DEF     0X62
#define QMA7981_INT1_NO_MOT      0X80
#define QMA7981_INT1_DATA        0X10
#define QMA7981_INT1_ANY_MOT     0X01

#define QMA7981_INT_MAP2         0X1B
#define QMA7981_INT_MAP2_DEF     0XB0
#define QMA7981_INT2_SIG_STEP    0X40
#define QMA7981_INT2_STEP        0X08
#define QMA7981_INT2_HD          0X04
#define QMA7981_INT2_RAISE       0X02
#define QMA7981_INT2_SIG_MOT     0X01

#define QMA7981_INT_MAP3         0X1C
#define QMA7981_INT_MAP3_DEF     0X62
#define QMA7981_INT2_NO_MOT      0X80
#define QMA7981_INT2_DATA        0X10
#define QMA7981_INT2_ANY_MOT     0X01

#define QMA7981_INTPIN_CONF      0X20
#define QMA7981_INTPINCONF_DEF   0X05
#define QMA7981_DIS_PU_SENB      0X80
#define QMA7981_DIS_IE_AD0       0X40
#define QMA7981_EN_SPI3W         0X20
#define QMA7981_STEP_COUNT_PEAK  0X10
#define QMA7981_INT2_OD          0X08
#define QMA7981_INT2_LVL         0X04
#define QMA7981_INT1_OD          0X02
#define QMA7981_INT1_LVL         0X01


#define QMA7981_MOT_CONF0        0X2C     //静止中断延时参数设置
#define QMA7981_MOT_CONF0_DEF    0X00
#define QMA7981_NO_MOT_DUR_1     0X00     // (<3:0>+1)*1s
#define QMA7981_NO_MOT_DUR_5     0X10    // (<3:0>+4)*5s
#define QMA7981_NO_MOT_DUR_10    0X20    // (<3:0>+10)*10s

#define QMA7981_MOT_CONF1        0X2D      //静止中断报警阈值
#define QMA7981_MOF_CONF2        0X2E      //运动中断报警阈值

#define QMA7981_RESET            0X36
#define QMA7981_RESET_DEF        0X00
#define QMA7981_RESET_DAT        0XB6


#define shock_sda_pin      LL_GPIO_PIN_3
#define shock_scl_pin      LL_GPIO_PIN_4
#define shock_int_pin      LL_GPIO_PIN_5
#define shock_port         GPIOB

void iic7_init(void);
void QMA7981_init(void);
void QMA7981_read_AC(void);
void shock_interrupt_init(void);
#endif



