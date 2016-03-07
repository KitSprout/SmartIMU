[SmartIMU](https://github.com/Hom-Wang/SmartIMU)
========
* Author  : [Hom](http://about.me/Hom)
* Version : v2.0
* Update  : 2016/03/08

Description
========
SmartIMU 是一個集成微控制器（STM32F411C）、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤與氣壓計（MPU9250、LPS25H）於一體的 10DOF 模組，可以直接透過 SPI/I2C 來獲取原始感測器資料，也可以透過另外的 SPI、I2C、UART 或是 USB 來讀取微控制器處理好的資訊，像是載具的角度、加速度、速度、位移、高度 ... 等等，除了上述功能外，也預留的了十幾個 I/O 與其對應功能來做擴充的應用，[QCopterNano](https://github.com/QCopter/QCopterNano) 即使用 SmartIMU 作為飛控。
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
* 其他　　 : 1xLED
* 外接介面 : 3 x UART、2 x SPI、1 x I2C、1 x USB、2 x ADC、...
* PCB 尺寸 : 12.70 x 15.24 mm
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.0](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.0) )

<img src="https://lh3.googleusercontent.com/licxJsxRTuTLnPZZlofxS0ZMqrvBqYvuA-FqHDHxf8Oq6VUwYCk2uyAkr22z7EwHdeuyAnASocn5vM7uItCAZi5S3wWhRUFsGZMYnZrQiewkpgBc7S_-pd6NFrvHHTzyzLAEdI57nBOKRzxo-sV7nLZvLuy5dDp8kWAx5G9Iylb2brZFcyH3f5IcD4Y78eah89jBiIQfb6Omhc_-aSjhUXzB2hvIwAFj3thQ3bZsQW-9Jiy1rNDQKSnmwmY3IBmCTqiU_VATd6uq-Z4RW1W17xR88L22QXLcPiyQsKeTe5eQBlxzujmmOIpny1PlDelgeG6PQ9elcFpqGlSPiKmEmMl5QQYrtfquhti4lXoPWiavtQkvg3ZlF9YxY6RAf3dHQNhOEr8gcYc7m6tczmdy5lTY0F6PgDAZXVVxbnNWXq6N-QVrAEKPsewGP4mg17A-MoHurVD5PyCvI8l-MXWh2_WvgUktvqanB5jq69m_uUjDts7o4UIsXKphPlFTSpr7Q7Sj4BprPOsBF89t6-ZJbq2Vu2WmZsCb6usBPXLMV2E5HF-tgOAre3XMwK7Yb3Rsf637=w820-h640-no"/>

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
<img src="https://lh3.googleusercontent.com/KXd2Y2Ptsf-PeH4lp2HCvZcebQV8Frodng1kN31TEERZ4A9GtYcO_6lQ6U-5C1ykVlfk5uw7Ohzipect9kMQDbHPYUZsgy7wxOBs34mEhkiBOvrTfbf0daDnmokohrkQde7yNl6_4HiePosvKPpWIkaL3WYN4upAiJ3WQldcl7o9gix6jXPbsk0XH4Po0l3CMOd7_cBHKCyHmbtmQgzPqg_0Z0VBEyM7vU6JbW8NRoSsELZ04CGhMnQ8W1UxdY0HpQ4i144YhPwnXoun9sEB6bSyvVOyfAVjkBMwtmSg6tKQthUOJlse7Wlr3bey8H1frH53UuiIRdGHxcXjS6i7ECXDxtTjtx0M_Ep3J0HnoD8YVfkt8pSWaWi0t0yiBxTRrd5_skJtTQMrrGd6n7mtok50kZ4ocYhZmez0WrcNg9wq1szxZw5nzUq197u-ua_BaErFCm0aUtWcpSxPn8egzY_0B5vJvzM5PE0c_M7HemDsKXEK1AE5Six2PmP7lj2B2qgrjpI4-jXBf7qB5Kb7-yR9XjDVjREVY6TEYIwHuDU-hM6BhA6YNpVhtWOTh3Pj518r=w1034-h775-no"/>

<br />
[more photo...](https://goo.gl/photos/fu2XTdUS5BwgrGjs7)

Config
========
<img src="https://lh3.googleusercontent.com/RAMiYRZOy7m7Q0N_PZFFitTtqaHwYe5qoBMcVmYCqb3L-loH2jtSlmJA4qScpVQKGNPhIfHEF2yB6P9qxlPrWauWn7GRzqYt7qk4pjLcXcVKB-S1AThwhuPk5dJ2xNg_YefzMYc_ifM0d8ro17nZMtYYocnfrmt7YRiJJnDw1LRM8un5VDwjLLx_wefNSfOuNHZZjrYY3H4UDSvpnBdyFgtcFG3azX3JefdaDegvEJ7MMe0gCFqlKdNDeGnALlkSn-znvb-u8yb4eFNdCsgg5ezHbeOCDoWPuyBtohuh2FcaURUcng1AQdF5SJ-F_F0MiytPY5lrn7Sra2gWxwz0zUGjpCaEUNniH3qfH4UxMOiaVrw--kryyArDugbQOFhJHzmUg9xGx1PY-K_XYlsDEdlLhUZjovEuRcReGG4sBPpgATewOkIEwLOfKt_pDfRlGQdZSUTqomt3dctXFInFKBhQXinjTUupyjtoBG-O-ZlvpehDrlyIoEZwmkH1h8izInWL-c5ePwdJiof_7fIZ4TEoJkE9PRs2GFNDhCeNGl3q7uWNmGIBDIEIi47kO03LXljb=w1360-h592-no"/>

Schematic
========
<img src="https://lh3.googleusercontent.com/qN5Hbv1okKa01bgwoN7vs8byPCMUJP4qZ-C6ZlSpeMgfWP3NFnccEpOz0BfWu_E-trDqinmqd7AofU-VKiO_yuaKxpM8sPxZr0_oSe618ZuxZB6KqozDLCrNBnbGZFf08cG1f9twkgjlIK2iNv_rZo6sYObJqewSwO1EjStzv5hLqjaqm9E3we8KCusbbAup9aSjvQ5WTDM-D8fC7tnQXKFzJKNFQ03ugOIi-UFxm3BtxVVBoTN3lnX1XWGTcbtvTa3zBTebm0CM11JZGIrAPJ9oozUBDiynvIToM2KfedjGJq5_QH0xvEXVJ5AaXYtj6RpoJye0PIB8giiego2QG-IECcpV4vsNBotHkjXjJvb5PFMyfoxpTbL6WAPPHMHkum3zkb4avw_ne4wZwaCqUVY02fBYy7cGVjizlEi11xCf_peOGnTsS-eUHVbn4YIBrPwo9xAGqE652WlcCPbbkkGh_t3FjBStrxRRFwIKNgJ3J4XfTFUntdU824YO8KHq5JFyEkV1fNttTrjvJgJTdD3PKjFqK03KRs1AvNJVZOP3N1tKWNsPKytAL4dwLJ_OI2T3=w2643-h2641-no" />
