[SmartIMU](https://github.com/Hom-Wang/SmartIMU)
========
* Author  : [Hom](http://about.me/Hom)
* Version : v1.0
* Update  : 2016/10/14

Description
========
SmartIMU 是一個集成微控制器（STM32F405）、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤與氣壓計（MPU9250、LPS25H）於一體的 10DOF 模組，
另外新增 DEV 版本，暫時採用 STM32F405，但預計後續改版會更改成 LQFP64 STM32F412 芯片，DEV 版本是為了方便開發 SmartIMU 設計的，集成燒錄器、FLASH 等功能。  

License
========
* 硬體(Hardware)採用 [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW)  方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> SmartIMU v1.0 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : STM32F405R 64Pin 168MHz DSP FPU
* 感測器　 : [MPU9250](http://www.invensense.com/mems/gyro/mpu9250.html) + [LPS25H](http://www.st.com/web/catalog/sense_power/FM89/SC1316/PF255230)
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.2](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.2) )

<img src="https://lh3.googleusercontent.com/PS6K8fqAXm_xBcWc_b7xoClNne4HpccdXXxs5nafgnEyrIxzCeDRUUshTNtQR8iuvN8L7cr-v8QnxSx7NLCZBpAJo2CqkUsY9A9jwXFb4AshrB4RifAjroEh4q4pXpxrxWa3Kev4-3ESvFyUinC-7xF2PZD_jC5uMzbl1k4UOI8nsvAAaLv4OCjsCYRxQlrud5Lqbi8aWWEnfcWZuq2leWRDTNZQv-ZylW2k4WP0JYxbRLKcjfMC5WuGHr9gg9HoQkL5FNXXj-MkKBsbALvsUMqG6xTg4o6GBtFZzo8cvxLQ1KWhWF2-UTBcjpTIWyT4wGrY6ZHPyhW7N3MWx3Y0VdMlqogmGjuHYk1dSGWWNqLycEW4zJ1T0K9BoMKmSVOsg9NqU2Qrqh2M6tYfokCYbDyO3onK4JzwAJ_7VPGuvCOKWYwt_Am_pT_KTfltMd9mA8li5WaV3er6ZSvzuMKnAOumJBNBaOZJOkeWYjxm-UY-Y3_YFxgH-_A9_SnVXSMcJXpHm3teswJPUvBhxJTjatahs72h2I83OMzfjJ70Ay453A3n0JqczXouUTuMOdmV33cggnbMV4_n7x-UFgRw1ThN71ylgKhv2ec0uj-lvS_ZWHuZ=w848-h950-no"/>

Related Documents
========
* [Google Drive](https://goo.gl/lxlWVo)

View
========
<img src="https://lh3.googleusercontent.com/FvuTsuiwMtdf5rc7u7woPQBALcFJKRl8ZFQls50RI0xK3Ti6AvWbKV7irKp73L14JkFyWlINtSFJvhcJVGLsC0IHkmMoGzODzb6nVZizU5TDylNx7IdqLX6wruOqhyYySXOSRbr_OnGvxfcRs1QrbU3U4YzJqeRInXOEuLTug_A9DsMzmIkZSZA3gqZ1RdFFvOUQVWrODC_5qMWlSmQWjJh5OKmYW0IswbO0WkjVBwnwmBoBRPcMDADev7OiLfKZiuK-EGPT2i02wsY0bLeu7whp2BTm6M5TZuaAlraSwqULW4JljBCsG8Gu1V027tbvYHBNrOT14X05_yjaAn0XW_Xa80UrFCuNDlIW1OhthSCrY34g5CrgvD9zr3FsCl7VxODilMEivFHHa-k8sB3spJvvRjaLKPdr83mqkX1kSBnwcVFRGaPow4ElbEhTKTJGtDUSy64Vjx2nqMYRuHfzHmo8lz2pH_TXcqpb2rWQQsFs9LZtJ2DGkkNzRY9WQ0cp1YpYNykeJ_1Bzyq2sCui0rFv-QwBJh5DvVv5p6WWnTmMc1VQ4pYD6bbIZIFuJ8ZXN5xzlAicTczYHZeY8Iv2GygS99xAN2b1eDmkW-Gl66vPZKQc=w1267-h950-no"/>

<br />
[more photo...](https://goo.gl/photos/EfpjMtSc4LyJ7H2Q7)

Schematic
========
<img src="https://lh3.googleusercontent.com/sCu2NZ7Dq0rMnvh5DKHCjmcGaO5s01gMnd7dKAEeAm4zj17Bzdd8ytYA7Qt2lxfXb3Z7gz6xsztcc2rkEeGpzgmHJtxLIFAcRPFtwQJHJiaKgB9KMFb6pUxsbwj8P-g1NBSgtmdiOmIUZjK4fGkCn0sEL-Ulw9KB-LIhukZReNGc1e9jlN26nVyT00tXpL13-jMgtIeZ5GpGPjbbsCGdQwmtRBS-r-afkub4lbof4M3KJs0kIeI0UQkEbn2ed9-wmHwmjXCfh7rkE7OXr1ptTfR-9Vs8BkCz4LANcOG8kkBiCQKa4vinbjy1tcxfrpo0Bau6WIebSH07HF8xxF6APxuG8R62Fgccsg1IwA9_DJLhvn4iRjcGkAauB2F_QUvqlPtOwKB7x_SB8O2eN-NRZzG2iJwQEyAoANDGbN9mwusuNI1OAuIqQ6aMPHb2Jq4KQPv8VTtcT3fFz0-NIxPS2uueo94ecPJ3_EHkS4gd-Vr7sHkUowpSZ_k_TKqk0k5kyE5I_6EAVeNTCcgSPFMxD1wOUEpIot_X5eXvACVsvh7vX-VfE9XEKKvEfiT58zS_wK6qECRMaWwWWONm8sZ1S01p33kADkAfik30MooPdv7DbGPq=w1425-h950-no"/>

