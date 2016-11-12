SmartIMU
========
[![GitHub version](https://img.shields.io/badge/version-v1.4-brightgreen.svg)](https://github.com/KitSprout/SmartIMU)
[![GitHub license](https://img.shields.io/badge/license-%20MIT%20%2F%20CC%20BY--SA%204.0-blue.svg)](https://github.com/KitSprout/SmartIMU/blob/master/LICENSE)
[![GitHub pcb library](https://img.shields.io/badge/pcb%20library-%20v2.12-yellow.svg)](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.12)


SmartIMU 是一個集成微控制器（STM32F412）、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤與氣壓計（MPU9250、LPS22HB）於一體的 10DOF 模組，原本設計的 SMT 版本改名為 [SmartIMU-Nano](https://github.com/KitSprout/SmartIMU-Nano)，這是一款整合 10DOF 感測器、FLASH(Quad-SPI)、燒錄器於一體的開發板，並預留 16pin 1.27mm 間距的擴充腳位，用來連接 BT、WIFI、OLED... 等不同需求的底板。

Hardware
========
* 控制器　 : STM32F412R 64Pin 100MHz DSP FPU
* 感測器　 : [MPU9250](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/) + [LPS22HB](http://www.st.com/en/mems-and-sensors/lps22hb.html)
* 記憶體　 : [W25Q128](http://www.winbond.com/hq/product/code-storage-flash-memory/serial-nor-flash/?__locale=en)
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.12](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.12) )

<img src="https://lh3.googleusercontent.com/s9HBhm7GyB9C3J3D8tBZ_l_-D4v2vez7xCFGqutOJ-oCAFO07tOo-m3nW81uAgWkpQxqNCAYbi4LopgGwTbtO1ED2h5ruqkYeNYvnRX5RTnO_izmU1MFDdLQR2Hhi86k_8P7mmo0bJnsvq16rgWPIeN3TWPkycpqyy1niHY6ZmfgYfxyiMmsEs0BBJsxEqS9oROSHnyCWG0FKIZ9JKuJwho9vtkZzaR4fJsjHGom6AIG3yrPQ7gTgjLBTQs42hSN0Ws3cs3P2jq5Sg151kF7wm1yBwrv7TU2IrkvtGc2SaIqbBpGt3vnSTUs9c7iqllAG0jGFkPG2QS1TJHBTHsXzahOu12iqPdmnpmslC180-wDmAPFuayt11v_vXl8IZUCVnhiq8vTk_dYxnjgENqKfCzKhl_ZxZkr-1KOMPMstDaI0NJsMBR7sfcNaxLUH9enYLawWT71r3KOZGj3DxSWRjt3qckF-RAsHvLHznBno0TPE10WIwvrwXjz0nYSbld2TQrfP0Xfgf7EFsSU_pYXoO3H-x8wQH0rGEE7UBVNebpq4WjmReRvG-6595ucTm8J7Jizz4cd2EIQMmWnZvW3nczSH0RFep01ok4IpVV2rRfEkzz6=w1065-h770-no"/>

View
========
<img src="https://lh3.googleusercontent.com/K6z3PWVtnvwSf8z2JNaz9UfAK4tOWMneHYoEdrbC76Tyua_JgqHH_AuzV2lg466fbNfNz_I_9WEJ58rxjCQhury47MdEoYA1nYP9kFBOJgs1kJUcp8iIXVuM0tTT3k_6hVB8ydSNGO1Y2FD1jGzxKtK8zes5XaRs5uhsOmp4i2klrg52MWRnq2XNhTtdSH4Xbz9f2BY3uIkzsClAQrZA7hh7hByhJI4YNVADmf2utnXyzqYUu_WhLjAMhqPKmFbXLhCMW7_jY33qUvCZwTHUMaxN8aKleGfnpwUXG7oVXtcju3SeiqbxZqS5RM3gSGpuJu0Afh4flqspTATPOo6bKlCf19rgljXr4p6u3poyKtWQjan87Y_wBnaLF0B3rCityivNAPWcD662LTFxa0DOPRGFigAvd2kaA1E2l1BFF_zorfqRShbwziw2RD2b9u2OxTi_vS6jw4jGczujVI3psrqtGQyccDMnniQsMXHIZ2X3hhwkrfvGIF0cD36DDbdFErCRLwHuFYHV9_J7_iK40kqvZCU-3WfFxL8Rdl3JSP1oPd9cZPE40IdTlblpZmw2KGQv_qv03U98KAjZbyXHfWy9Ckr0qGrhDsoPbt4dcgfNOrg_=w1027-h770-no"/>

<br />
[more photo...](https://goo.gl/photos/EfpjMtSc4LyJ7H2Q7)

Schematic
========
<img src="https://lh3.googleusercontent.com/UXaOqNPgnoPvc2oBSGjZ-RLceH0uIZzTc-9xw6aptvXVqLhpaVLYN0bcWA_acjcdtcjLt5V73go73TgPfY0afOwrvllRabWnj2g4cbgg8m3AjqZ86fuuFVLe-T8g3evzUVje0dWnNTtc3jiD4eMLyAyJgVjrHOemRI9_p8wE5V0TFVNq7LwAPuCExvXUu2T6_N-GDE5xK-pT5HgKxHFglZxzISirrxqsa9uxIYQVQbafrrcyM9PsMVhPVJLrBTSTLIcLjmm51c-fj5oVBV6ZxBQrD9m26ui9JfonbW5ZGska1OiaAZJnsyrIkRMyvmOCkKXDcvDszbs9MYtPOzAzMgLv3lB9OezoHc40pgXOo5aShTwsJ9IJaxHqTqaAYWARS9c5m-njrGMAggIdYq6zNVTI9Vjiq6RXmN-uyfw5kYOqwCH2rWRw0PEJqHM2tDRE2Nss-PLelR0XrWpN88Wnsmq0TWyPi81IsfH4ql8CBxO6HYkTcQTkWiJcrr8RUywXfFqPoWZnzri1etGEZtiaQ2oD1mKrFDNFAdlXNpiUEc-s_x4UTVqgAvcknEDoUpo-y52w73xwaxL-YDYiPHQVJDXRhuL1U7qH0IzvYt3zUyfePCBO=w1210-h770-no"/>

