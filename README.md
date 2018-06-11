SmartIMU
========
[![GitHub version](https://img.shields.io/badge/version-v2.0-brightgreen.svg)](https://github.com/KitSprout/SmartIMU)
[![GitHub old version](https://img.shields.io/badge/old%20version-%20v1.4-green.svg)](https://github.com/KitSprout/SmartIMU/releases/tag/v1.4)
[![GitHub license](https://img.shields.io/badge/license-%20MIT%20%2F%20CC%20BY--SA%204.0-blue.svg)](https://github.com/KitSprout/SmartIMU/blob/master/LICENSE)
[![GitHub pcb library](https://img.shields.io/badge/pcb%20library-%20v3.4-yellow.svg)](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v3.4)

SmartIMU 是一個集成微控制器（STM32F411）、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤與氣壓計（MPU9250、LPS22HB）以及無線傳輸（NRF52810）於一體的 10DOF 模組，控制器透過 UART 連接無線傳輸芯片，可以將感測器資料無線傳輸至其他裝置觀察。板上除了有 LED、按鍵、電源管理以及 RF 功率放大外，亦拉出至多 25pin 帶 ADC、UART、SPI、I2C 等功能的可用 I/O，1.27mm 間距的擴充腳位上下皆可連接，方便擴充不同應用，像是小型四軸飛行器。

Hardware
========
* 控制器 : [STM32F411CE](http://www.st.com/en/microcontrollers/stm32f411ce.html) 48Pin 100MHz DSP FPU / [nRF52810](https://www.nordicsemi.com/eng/Products/nRF52810) 32Pin 64MHz DSP
* 感測器 : [MPU9250](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/) + [LPS22HB](http://www.st.com/content/st_com/en/products/mems-and-sensors/pressure-sensors/lps22hb.html)
* 設計軟體 [Altium Designer 17](http://www.altium.com/altium-designer/overview) ( PcbLib use AD [PcbLib v3.5](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v3.5) )

View
========
<img src="https://lh3.googleusercontent.com/YtkdFV8B7B4PPP56pD-DJ3jshflbL-XpCipRkmdZWjd5KZ2cg5BxMuydJVUHmblZqxt91NkUKHjLOZrjVjbFqaTt6ykaCPQUgx_NSJsgLQWInibukLW9geDOEX_8Pfc2n4e6jBm3kowTZRkEJFYJ_BWLv2nJt58ZSiEL06OTN94nLUNYk11onPNXB2uHBZ_arsCWzX2XY4Ye-hNV_biZUXQySvqhY89XYxT7cmZDe0PSKklJH2Zc8te58TdCOwFp6FskcLgMV_Jxnd2y9ZrIo-Z5z9MekHay6uDPh3bY1bf_ZYvs0rQx-6cYK8Z3tUkYHk8-Do_Mdn7K5YVAOZZKxTiBvIIx8M_p-6N7vBU1rNFWgVXkPiO1YS04rznXG4VQp4E-8smSGwPLdnHK0-f3xcZ3ErbuebrXz_4f_5CJheGpwYB7-XVj6evX7BAO0KOhy2fWypo4EIxYCuHTU94zZ1hS8P8-3T8qGs2twLH9AtrxE1apEdPdsHk-ZyD72M8bOKh6K-4W0t8kr632u7xYjGrn6g5GZLA1UdPunMCUWPmDa7UZZTkFHTVgJwl1NCPPL8UCtaThLoAalB7OwFlIsDp3BTIqr859IRcvwZEdC6O8jsk8Hf6_ryX1JNigiCEEWopVOVvD26nGb108OR7g0pvTHpnzmROJ=w2086-h1564-no"/>
<br />

[more photo ...](https://goo.gl/photos/EfpjMtSc4LyJ7H2Q7)
