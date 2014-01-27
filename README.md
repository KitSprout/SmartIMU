[SmartIMU](https://github.com/Hom-Wang)
========
* Author  : [Hom](https://github.com/Hom-Wang)
* Version : v1.0 設計中...
* Update  : 2014/01/27

Description
========
SmartIMU 是一個集成微控制器 ( STM32F103T )、感測器 ( MPU-9250、MS5611 ) 於一體的模組，透過 UART/I2C、SPI、USB 來讀取計算出載體的角度、加速度、速度、位移、高度 ... 等等資訊。  
> 預計建立兩種模式：  
> 1. 透過 SPI 讀取 Sensor  
> 　 此方案不須使用到 MCU，可以比 模式2 更接近實時，同時 MCU 也可以做其他處理。  
> 2. 透過 MCU 讀取 Sensor  
> 　 此方案可以預先計算姿態來節省飛控負擔，同時會加載 IAP 功能，  
> 　 可以直接透過飛控板來更新程式碼，不需要再做燒錄的動作。  

License
========
* 硬體(Hardware)採用 [CC BY-SA 3.0 TW](http://creativecommons.org/licenses/by-sa/3.0/tw/deed.zh_TW) 方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/tw/"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> SmartIMU </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="https://plus.google.com/u/0/112822505513154783828/posts" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/tw/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 3.0 台灣 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F103T](http://www.st.com/web/catalog/mmc/FM141/SC1169/SS1031/LN1565/PF250484) 36Pin 72MHz DSP
* 感測器　 : [MPU-9250](http://www.invensense.com/mems/gyro/mpu9250.html) + [MS5611](http://www.meas-spec.com/product/pressure/MS5611-01BA03.aspx)
* 外接介面 : 1*UART/I2C、1*SPI、1*USB
* PCB 尺寸 : ?? * ??mm
* 設計軟體 [Altium Designer 13](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v?](https://github.com/OpenPCB/AltiumDesigner_PcbLibrary) )

Related Documents
========
* [Google Drive](https://drive.google.com/folderview?id=0BzL2wwAot6oPN05nMklQQW0zVHc&usp=drive_web)

Program
========

Update Records
========
* [Hackpad](https://hom.hackpad.com/SmartIMU-E9zzbSI7QyK)
