[SmartIMU](https://github.com/Hom-Wang/SmartIMU)
========
* Author  : [Hom](http://about.me/Hom)
* Version : v2.0 (update...), [v1.2(Old)](https://github.com/Hom-Wang/SmartIMU/tree/0b4853c32ce107648c82dfed67a566d61b5bb8b1)
* Update  : 2015/11/07

Description
========
SmartIMU 是一個集成微控制器 ( STM32F441C )、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤、氣壓計等感測器 ( MPU9250、LPS25H ) 於一體的模組，可以透過 UART/I2C、SPI、USB 來讀取計算出載體的角度、加速度、速度、位移、高度 ... 等等資訊，除了上述，SmartIMU 也另外引出了第二個 SPI 和 4CH PWM，可以獨立成一片飛控板，微型四軸飛行器 [QCopterNano](https://github.com/QCopter/QCopterNano) 即使用 SmartIMU 作為飛控。 
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
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> SmartIMU v2.0 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F411C](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1877/PF260148) 48Pin 100MHz DSP FPU
* 感測器　 : [MPU9250](http://www.invensense.com/mems/gyro/mpu9250.html) + [LPS25H](http://www.st.com/web/catalog/sense_power/FM89/SC1316/PF255230)
* 其他　　 : 1*LED
* 外接介面 : 1*UART/I2C、2*SPI、1*USB、2*GPIO
* PCB 尺寸 : 12.70 * 15.24mm
* 設計軟體 [Altium Designer 15](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib ?]() )

SmartIMU v2.0 預計修改：  
>1. 修改部分腳位以兼容 MPU65x0。
>2. 修改部分元件 footprint 以方便焊接。
>3. 將氣壓計與慣性元件共用 SPI。
>4. 將 STM32F401 更換成 STM32F411 增加處理效能。
>5. 將 MS5611 更換成 LPS25HB。
>6. 修正 VCAP 電路問題。
>7. 設計新的測試座，以方便測試半孔板。
>8. 分成兩種開發板，DIP 與 SMD 兩種，方便麵包板測試與應用。

**** TestBoard  
* PCB 尺寸 : 26.16 * 31.75mm
* 設計軟體 [Altium Designer 14](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v0.12](https://github.com/OpenPCB/AltiumDesigner_PcbLibrary/releases/tag/v0.12) )

<img src="https://lh3.googleusercontent.com/-utxuzgyjc8o/U1PQgXltkQI/AAAAAAAAHpw/5_EWkP11yPE/w1200-h618-no/System_v1.2.png" />

**** SmartIMU-E  
* PCB 尺寸 : 
* 設計軟體 [Altium Designer 15](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib ?]() )


Software
========
* [SmartIMU_ModuleHAL_MPU6500](https://github.com/Hom-Wang/SmartIMU/tree/master/Software/SmartIMU_ModuleHAL_MPU6500)
* [SmartIMU_ModuleHAL_MPU9250](https://github.com/Hom-Wang/SmartIMU/tree/master/Software/SmartIMU_ModuleHAL_MPU9250)
* [SmartIMU_PeripheralHAL_GPIO](https://github.com/Hom-Wang/SmartIMU/tree/master/Software/SmartIMU_PeripheralHAL_GPIO)
* [SmartIMU_PeripheralHAL_UART](https://github.com/Hom-Wang/SmartIMU/tree/master/Software/SmartIMU_PeripheralHAL_UART)

Related Documents
========
* [Google Drive](https://goo.gl/lxlWVo)

View
========
<img src="https://lh4.googleusercontent.com/-D_xvblRS01U/U2cgfyH5foI/AAAAAAAAIOA/RJmb9rC6jjs/s1600/DSC_2175.jpg" />
<img src="https://lh3.googleusercontent.com/-IDrVWKv29KM/U2cge4Dcw-I/AAAAAAAAIOI/Mdec4T33aDY/s1600/DSC_2152.jpg" />
<img src="https://lh5.googleusercontent.com/-STk0VoAsyt8/U2cgfYZAiFI/AAAAAAAAIN4/LG56PU2AU6Y/s1600/DSC_2162.jpg" />
<img src="https://lh4.googleusercontent.com/-ENO7XYzZRIc/U2cgeEFghGI/AAAAAAAAIOo/irh-smYJEDI/s1600/DSC_2142.jpg"/>

<br />
更多圖片 [Google+ albums](https://plus.google.com/photos/112822505513154783828/albums/5973678489492691265)

Config
========

Schematic
========
