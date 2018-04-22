SmartIMU
========
[![GitHub version](https://img.shields.io/badge/version-v2.0-brightgreen.svg)](https://github.com/KitSprout/SmartIMU)
[![GitHub old version](https://img.shields.io/badge/old%20version-%20v1.4-green.svg)](https://github.com/KitSprout/SmartIMU/releases/tag/v1.4)
[![GitHub license](https://img.shields.io/badge/license-%20MIT%20%2F%20CC%20BY--SA%204.0-blue.svg)](https://github.com/KitSprout/SmartIMU/blob/master/LICENSE)
[![GitHub pcb library](https://img.shields.io/badge/pcb%20library-%20v3.4-yellow.svg)](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v3.4)

SmartIMU 是一個集成微控制器（STM32F411）、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤與氣壓計（ICM20948、LPS22HB）以及無線傳輸(NRF52810)於一體的 10DOF 模組，控制器透過 UART 連接無線傳輸芯片，可以將感測器資料無線傳輸至其他裝置觀察。板上除了有 LED、按鍵、電源管理以及 RF 功率放大外，亦拉出至多 25pin 帶 ADC、UART、SPI、I2C 等功能的可用 I/O，1.27mm 間距的擴充腳位上下皆可連接，方便擴充不同應用，像是小型四軸飛行器。

Hardware
========
* 控制器 : [STM32F411CE](http://www.st.com/en/microcontrollers/stm32f411ce.html) 48Pin 100MHz DSP FPU / [nRF52810](https://www.nordicsemi.com/eng/Products/nRF52810) 32Pin 64MHz DSP
* 感測器 : [ICM20948](https://www.invensense.com/products/motion-tracking/9-axis/icm-20948/) + [LPS22HB](http://www.st.com/content/st_com/en/products/mems-and-sensors/pressure-sensors/lps22hb.html)
* 設計軟體 [Altium Designer 17](http://www.altium.com/altium-designer/overview) ( PcbLib use AD [PcbLib v3.4](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v3.4) )

View
========
<img src="https://lh3.googleusercontent.com/yAvklEiWQo2CISJ34oGOp9XRkrRPKPwtQyJPFb2feDKzlPkvHNjn9nWBixDtjnqvGzAMpRPt6gZBmLNcfM1cPllx6sg7sPUtQX-MVfQdsdAtfuskPCePRW4Gfy4UBf-306VE4BnebFU66TzSY8XY5ityhBnxAjBi7OWcNYE90vwBudlF4KnIeA406b8K4mvqTIm7xU2fTGtZSHEvPHCqpAmNOkZiW1k2aG0zrEp-Ic5i5rlcebHM6HODUuv3z-aYoZQIIjpr1WoP0pqUdB0jJnD-7VMWnv-P0pUTpYCBOASoYLGMFODaV98gnTc_9m6L759aKsH-3KjK3ru47cCIlp5hm-nwrV8MgdoYIcKkZ1gjUkNQkGrdIpzqOZc_A_MIo06Squ1Me6frgCRPCAkxuxxTt3iV0DYXCf044_HBERyFo3Z8d5YXrmJ93sIdp0ck7R8lpcf3qes2pNu5mXXBfEVwYF30i0s3fS9Lzab4RLzu9ljelTRJwCMqbM5jYtAaAzoWxBntASsh02B1efynLXirZVrMvk84m8xVhcOcD6EZUd3ChlVDS10LcGCaO1Sz-KrV7DQgDGvNqGZl-SQVhpsuu148mLd7UvVdhqO81J0VZ-S4dFfZ9PLqxrwYL6g36iFmY1VE2GsBUGoGs6QcBWjiol3LDvfI=w2052-h1564-no"/>
<br />

[more photo ...](https://goo.gl/photos/EfpjMtSc4LyJ7H2Q7)
