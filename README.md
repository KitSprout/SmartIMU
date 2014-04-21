[SmartIMU](https://github.com/Hom-Wang)
========
* Author  : [Hom](https://github.com/Hom-Wang)
* Version : v1.2 送廠製作中 ... ( v1.0 MPU6500 部分沒有問題，測試 AK8963 中 ... ) 
* Update  : 2014/04/21

Description
========
SmartIMU 是一個集成微控制器 ( STM32F401C )、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤、氣壓計等感測器 ( MPU9250、MS5611 ) 於一體的模組，可以透過 UART/I2C、SPI、USB 來讀取計算出載體的角度、加速度、速度、位移、高度 ... 等等資訊，除了上述，SmartIMU 也另外引出了第二個 SPI 和 4CH PWM，可以獨立成一片飛控板，微型四軸飛行器 [QCopterNano](https://github.com/QCopter/QCopterNano) 即預計使用 SmartIMU 作為飛控。 
> 預計建立兩種模式：  
> 1. 透過 SPI 讀取 Sensor  
> 　 此方案不須使用到 MCU，可以比"模式2"更接近實時，同時 MCU 也可以做其他處理。  
> 2. 透過 MCU 讀取 Sensor  
> 　 此方案可以預先計算姿態來節省飛控負擔，同時會加載 IAP 功能，  
> 　 可以直接透過飛控板來更新程式碼，不需要再做燒錄的動作。  

License
========
* 硬體(Hardware)採用 [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW)  方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> SmartIMU v1.2 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F401C](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1810/PF258491) 48Pin 84MHz DSP FPU
* 感測器　 : [MPU9250](http://www.invensense.com/mems/gyro/mpu9250.html) + [MS5611](http://www.meas-spec.com/product/pressure/MS5611-01BA03.aspx)
* 其他　　 : 1*LED
* 外接介面 : 1*UART/I2C、2*SPI、1*USB、2*GPIO
* PCB 尺寸 : 12.95 * 15.49mm
* 設計軟體 [Altium Designer 14](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v0.11](https://github.com/OpenPCB/AltiumDesigner_PcbLibrary/releases/tag/v0.11) )

***** TestBoard *****
* PCB 尺寸 : 26.16 * 31.75mm
* 設計軟體 [Altium Designer 14](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v0.10](https://github.com/OpenPCB/AltiumDesigner_PcbLibrary/releases/tag/v0.10) )

<img src="https://lh3.googleusercontent.com/-utxuzgyjc8o/U1PQgXltkQI/AAAAAAAAHpw/5_EWkP11yPE/w1200-h618-no/System_v1.2.png" />

Software
========
TEST_SmartIMU_ADC  
TEST_SmartIMU_LED  
TEST_SmartIMU_MPU9250  
TEST_SmartIMU_PWM  
TEST_SmartIMU_Systick  
TEST_SmartIMU_UART  

Related Documents
========
* [Update Records - Hackpad](https://hom.hackpad.com/SmartIMU-E9zzbSI7QyK)
* [Datasheet & BOM - Google Drive](https://drive.google.com/folderview?id=0BzL2wwAot6oPN05nMklQQW0zVHc&usp=drive_web)

View
========

<br />
更多圖片 [Google+ albums](https://plus.google.com/photos/112822505513154783828/albums/5973678489492691265)

Config
========
<img src="https://lh4.googleusercontent.com/-HK_TAZm_NEY/U1PQfM7u5mI/AAAAAAAAHpQ/SJ-SuhdjOGc/w840-h843-no/SmartIMU_v1.2_Config_CHIP.png" />
<img src="https://lh6.googleusercontent.com/-95ublXhWd48/U1PQfwcGhPI/AAAAAAAAHp0/hx4adyIoptM/w1367-h843-no/SmartIMU_v1.2_Config_PIN.png" />
<img src="https://lh5.googleusercontent.com/-Kxun-eKL8j4/U1PQfB7SGrI/AAAAAAAAHpU/fu1MyTtCUUE/w1512-h843-no/SmartIMU_v1.2_Config_AF.png" />
<img src="https://lh6.googleusercontent.com/-PQ02iK9h6BI/U1PQfGdMllI/AAAAAAAAHpc/YpYp3_Tuee8/w1263-h843-no/SmartIMU_v1.2_Config_DMA.png" />

Schematic
========
<img src="https://lh4.googleusercontent.com/-bp71xaYUoaU/U0mPUlZmWEI/AAAAAAAAHX4/PclqwTD8XBg/w1518-h804-no/SmartIMU+v1.2_Sch.png" />
