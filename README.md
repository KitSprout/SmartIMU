SmartIMU
========
[![GitHub version](https://img.shields.io/badge/version-v1.4-brightgreen.svg)](https://github.com/KitSprout/SmartIMU)
[![GitHub old version](https://img.shields.io/badge/old%20version-%20v1.0-green.svg)](https://github.com/KitSprout/SmartIMU/tree/6410f97f9d84b54c73befec515bb6b5a17d4caeb)
[![GitHub license](https://img.shields.io/badge/license-%20MIT%20%2F%20CC%20BY--SA%204.0-blue.svg)](https://github.com/KitSprout/SmartIMU/blob/master/LICENSE)
[![GitHub pcb library](https://img.shields.io/badge/pcb%20library-%20v2.12-yellow.svg)](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.12)


SmartIMU 是一個集成微控制器（STM32F412）、3-Axis 加速度計、3-Axis 陀螺儀、3-Axis 電子羅盤與氣壓計（MPU9250、LPS22HB）於一體的 10DOF 模組，原本設計的 SMT 版本改名為 [SmartIMU-Nano](https://github.com/KitSprout/SmartIMU-Nano)，這是一款整合 10DOF 感測器、FLASH(Quad-SPI)、燒錄器於一體的開發板，並預留 16pin 1.27mm 間距的擴充腳位，用來連接 BT、WIFI、OLED... 等不同需求的底板。

Hardware
========
* 控制器　 : STM32F412R 64Pin 100MHz DSP FPU
* 感測器　 : [MPU9250](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/) + [LPS22HB](http://www.st.com/en/mems-and-sensors/lps22hb.html)
* 記憶體　 : [W25Q128](http://www.winbond.com/hq/product/code-storage-flash-memory/serial-nor-flash/?__locale=en)
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.12](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.12) )

<img src="https://lh3.googleusercontent.com/rmx8gYGqIcmq9l0NmgfNyMVL6fzyN_3PPl22pRVfn-9coB21jN5DL3DAHhVtA1zLKLsgnBWzN8c-IILjPYbqkxwdRK8EkIJm9dYot7h5gKFYb979M7yPsLKS3hV2HU_HCouspcvyP3o7GNx0IjCK2qydNyjL3km2iraVINAow5VC4bqC9TtRsPgC3RESiFzU6R0zI65iMnsthUziZvAECOSBKWnU4sADaiwWtsI9mJ-Da4rgG2VTdrWQh3yQc7--zgwjZ-uBmlFaP_pIF8zruR43DoUFLPKHdH6UEKfjjvceMmlVx13xQmJFnsKMS-5MPF67ufx2VQkhOWOILIS7z1xoaDcoyrSOEAdtBGR5hdKkaInbfP9F5xzQEv2542o2pylchnHDaVPDFsmbYlG4lwjVotuJhUfQg8SYmQrkP1wOuvuOdlV4w7AX-FIU6qQzXLS-Iu6PSFkD9tuJwqBU8KZjQyd8Ywo4O52nCjD1LZbJ_PB92KT8vBYWj8EMcr1W6Au5oFE0Xc0l0EjMFQpravDFJI6WP1Xf2VSE-mW-D61AxX-fHj6SN8c_jbFsqMd7diAg5qi88UF24uMamMPqU70Z5nAkQ8a8h9N1OK7md_OONkEq=w1065-h770-no"/>

View
========
<img src="https://lh3.googleusercontent.com/7bsYjuloV0wsuKK7KqoJoWQRzVvJJeDxiLj_EJdjkX4cotJEDQw3grdWjIQE2xoCSIobk-NndqhX7VPAVafULoK4JHCJukWrTU_2qAILku-P47KETi4439Zw3qHGAjnKsZLDm2lclXVwDaNwZOg7AbvyD4yt6ld1vLuWLV6ndwbXlPzybHGjl2sT33JcSiaupfgijYW8C4ZOxNJF6ZBsV5ku3Sz7KJkJnT4zr4nz0vvvZwewCRbdXH4DuL91nsZdxHhkjE4we1Q_8-BPovtrlR_m34c7BLC9_iRMK2NjQrpv5ZvcqvbeAJAFnyiwew36fyiCfE5TkBhPE8T2O49rcd3yGKTxVvmUZaKcFx50A7UzBghd3j6sFlkXFuAI3bZtaYzk5S-9nXcBxcQ7zOuFgVdGQlQEE7j0Fxe0IAt6kM85bV24Qq38TyFcNUr05CzqKUY710wQE5uNkkiqLiEAKtgAL9IEv7qV3kGlzBV8egTvoebU5cqX1_y_5z8CEefRZXITWt8pOfAqhTNmW9sT8DoaqWBiAV6B3aMKyfMXAW-mrfZqQNdoruiplAta4oabeDYenWi50He3cJ9RP2h-WG0F819TlRDyNq6HPuf3QBvwDVeH=w1027-h770-no"/>

<br />
[more photo...](https://goo.gl/photos/EfpjMtSc4LyJ7H2Q7)

Schematic
========
<img src="https://lh3.googleusercontent.com/5oUIY8wpbTkbntU-W3S1uvxHjL_qbzgvfNcw6qtpS7rRAT9TACgVvG_-Fqpc3YQG6xwSqHUGuF6gd3e2cNXq2ma42V_wQgts8fwvrQn2Ot_Qb3sgPrjHJD_b_adIj5BwkQz4bt7yjQ1rtVYDAB7un3UktSVbXzksQAQQdl15m6r0qIKljrpZXF2P-GLKRu1XkoPD02Giyd-cY8_aIkD8xc5g08GlUzARGvanvWx-xfwwGmSTtiMuJ3-dKHR2cvTAonp-5tuY7Yz46efJSKoKAo4nuqW5wiRR4w5aNrQCxt-52A0VnR5iQbOl3nXUPsezJSvBULFCh6lwPoMjTRP2b0cxbPN8VGQ_GtPl6OBHajF0KhjyDqng1UmBvHi0kMbd4Y0-BHvrO1P7YzP6qzLj9pxPUHeKG2A7ublQzF5mXRY4S1fFyVraBn3cf4oDm9QVRLqfdXH9raHujut55-KcMTO9RhV9s27zdVkST6BESshz12J3CiRfxHTwMKX7Uw9OdfRPu8OCGGbV3szo3lhQsrTVrsRurqIPN_bYplfVNuQ_gVmA_YRirYuVyp_C88xcBUCwXtw2BuUDktosSnlsH2hTNEEpswj7O6QA4H25i6cVEC6H=w1210-h770-no"/>

Get it
========
Ruten Shop http://goods.ruten.com.tw/item/show?21645325788922
