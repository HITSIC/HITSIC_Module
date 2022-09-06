# 移植说明

[TOC]

## 简介





## 前期准备

### 开发环境

- 模组库使用C语言编写，兼容C++。C语言标准为C11，推荐C17或更高；C++可以使用C++11，推荐使用C++17或更高。

- 开发环境必须具有全套C标准库，且能够使用大部分标准库函数。目前我们遇到的问题有：assert缺失、动态内存分配缺失。

### GCC 




### IAR EWARM





## 移植步骤

### 添加模组库

1. 向底层中添加模组库。使用`git submodule`将CMODULE_Module添加至工程目录下
2. 创建移植文件夹`d_your_device`。
3. 将CMODULE_Module/utilities文件夹及上述移植文件夹添加至工程，使其中的头文件能被正确包含，源文件能被正确编译。

### 必需组件

1. 在移植文件夹内创建`inc_your_device_lib.h`，在这个文件夹内包含所有用到的底层库（如果为了加快编译速度，亦可不使用该文件，而是在移植文件中仅包含必须的底层库头文件）。

2. 仿照现有移植，在移植文件夹内创建`cmodule_common.h`文件，并根据设备名称创建签名：

  ```c
  #ifndef D_YOUR_DEVICE_CMODULE_COMMON_H
  #define D_YOUR_DEVICE_CMODULE_COMMON_H
  
  #include "inc_your_device_lib.h"
  
  //Other Code ...
  
  #endif // ! D_YOUR_DEVICE_CMODULE_COMMON_H
  ```

3. 在`cmodule_common.h`中添加该平台支持的模组的启用宏。**注意：不要添加该平台无法支持的模组！**

4. 如果该平台是**非NXP平台**，还需在此处添加模组库所需的状态定义：

   ```c
   /*******************************************************************************
    * Return Value System
    ******************************************************************************/
   
   /*! @brief Construct a status code value from a group and code number. */
   #define CMODULE_MAKE_STATUS(group, code) ((((group)*100) + (code)))
   
   /*! @brief Status group numbers. */
   enum _status_groups
   {
       mStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
   
       mStatusGroup_FLASH = 1,                   /*!< Group number for FLASH status codes. */
       //...此处省略若干行...
       mStatusGroup_SDIOSLV = 151,                 /*!< Group number for SDIOSLV status codes. */
   
   };
   
   /*! \public
    * @brief Generic status return codes.
    */
   enum
   {
       mStatus_Success = CMODULE_MAKE_STATUS(mStatusGroup_Generic, 0),  /*!< Generic status for Success. */
       mStatus_Fail = CMODULE_MAKE_STATUS(mStatusGroup_Generic, 1),      /*!< Generic status for Fail. */
       mStatus_ReadOnly = CMODULE_MAKE_STATUS(mStatusGroup_Generic, 2),    /*!< Generic status for read only failure. */
       mStatus_OutOfRange = CMODULE_MAKE_STATUS(mStatusGroup_Generic, 3),   /*!< Generic status for out of range access. */
       mStatus_InvalidArgument = CMODULE_MAKE_STATUS(mStatusGroup_Generic, 4),   /*!< Generic status for invalid argument check. */
       mStatus_Timeout = CMODULE_MAKE_STATUS(mStatusGroup_Generic, 5),   /*!< Generic status for timeout. */
       mStatus_NoTransferInProgress = CMODULE_MAKE_STATUS(mStatusGroup_Generic, 6),   /*!< Generic status for no transfer in progress. */
   };
   
   
   /*! @brief Type used for all status and error return values. */
   typedef int32_t mstatus_t;
   ```

   其中mStatus_Group部分可以按需编写。

5. 下面的内容可按需编写。亦可按需添加`cmodule_common.c`。







### 可选组件

1. 创建一个现有的移植的副本。当前支持最全面的移植是 `d_mk66f18` ，推荐使用此移植开始。
2. 修改每个头文件的Include Guard。例如，KV10的`EXTINT`移植文件的Guard宏名为：`D_KV10Z7_SYS_EXTINT_PORT_H` 。
3. 禁用大部分模块，只保留你想要移植的模块开启。
4. 根据该模块的移植手册完成移植。
5. 测试并修复BUG。
6. 启用一个新模块，重复步骤3~6，直到移植完所有需要的模块。



### 外部组件





## 贡献



