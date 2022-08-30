#include "textmenu_port.h"

/**
 * @name 事件任务接口
 * @ {
 */

/**
 * @brief 菜单事件服务前置定义。
 *
 * 该函数需要在事件处理线程中调用。每收到一次信号量，该线程激活一次，运行此函数。
 * 
 */
void MENU_EventService(void);

#ifdef __cplusplus
extern "C"
{
#endif

void TEXTMENU_SERVICE_IRQHandler(void)
{
    TEXTMENU_SERVICE_SEM_TAKE();
    MENU_EventService();
}

#ifdef __cplusplus
}
#endif

#include "sys_pitmgr.h"

extern pitmgr_t pitmgr_main;
extern pitmgr_handle_t menu_pitHandle;

/**
 * @brief 底层就绪函数。
 *
 * 调用该函数使能菜单定时触发器，并使能事件处理线程，使其可以接收信号量。
 * 
 */
void MENU_PORT_LowLevelResume(void)
{
    menu_pitHandle.pptFlag |= pitmgr_pptEnable;
    NVIC_EnableIRQ(TEXTMENU_SERVICE_IRQn);
}

/**
 * @brief 底层挂起函数。
 *
 * 调用该函数禁用菜单定时触发器，并禁用事件处理线程，使其无法接收信号量。
 * 
 */
void MENU_PORT_LowLevelSuspend(void)
{
    menu_pitHandle.pptFlag &= ~pitmgr_pptEnable;
    NVIC_DisableIRQ(TEXTMENU_SERVICE_IRQn);
}

/* @ } */

/**
 * @name 屏幕打印接口
 * @ {
 */
#include <drv_disp_ssd1327.h>
#include <lib_graphic.h>

#include "textmenu_strbuf.h"

/**
 * @brief 向显示设备输出画面。
 *
 * 关于字符缓存的格式，启用或禁用色盘（Palette）时有不同的定义。详见相关文档。
 * 
 * @param _buf 要输出的字符缓存指针。
 */
void MENU_PORT_DisplayOutput(menu_strBuf_t *_buf)
{
    extern disp_ssd1327_fb_t dispBuffer;
    DISP_SSD1327_FB_Clear(&dispBuffer);
    for (uint8_t i = 0; i < TEXTMENU_DISPLAY_STRBUF_ROW; ++i)
    {
        DISP_SSD1327_FB_Print0608_Print_Overlay(&dispBuffer, i << 3U, 1U, 0x0fU/*f_color*/, 0x00U/*b_color*/, (const char*)_buf->strbuf[i]);
    }
    DISP_SSD1327_BufferUpload((uint8_t*) &dispBuffer);
}

/* @ } */


/**
 *  @name 键值数据库
 *  @ {
 */
#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

#include "easyflash.h"

/**
 * @brief 通过键获取对应值的大小。
 * 
 * @param _key 想要读取的键。
 * @param _size 保存大小的指针。
 * @return mstatus_t 成功返回mstatus_Success，失败返回mstatus_Fail。
 */
mstatus_t MENU_PORT_KVDB_GetSize(char const *_key, uint32_t *_size)
{
    ef_get_env_blob(_key, NULL, 0, _size);
    if(0U == *_size)
    {
        return mstatus_Fail;
    }
    else
    {
        return mstatus_Success;
    }
}

/**
 * @brief 读取值。
 * 
 * @param _key 想要读取的键。
 * @param _data 存放读取数据的指针。
 * @param _size 提供的缓存区的大小。
 * @return mstatus_t 成功返回mstatus_Success，失败（键不存在或缓存区太小）返回mstatus_Fail。
 */
mstatus_t MENU_PORT_KVDB_ReadValue(char const *_key, void *_data , uint32_t _size)
{
    uint32_t dataLen = ef_get_env_blob(_key, _data, _size, NULL);
    if(0U == dataLen)
    {
        return mstatus_Fail;
    }
    else if (dataLen > _size)
    {
        return mstatus_Fail;
    }
    else
    {
        return mstatus_Success;
    }
}

/**
 * @brief 保存值。
 * 
 * @param _key 想要保存的键。
 * @param _data 存放保存数据的指针。
 * @param _size 保存数据的大小。
 * @return mstatus_t 成功返回mstatus_Success，失败返回mstatus_Fail。
 */
mstatus_t MENU_PORT_KVDB_SaveValue(char const *_key, void const *_data, uint32_t _size)
{
    if(0U != ef_set_env_blob(_key, _data, _size))
    {
        return mstatus_Fail;
    }
    else
    {
        return mstatus_Success;
    }
}

/**
 * @brief 删除键和对应值。
 * 
 * @param _key 想要删除的键。
 * @return mstatus_t 成功返回mstatus_Success，失败返回mstatus_Fail。
 */
mstatus_t MENU_PORT_KVDB_DeltValue(char const *_key)
{
    if(0U !=  ef_del_env(_key))
    {
        return mstatus_Fail;
    }
    else
    {
        return mstatus_Success;
    }
}

#endif // ! TEXTMENU_USE_KVDB

/* @ } */

/**
 *  @name 键值数据库
 *  @ {
 */
#if defined(TEXTMENU_USE_NVM) && (TEXTMENU_USE_NVM > 0)

/*!
 * @addtogroup menu_nvm
 * @{
 */
#define SYSLOG_TAG  ("MENU.NVM")
#define SYSLOG_LVL  (TEXTMENU_KVDB_LOG_LVL)
#include "inc_syslog.h"

#include "textmenu.h"

/**
 * ********** NVM存储变量定义 **********
 */

/**
 * @brief : 每个扇区包含的字节数
 */

/**
 * @brief : 全局存储 Global Storage
 */
uint32_t menu_nvm_glAddrOffset = TEXTMENU_NVM_GLOBAL_SECT_OFFSET * TEXTMENU_NVM_SECTOR_SIZE;/// 全局存储区地址偏移
;/// 全局存储区地址偏移
    
/**
 * @brief : 局部存储 Region Storage
 */


// 每个局部存储区占用的扇区数
uint32_t menu_nvm_rgSectOffset
[TEXTMENU_NVM_REGION_CNT];/// 三个局部存储区的扇区偏移

uint32_t menu_nvm_rgAddrOffset
[TEXTMENU_NVM_REGION_CNT];/// 三个局部存储区的地址偏移

/**
 * @brief : 菜单存储占用的总扇区数
 */
uint32_t menu_nvm_totalSectCnt;
/**
 * @brief : 每个菜单项保存时占用的字节数
 */
uint32_t menu_nvm_dataSize = 32u;

/**
 * @brief : 菜单项写入缓存。
 * 当改写第N个扇区时，menu_nvm_cachedSector = N, menu_nvm_cache分配4KB缓存
 * 并读入第N扇区的所有内容。此时能且仅能修改第N扇区的内容。对第N扇区内容的修改
 * 将缓存至上述内存。
 */
uint8_t *menu_nvm_cache = NULL;
uint32_t menu_nvm_cachedSector = 0;
/**
 * @brief : 菜单进行全局擦除/保存的次数，可用于估计Flash寿命
 */
uint32_t menu_nvm_eraseCnt = 0;

int32_t menu_currRegionNumAdj[3] = { 0, 0, TEXTMENU_NVM_REGION_CNT - 1 };

extern menu_list_t *menu_menuRoot;             ///< 根菜单指针。

const char menu_itemNameStr_RegnSel[] = {'R','e','g','n','S','e','l','(','0','-',('0' + TEXTMENU_NVM_REGION_CNT - 1),')','\0'};


/**
 * ********** NVM存储操作接口 **********
 */

mstatus_t MENU_NvmRead(uint32_t _addr, void *_buf, uint32_t _byteCnt)
{
    SYSLOG_D("Read addr = 0x%8.8x, Size = %4.4ld", _addr, _byteCnt);
    uint32_t result = TEXTMENU_NVM_AddressRead(_addr, _buf, _byteCnt);
    if (TEXTMENU_NVM_RETVAL_SUCCESS == result)
    {
        return mstatus_Success;
    }
    else
    {
        SYSLOG_E("Read failed. Addr = 0x%8.8x, size = %4.4ld, err = %ld", _addr, _byteCnt, result);
        return mstatus_Fail;
    }
}

bool MENU_NvmCacheable(uint32_t _addr)
{
    if (menu_nvm_cache == NULL ||
            _addr / flash_sectorSize == menu_nvm_cachedSector)
    {
        return true;
    }
    else
    {
        return false;
    }
}

mstatus_t MENU_NvmCacheSector(uint32_t _sect)
{
    if (menu_nvm_cache != NULL)
    {
        SYSLOG_E("Cached failed. Sector %2.2ld [-Existing]", _sect);
        return mstatus_Fail;
    }
    menu_nvm_cache = (uint8_t *)malloc(flash_sectorSize);
    if (menu_nvm_cache == NULL)
    {
        SYSLOG_E("Cached failed. Sector %2.2ld [-MemMalloc]", _sect);
        return mstatus_Fail;
    }
    if (TEXTMENU_NVM_RETVAL_SUCCESS !=
            TEXTMENU_NVM_SectorRead(menu_nvm_cachedSector,
                    (void *)menu_nvm_cache))
    {
        free(menu_nvm_cache);
        menu_nvm_cache = NULL;
        SYSLOG_E("Cached failed. Sector %2.2ld [-FlashRead]", _sect);
        return mstatus_Fail;
    }
    menu_nvm_cachedSector = _sect;
    SYSLOG_V("Cached sector %2.2ld", menu_nvm_cachedSector);
    return mstatus_Success;
}

mstatus_t MENU_NvmWriteCache(uint32_t _addr, void *_buf, uint32_t _byteCnt)
{
    if (menu_nvm_cache == NULL)
    {
        if (TEXTMENU_NVM_RETVAL_SUCCESS !=
                MENU_NvmCacheSector(_addr / TEXTMENU_NVM_SECTOR_SIZE))
        {
            SYSLOG_E("Write failed. Addr = 0x%8.8x, size = %4.4ld", _addr, _byteCnt);
            return mstatus_Fail;
        }
    }
    memcpy(menu_nvm_cache + _addr % flash_sectorSize, _buf, _byteCnt);
    SYSLOG_D("Write addr = 0x%8.8x, size = %4.4ld", _addr, _byteCnt);
    return mstatus_Success;
}

mstatus_t MENU_NvmUpdateCache(void)
{
    if (menu_nvm_cache == NULL)
    {
        SYSLOG_W("Update cache failed ! [-CacheEmpty]");
        return mstatus_Fail;
    }
    if (TEXTMENU_NVM_RETVAL_SUCCESS !=
            TEXTMENU_NVM_SectorWrite(menu_nvm_cachedSector, menu_nvm_cache))
    {
        SYSLOG_E("Update cache failed ! [-FlashWrite]");
        return mstatus_Fail;
    }
    free(menu_nvm_cache);
    menu_nvm_cache = NULL;
    SYSLOG_D("Update cached sector %2.2ld", menu_nvm_cachedSector);
    return mstatus_Success;
}

void MENU_Data_NvmSave(int32_t _region)
{
    if(_region < 0 || (uint32_t)_region >= TEXTMENU_NVM_REGION_CNT) { return; }
    ++menu_nvm_eraseCnt;
    SYSLOG_I("Data Save Begin");
    SYSLOG_I("Global Data");

    menu_iterator_t *iter = MENU_IteratorConstruct();

    do{
        menu_nvmData_t dataBuf;
        menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
        if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoSave))
        {
            MENU_ItemGetData(thisItem, &dataBuf);
            uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
            if (!MENU_NvmCacheable(realAddr))
            {
                MENU_NvmUpdateCache();
                assert(MENU_NvmCacheable(realAddr));
            }
            MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
        }
    }while(mstatus_Success == MENU_IteratorIncrease(iter));
    SYSLOG_I("Global Data End");

    if (menu_currRegionNumAdj[0] < 0 || menu_currRegionNumAdj[0] >= TEXTMENU_NVM_REGION_CNT)
    {
        SYSLOG_W("RegionNum illegal! Aborting.");
        MENU_IteratorDestruct(iter);
        return;
    }
    SYSLOG_I("Nvm Region %d Data", menu_currRegionNumAdj[0]);

    MENU_IteratorSetup(iter);

    do{
        menu_nvmData_t dataBuf;
        menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
        if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoSave))
        {
            MENU_ItemGetData(thisItem, &dataBuf);
            uint32_t realAddr = menu_nvm_rgAddrOffset[_region] + thisItem->saveAddr * sizeof(menu_nvmData_t);
            if (!MENU_NvmCacheable(realAddr))
            {
                MENU_NvmUpdateCache();
                assert(MENU_NvmCacheable(realAddr));
            }
            MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
        }
    }while(mstatus_Success == MENU_IteratorIncrease(iter));

    MENU_NvmUpdateCache();
    SYSLOG_I("Region %d Data End.", menu_currRegionNumAdj[0]);
    MENU_IteratorDestruct(iter);
    SYSLOG_I("Save Complete");
}

void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op)
{
    MENU_Data_NvmSave(menu_currRegionNumAdj[0]);
    *_op = 0;
}

void MENU_Data_NvmRead(int32_t _region)
{
    static_assert(sizeof(menu_nvmData_t) == 32, "sizeof menu_nvmData_t error !");

    if(_region < 0 || (uint32_t)_region >= TEXTMENU_NVM_REGION_CNT) { return; }
    SYSLOG_I("Read Begin");
    SYSLOG_I("Global Data");

    menu_iterator_t *iter = MENU_IteratorConstruct();

    do{
        menu_nvmData_t dataBuf;
        menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
        if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoLoad))
        {
            uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
            MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
            SYSLOG_D("Get Flash. menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
            MENU_ItemSetData(thisItem, &dataBuf);
        }
    }while(mstatus_Success == MENU_IteratorIncrease(iter));

    SYSLOG_I("Global Data End.");
    if (menu_currRegionNumAdj[0] < 0 || menu_currRegionNumAdj[0] >= TEXTMENU_NVM_REGION_CNT)
    {
        SYSLOG_W("RegionNum illegal! Aborting");
        MENU_IteratorDestruct(iter);
        return;
    }
    SYSLOG_I("Region %d Data.", menu_currRegionNumAdj[0]);

    MENU_IteratorSetup(iter);

    do{
        menu_nvmData_t dataBuf;
        menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
        if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoLoad))
        {
            uint32_t realAddr = menu_nvm_rgAddrOffset[_region] + thisItem->saveAddr * sizeof(menu_nvmData_t);
            MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
            SYSLOG_D("Get Flash. menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
            MENU_ItemSetData(thisItem, &dataBuf);
        }
    }while(mstatus_Success == MENU_IteratorIncrease(iter));

    SYSLOG_I("Region %d Data End", menu_currRegionNumAdj[0]);
    MENU_IteratorDestruct(iter);
    SYSLOG_I("Read complete");
}

void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op)
{
    MENU_Data_NvmRead(menu_currRegionNumAdj[0]);
    *_op = 0;
}

void MENU_Data_NvmSaveRegionConfig(void)
{
    SYSLOG_I("Saving region config ...");
    menu_nvmData_t dataBuf;
    menu_itemIfce_t *thisItem = MENU_DirGetItem(MENU_DirGetList("/MenuManager"), menu_itemNameStr_RegnSel);
    MENU_ItemGetData(thisItem, &dataBuf);
    uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
    if (!MENU_NvmCacheable(realAddr))
    {
        MENU_NvmUpdateCache();
        assert(MENU_NvmCacheable(realAddr));
    }
    MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
    MENU_NvmUpdateCache();
    SYSLOG_I("Save region config complete");
}
void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op)
{
    MENU_Data_NvmSaveRegionConfig();
    *_op = 0;
}

void MENU_Data_NvmReadRegionConfig(void)
{
    SYSLOG_I("Reading region config ...");
    menu_nvmData_t dataBuf;
    menu_itemIfce_t *thisItem = MENU_DirGetItem(MENU_DirGetList("/MenuManager"), menu_itemNameStr_RegnSel);
    uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
    MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
    MENU_ItemSetData(thisItem, &dataBuf);
    SYSLOG_I("Read region config complete");
}

void MENU_Data_NvmReadRegionConfig_Boxed(menu_keyOp_t *const _op)
{
    MENU_Data_NvmReadRegionConfig();
    *_op = 0;
}

void MENU_NVM_MenuDataSetup(menu_list_t *_list)
{

}

#endif // ! TEXTMENU_USE_NVM

/* @ } */

