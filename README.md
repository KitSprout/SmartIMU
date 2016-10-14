[SmartIMU](https://github.com/Hom-Wang/SmartIMU)
========
* Author  : [Hom](http://about.me/Hom)
* Version : v2.0, dev(1.0)
* Update  : 2016/10/14

Description
========
SmartIMU 是一個集成微控制器（STM32F411C）、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤與氣壓計（MPU9250、LPS25H）於一體的 10DOF 模組，可以直接透過 SPI/I2C 來獲取原始感測器資料，也可以透過另外的 SPI、I2C、UART 或是 USB 來讀取微控制器處理好的資訊，像是載具的角度、加速度、速度、位移、高度 ... 等等，除了上述功能外，也預留的了十幾個 I/O 與其對應功能來做擴充的應用，[QCopterNano](https://github.com/QCopter/QCopterNano) 即使用 SmartIMU 作為飛控。
> 預計建立兩種模式：  
> 1. 透過 SPI 讀取 Sensor  
> 　 此方案不須使用到 MCU，可以比"模式2"更接近實時，同時 MCU 也可以做其他處理。  
> 2. 透過 MCU 讀取 Sensor  
> 　 此方案可以預先計算姿態來節省飛控負擔，同時會加載 IAP 功能，  
> 　 可以直接透過飛控板來更新程式碼，不需要再做燒錄的動作。  

另外新增 DEV 版本，暫時採用 STM32F405，但預計後續改版會更改成 LQFP64 STM32F412 芯片，DEV 版本是為了方便開發 SmartIMU 設計的，集成燒錄器、FLASH 等功能。  

License
========
* 硬體(Hardware)採用 [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW)  方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> SmartIMU v2.0 和 SmartIMU-Dev v1.0 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F411C](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1877/PF260148) 48Pin 100MHz DSP FPU
* 感測器　 : [MPU9250](http://www.invensense.com/mems/gyro/mpu9250.html) + [LPS25H](http://www.st.com/web/catalog/sense_power/FM89/SC1316/PF255230)
* 其他　　 : 1 x LED
* 外接介面 : 3 x UART、2 x SPI、1 x I2C、1 x USB、2 x ADC、...
* PCB 尺寸 : 12.70 x 15.24 mm
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.2](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.2) )

<img src="https://lh3.googleusercontent.com/licxJsxRTuTLnPZZlofxS0ZMqrvBqYvuA-FqHDHxf8Oq6VUwYCk2uyAkr22z7EwHdeuyAnASocn5vM7uItCAZi5S3wWhRUFsGZMYnZrQiewkpgBc7S_-pd6NFrvHHTzyzLAEdI57nBOKRzxo-sV7nLZvLuy5dDp8kWAx5G9Iylb2brZFcyH3f5IcD4Y78eah89jBiIQfb6Omhc_-aSjhUXzB2hvIwAFj3thQ3bZsQW-9Jiy1rNDQKSnmwmY3IBmCTqiU_VATd6uq-Z4RW1W17xR88L22QXLcPiyQsKeTe5eQBlxzujmmOIpny1PlDelgeG6PQ9elcFpqGlSPiKmEmMl5QQYrtfquhti4lXoPWiavtQkvg3ZlF9YxY6RAf3dHQNhOEr8gcYc7m6tczmdy5lTY0F6PgDAZXVVxbnNWXq6N-QVrAEKPsewGP4mg17A-MoHurVD5PyCvI8l-MXWh2_WvgUktvqanB5jq69m_uUjDts7o4UIsXKphPlFTSpr7Q7Sj4BprPOsBF89t6-ZJbq2Vu2WmZsCb6usBPXLMV2E5HF-tgOAre3XMwK7Yb3Rsf637=w820-h640-no"/>
<img src="https://lh3.googleusercontent.com/PS6K8fqAXm_xBcWc_b7xoClNne4HpccdXXxs5nafgnEyrIxzCeDRUUshTNtQR8iuvN8L7cr-v8QnxSx7NLCZBpAJo2CqkUsY9A9jwXFb4AshrB4RifAjroEh4q4pXpxrxWa3Kev4-3ESvFyUinC-7xF2PZD_jC5uMzbl1k4UOI8nsvAAaLv4OCjsCYRxQlrud5Lqbi8aWWEnfcWZuq2leWRDTNZQv-ZylW2k4WP0JYxbRLKcjfMC5WuGHr9gg9HoQkL5FNXXj-MkKBsbALvsUMqG6xTg4o6GBtFZzo8cvxLQ1KWhWF2-UTBcjpTIWyT4wGrY6ZHPyhW7N3MWx3Y0VdMlqogmGjuHYk1dSGWWNqLycEW4zJ1T0K9BoMKmSVOsg9NqU2Qrqh2M6tYfokCYbDyO3onK4JzwAJ_7VPGuvCOKWYwt_Am_pT_KTfltMd9mA8li5WaV3er6ZSvzuMKnAOumJBNBaOZJOkeWYjxm-UY-Y3_YFxgH-_A9_SnVXSMcJXpHm3teswJPUvBhxJTjatahs72h2I83OMzfjJ70Ay453A3n0JqczXouUTuMOdmV33cggnbMV4_n7x-UFgRw1ThN71ylgKhv2ec0uj-lvS_ZWHuZ=w848-h950-no"/>

Related Documents
========
* [Google Drive](https://goo.gl/lxlWVo)

View
========
<img src="https://lh3.googleusercontent.com/uQ88Q_gPXKgzSeStGQ-kGKBkgspJCNxCLVJEgJLyI2bOD6iCXu7vgKVb-sB1rm_2UdyNVsLckJeZDEsUs09l9bM1T7Zjx0xA0bXVbK6fJ07o5EJalJ3M4-LBCQ2Tf1mD4B7ZNaQ0yV9a0zehk06_iopncFqL3-mh-cNHpyHlX10utQy0yLPy_zVD6qcMk5QrytOvkRjKa5MUPi3irwiDNKGREHcH69BT-ZlBgN4UvjMarEvlWiu4Royo-rjPEPUEqcOPabIp4_zVM3PxC_TZSKQPFxSJMcAV0L3ftk52WU3g4F9wgoZD2Cfkz7nH3jLn0K-Q4heF0RcTXSVi2dZcXA1VYM0dPkqaZtyksGftCJUcU6BgGJK-vyrPK4c09LbxyvBLRjyCGWWb85tcGKuqMe8yDdrPB42ekp7R9JZGvVdcle4UnaAbf0T48Sz2ZjNIfh284tPhNBFTsV2FCIuV5BigISUcS3hNgJB8uFOb0-2GI3rY_V6MOZsWU4-eOfP_G3v5GVEfQTJMOHrznM0yxXdxbcuY75bp8WiN-OBi9_IMcycR3903zQWbrBs_WgTz8y97=w1034-h775-no"/>
<img src="https://lh3.googleusercontent.com/FvuTsuiwMtdf5rc7u7woPQBALcFJKRl8ZFQls50RI0xK3Ti6AvWbKV7irKp73L14JkFyWlINtSFJvhcJVGLsC0IHkmMoGzODzb6nVZizU5TDylNx7IdqLX6wruOqhyYySXOSRbr_OnGvxfcRs1QrbU3U4YzJqeRInXOEuLTug_A9DsMzmIkZSZA3gqZ1RdFFvOUQVWrODC_5qMWlSmQWjJh5OKmYW0IswbO0WkjVBwnwmBoBRPcMDADev7OiLfKZiuK-EGPT2i02wsY0bLeu7whp2BTm6M5TZuaAlraSwqULW4JljBCsG8Gu1V027tbvYHBNrOT14X05_yjaAn0XW_Xa80UrFCuNDlIW1OhthSCrY34g5CrgvD9zr3FsCl7VxODilMEivFHHa-k8sB3spJvvRjaLKPdr83mqkX1kSBnwcVFRGaPow4ElbEhTKTJGtDUSy64Vjx2nqMYRuHfzHmo8lz2pH_TXcqpb2rWQQsFs9LZtJ2DGkkNzRY9WQ0cp1YpYNykeJ_1Bzyq2sCui0rFv-QwBJh5DvVv5p6WWnTmMc1VQ4pYD6bbIZIFuJ8ZXN5xzlAicTczYHZeY8Iv2GygS99xAN2b1eDmkW-Gl66vPZKQc=w1267-h950-no"/>

<br />
[more photo...](https://goo.gl/photos/EfpjMtSc4LyJ7H2Q7)

Schematic
========
<img src="https://lh3.googleusercontent.com/1TX4mns8esvXUkUJbkO3DLjduwXPzqh_njAdmFrEUgbrfqa7nyj9Sy0QI2acpUgoZbLQXtM-0Sg-42d3TveDYvtD9V5hi94oXnS3LmK9e4yxuyfCRUNkDjwTRYT-3UV74_Pw1bKtGCoS6ZWtkqL7B8y9W5Q__1qOv7JwCA0i5Puaei5sFX8xXa60_zHRgkYQp9YULGPPvXqfavGJ_Shqae2e8GcgMMPNMZJSkcy6yaxHn7W3n27Aa__LCizYgr-AxG23FfmeRz8HEaV06kElCx_hFpXm6wBXP8AV6N_9TLFUYi1suGTpRv4h5k5XfmEkawSn7h4NPLInlYB2R9954INxRTrD7gjeckKix6Sme8atlVImXP6emUbAG6ZumIQpI2eHjU3XFmJxOBJGhSYxQUCe-06deO61G_CMVOg3oAtf1XbWEoTys4t0L5a9TDWLhfQXxY-nKwUQh0dwg69cMDMnWzNhhU2HPK4bohOWnWLhxX9srhgcBJosUWdUfC6fYxtS4WKHvaq_29geIeuMTjG3O2WkvJ5Ij2vGe2EVg0myH5OJL55L3OiEJPrNHobq3T9z=w1552-h1550-no" />
<img src="https://lh3.googleusercontent.com/sCu2NZ7Dq0rMnvh5DKHCjmcGaO5s01gMnd7dKAEeAm4zj17Bzdd8ytYA7Qt2lxfXb3Z7gz6xsztcc2rkEeGpzgmHJtxLIFAcRPFtwQJHJiaKgB9KMFb6pUxsbwj8P-g1NBSgtmdiOmIUZjK4fGkCn0sEL-Ulw9KB-LIhukZReNGc1e9jlN26nVyT00tXpL13-jMgtIeZ5GpGPjbbsCGdQwmtRBS-r-afkub4lbof4M3KJs0kIeI0UQkEbn2ed9-wmHwmjXCfh7rkE7OXr1ptTfR-9Vs8BkCz4LANcOG8kkBiCQKa4vinbjy1tcxfrpo0Bau6WIebSH07HF8xxF6APxuG8R62Fgccsg1IwA9_DJLhvn4iRjcGkAauB2F_QUvqlPtOwKB7x_SB8O2eN-NRZzG2iJwQEyAoANDGbN9mwusuNI1OAuIqQ6aMPHb2Jq4KQPv8VTtcT3fFz0-NIxPS2uueo94ecPJ3_EHkS4gd-Vr7sHkUowpSZ_k_TKqk0k5kyE5I_6EAVeNTCcgSPFMxD1wOUEpIot_X5eXvACVsvh7vX-VfE9XEKKvEfiT58zS_wK6qECRMaWwWWONm8sZ1S01p33kADkAfik30MooPdv7DbGPq=w1425-h950-no"/>

