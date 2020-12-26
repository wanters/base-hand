# 基站-手持设备
## 1.工作流程
```
    1.通信方式
        pc与基站通过udp通信，基站与手持通过串口无线模块（E22-230T30S，LoRa无线模块）通信。
    2.数据流
            控制命令[开始/停止上报基站/手持位置信息]          通知手持上报位置信息
            控制命令[心跳]                                  下发目标方位
            目标方位                
        PC          ------[udp]------>          基站        -----[串口无线模块]------>     手持
                    <------[udp]------                      <-----[串口无线模块]------     
            控制命令返回[开始/停止上报基站/手持位置信息]       上报位置信息
            控制命令[心跳]返回       
            设备[基站/手持]位置信息
    3.自定义通信协议
        基站板卡和上位机通信协议.docx
        基站板卡和手持板卡通信协议.docx
    4.E22-230T30S参考资料
```
## 2.驱动开发
```
    1.基站与手持采用相同原理图
    2.主控芯片：STM32F429VIT6
    3.网络[lan8720]
        PC1     ------> ETH_MDC
        PA1     ------> ETH_REF_CLK
        PA2     ------> ETH_MDIO
        PA7     ------> ETH_CRS_DV
        PC4     ------> ETH_RXD0
        PC5     ------> ETH_RXD1
        PB11    ------> ETH_TX_EN
        PB12    ------> ETH_TXD0
        PB13    ------> ETH_TXD1 
        PC3     ------> ETH_RESET
    4.gps串口[串口4]
        PA0/WKUP     ------> UART4_TX
        PC11         ------> UART4_RX 
    5.无线串口[串口2]
        PD5     ------> USART2_TX
        PD6     ------> USART2_RX  
    6.调试串口[串口1]
        PA9     ------> USART1_TX
        PA10    ------> USART1_RX 
    7.定时[TIM2]
        用于计时
    8.4路模块电压电流和一路电源电压
        PC0     ------> ADC1_IN10
        PC2     ------> ADC1_IN12
        PA0/WKUP------> ADC1_IN0
        PA3     ------> ADC1_IN3
        PA4     ------> ADC1_IN4
        PA5     ------> ADC1_IN5
        PA6     ------> ADC1_IN6
        PB0     ------> ADC1_IN8
        PB1     ------> ADC1_IN9 
    9.tmp116温度 i2c
        SDL     ------> PB6
        SDA     ------> PB7
    10.oled i2c
        SDL     ------> PA8
        SDA     ------> PC9   
    11.按键-模块开关
        PE5     ------> SWITCH1
        PE4     ------> SWITCH2
        PE3     ------> SWITCH3
        PE2     ------> SWITCH4
        PB14    ------> KEY1
        PB15    ------> KEY2
        PD8     ------> KEY3
        PD9     ------> KEY4
    12.蜂鸣器
        PD1
```
## 3.策略
```
    1.强制关闭所有模块
        温度超过70度或者采集到的电源电压小于(22.41+0.35)
    2.1s采集一次温度，刷新一个电量显示
    3.按键控制模块开关
        模块开关策略
            当模块开启后，30s开30s关
    4.蜂鸣器鸣叫策略
        任意一个/多个模块开，蜂鸣器1s翻转一次
        无模块开，收到目标位置信息，0.5s反转一次，若超过20s未再次收到目标位置信息，则停止鸣叫
        无模块开，没有收到目标位置信息，则停止鸣叫
        电量低于25%时，蜂鸣器鸣叫1秒钟，电池图标闪 
    5.显示屏显示策略
    5.1驱离与迫降显示
        单开GPS不显示
        除GPS任意模块开，则显示驱离
        GPS加任意一个/多个模块开，则显示迫降
    5.2目标位置信息显示
        收到目标位置信息，则显示方向角度信息，若超过20s未再次收到目标位置信息，则清除
    5.3两屏数据切换
        显示内容
            第一屏：电池+驱离+无人机角度信息
            第二屏：电池+gps信息，显示小数点后六位（当有gps信息时）
        切换
            无操作（驱离/迫降）和角度信息时，显示本身gps信息
            有操作时，切换显示
            切换效果，分时切换，是否滚屏【目前无】
    6.可通过调试串口[串口1]配置无线串口模块
        定点传输模式，信道为20
        基站发送消息为定点广播
        手持为定点-点对点
    7.下发获取手持设备位置信息
        加入有10个设备，1s发送一次，则第十个设备需要第十秒才能上报，即10s一个周期
```
## 4.工具软件
```
    1.串口调试助手：Xcom 
    2.无线模块配置工具：RF_Setting(E22-E90(SL)) V1.9.exe
    3.取模软件：PCtoLCD2002
    4.网络调试助手：NetAssist.exe
```
## 5.待完善
```
    1.上电读取无线模块配置，有时会失败
        1.1因为采用的是定点模式，所以需要获取下信道，如果失败的话就会将信道发错，可以将信道固定死，或者信道存到flash中，通过串口修改，与无线配置保持一致即可
        1.2查找读取失败原因
        1.3电路设计无线模块上电为配置模式
    2.将设备id存到flash中，这样更改设备id就不用再重新编译程序了
        通过调试串口修改
    3.将ip存到flash中
        通过调试串口修改
    4.完善无线串口通信机制
        frame_head frame_length data0 data1 ... crc frame_tail
    5.完善无线串口在线诊断
```