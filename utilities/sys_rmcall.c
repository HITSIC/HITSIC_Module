#include <sys_rmcall.h>

/*!
 * @addtogroup rmcall
 * @{
 */

#if defined(HITSIC_USE_RMCALL) && (HITSIC_USE_RMCALL > 0)

#define SYSLOG_TAG "RMCALL"
#define SYSLOG_LVL RMCALL_SYSLOG_LVL
#include <inc_syslog.h>

/** Private Functions */

void RMCALL_TxStatusMachine(rmcall_t *_inst)
{
    switch(_inst->statusFlag & rmcall_statusFlag_txBusy)
    {
    case 0: // no tx
        return;
    case rmcall_statusFlag_txHead:
        // tx data here.
        SYSLOG_D("Tx head done, Tx data begin. size = %4.4d.", _inst->txHeaderBuffer.dataSize);
        if(0U != _inst->txHeaderBuffer.dataSize)
        {
            _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_txHead)) | rmcall_statusFlag_txData;
            _inst->teleport->xfer_tx(_inst->txDataBuffer, _inst->txHeaderBuffer.dataSize);
        }
        else
        {
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
            _inst->statusFlag  = _inst->statusFlag | rmcall_statusFlag_txBusy;
            SYSLOG_D("Tx head done. Tx tail begin.");
#else // RMCALL_TRAILER_CRC32
            _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_txBusy);
            SYSLOG_D("Tx head done. Tx done.");
#endif // RMCALL_TRAILER_CRC32
        }
        return;
    case rmcall_statusFlag_txData:
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
        // tx data finished. tx tail.
        _inst->statusFlag  = _inst->statusFlag | rmcall_statusFlag_txBusy;
        _inst->teleport->xfer_tx((void*)&_inst->txTailBuffer, sizeof(uint32_t));
        SYSLOG_D("Tx data done. Tx tail begin.");
#else // RMCALL_TRAILER_CRC32
        // tx data finished. go idle.
        SYSLOG_D("Tx data done. Tx done.");
        _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_txBusy);
#endif // RMCALL_TRAILER_CRC32
        return;

    case rmcall_statusFlag_txBusy:
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
        // tx data finished. go idle.
        SYSLOG_D("Tx tail done. Tx idle.");
        _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_txBusy);
#else // RMCALL_TRAILER_CRC32
        assert(0); // should never end up here.
#endif // RMCALL_TRAILER_CRC32
        return;
    default:
        assert(0); // should never end up here.
        return;
    }
}

void RMCALL_RxStatusMachine(rmcall_t *_inst)
{
    switch(_inst->statusFlag & rmcall_statusFlag_rxBusy)
    {
    case 0: // no rx
        break;
    case rmcall_statusFlag_rxHead:
        // rx head finished. rx data here.
        if(RMCALL_HEADER_MAGIC != _inst->rxHeaderBuffer.magic)
        {
            uint8_t const *const ptr_end = ((uint8_t*)&_inst->rxHeaderBuffer) + sizeof(rmcall_header_t);
            uint8_t const *ptr = ((uint8_t*)&_inst->rxHeaderBuffer) + 1U; // The first DWord is not match for certain.
            for(; ptr < ptr_end; ++ptr)
            {
                uint32_t const cmp_val = RMCALL_HEADER_MAGIC;
                uint32_t copy_size = (ptr_end - ptr);
                uint32_t cmp_size = copy_size < 4U ? copy_size : 4U;
                if(0U == memcmp((void const*)ptr, (void const*)&cmp_val, (size_t)cmp_size))
                {
                    // Header magic pattern is found. Copy valid bytes to the beginning, and wait for rest bytes of header.
                    memcpy((void*)&_inst->rxHeaderBuffer, (void const*)ptr, (size_t)copy_size);
                    _inst->teleport->xfer_rx(((uint8_t*)&_inst->rxHeaderBuffer) + copy_size, sizeof(rmcall_header_t) - copy_size);
                    SYSLOG_D("Rx head magic error, but found magic pattern at last %d bytes. Continue.", copy_size);
                    return;
                }
            }
            _inst->teleport->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
            SYSLOG_D("Rx head magic error. Expected 0x%8.8x, got 0x%8.8x.", RMCALL_HEADER_MAGIC, _inst->rxHeaderBuffer.magic);
            return;
        }

        SYSLOG_D("Rx head done. ID = 0x%4.4x, size = %4.4d.", _inst->rxHeaderBuffer.handleId, _inst->rxHeaderBuffer.dataSize);

        if(0U == _inst->rxHeaderBuffer.dataSize) // No data. go idle. wait for handle execute.
        {
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
            // rx head finished. no data. rx trailer.
            _inst->statusFlag  = _inst->statusFlag | rmcall_statusFlag_rxBusy;
            _inst->teleport->xfer_rx(&_inst->rxTailBuffer, sizeof(uint32_t));
#else // RMCALL_TRAILER_CRC32
            // rx head finished. no data. go idle.
            _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
            SYSLOG_D("No data to receive. Rx command done.");
#endif // RMCALL_TRAILER_CRC32
        }
        else //recv data.
        {
            if(_inst->rxHeaderBuffer.dataSize > RMCALL_PUBLIC_BUF_SIZE) 
            {
                // rx data cannot fit in buffer. discard data.
                _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
                _inst->statusFlag  = _inst->statusFlag | rmcall_statusFlag_rxDataDiscard;
                SYSLOG_E("Invalid rx data size. Data size %4.4d byte(s) maximum, but got %4.4d byte(s) to receive.", _inst->rxHeaderBuffer.dataSize, RMCALL_PUBLIC_BUF_SIZE);
                SYSLOG_D("Rx data abort.");
            }
            else
            {
                _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_rxBusy)) | rmcall_statusFlag_rxData;
                _inst->teleport->xfer_rx((void*)_inst->rxDataBuffer, _inst->rxHeaderBuffer.dataSize);
                SYSLOG_D("Rx data begin.");
            }
        }
        return;
        
    case rmcall_statusFlag_rxData:
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
            // rx data finished. rx trailer.
            _inst->statusFlag  = _inst->statusFlag | rmcall_statusFlag_rxBusy;
            _inst->teleport->xfer_rx(&_inst->rxTailBuffer, sizeof(uint32_t));
            SYSLOG_D("Rx data done. Rx tail begin.");
#else // RMCALL_TRAILER_CRC32
            // rx data finished. go idle.
            _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
            SYSLOG_D("Rx data done. Rx idle.");
#endif // RMCALL_TRAILER_CRC32
        return;

    case rmcall_statusFlag_rxBusy:
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
            // rx trailer finished. go idle.
            _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
            SYSLOG_D("Rx tail done. Rx idle.");
#else // RMCALL_TRAILER_CRC32
        assert(0); // should never end up here!
#endif // RMCALL_TRAILER_CRC32
        return;

    default:
        assert(0); // should never end up here.
        return;
    }
}

/** Public Functions */

status_t RMCALL_Init(rmcall_t *_inst, rmcall_config_t const * const _config)
{
    assert(_config->teleport);
    assert(_config->teleport->xfer_tx);
    assert(_config->teleport->xfer_rx);
    assert(_config->teleport->xferAbort_tx);
    assert(_config->teleport->xferAbort_rx);

    SYSLOG_I("Init Begin. v%d.%d.%d",HITSIC_VERSION_MAJOR(SYS_RMCALL_VERSION),HITSIC_VERSION_MINOR(SYS_RMCALL_VERSION), HITSIC_VERSION_PATCH(SYS_RMCALL_VERSION));

    _inst->teleport = _config->teleport;

    _inst->statusFlag = 0U;

    _inst->rxDataBuffer = malloc(RMCALL_PUBLIC_BUF_SIZE);
    if(NULL == _inst->rxDataBuffer)
    {
        //memory allocation error !
        assert(0);
    }

    rmcall_isrDict_init(_inst->isrDict);

    SYSLOG_I("Init Comlpete.");
    
    return kStatus_Success;
}

void RMCALL_DeInit(rmcall_t *_inst)
{
    _inst->teleport = NULL;

    _inst->statusFlag = 0U;
    
    free(_inst->rxDataBuffer);
    _inst->rxDataBuffer = NULL;

    rmcall_isrDict_clear(_inst->isrDict);
}

status_t RMCALL_HandleInsert(rmcall_t *_inst, rmcall_handle_t *_handle)
{
    assert(_inst);
    assert(_handle);
    
    if(_handle->handleId > 65533)
    {
        SYSLOG_W("Insert Fail. Handle ID Out of Range !");
        return kStatus_Fail; // 65534 & 65535 is used for OOR detection.
    }

    HAL_EnterCritical();
    rmcall_isrDict_set_at(_inst->isrDict, _handle->handleId, _handle);
    HAL_ExitCritical();

    return kStatus_Success;
}

status_t RMCALL_HandleRemove(rmcall_t *_inst, rmcall_handle_t *_handle)
{
    assert(_inst);
    assert(_handle);

    HAL_EnterCritical();
    rmcall_isrDict_erase(_inst->isrDict, _handle->handleId);
    HAL_ExitCritical();

    return kStatus_Success;
}

status_t RMCALL_CommandSend(rmcall_t *_inst, uint16_t _handleId, void *_data, uint16_t _dataSize)
{
    assert(_inst);

    status_t ret = 0U;

    if(0U != (_inst->statusFlag & rmcall_statusFlag_txBusy))
    {
        return kStatus_RMCALL_TxBusy;
    }

    _inst->txHeaderBuffer.magic = RMCALL_HEADER_MAGIC;
    _inst->txHeaderBuffer.handleId = _handleId;
    _inst->txHeaderBuffer.dataSize = _dataSize;
    _inst->txDataBuffer = _data;
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
    _inst->txTailBuffer = CRC32_Calculate ((void*)&_inst->txHeaderBuffer, sizeof(rmcall_header_t), 0xffffffffU);
    _inst->txTailBuffer = CRC32_Calculate(_inst->txDataBuffer, _inst->txHeaderBuffer.dataSize, _inst->txTailBuffer);
#endif // RMCALL_TRAILER_CRC32
    
    _inst->statusFlag |= rmcall_statusFlag_txHead;
    SYSLOG_I("Tx head. ID = 0x%4.4x, size = %4.4d.", _handleId, _dataSize);
    ret = _inst->teleport->xfer_tx((void *)&_inst->txHeaderBuffer, sizeof(rmcall_header_t));

    return ret;
}

status_t RMCALL_CommandRecvEnable(rmcall_t *_inst)
{
    assert(_inst);
    status_t ret = kStatus_Success;

    if(0U == (_inst->statusFlag & rmcall_statusFlag_rxBusy))
    {
        _inst->statusFlag |= rmcall_statusFlag_rxHead;
        ret = _inst->teleport->xfer_rx((void*)&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
        if(kStatus_Success == ret)
        {
            SYSLOG_I("Rx enabled. Rx command begin.");
        }
        else
        {
            SYSLOG_E("Rx enable failed. Transfer error.");
        }
        
    }
    
    return ret;
}

status_t RMCALL_CommandRecvDisable(rmcall_t *_inst)
{
    assert(_inst);
    //FIXME
    if(0U != (_inst->statusFlag & rmcall_statusFlag_rxBusy))
    {
        _inst->teleport->xferAbort_rx();
        _inst->statusFlag &= (~rmcall_statusFlag_rxBusy);
        SYSLOG_I("Rx disabled.");
        return kStatus_Success;
    }
    
    SYSLOG_W("Rx disable failed. Rx busy.");
    return kStatus_Fail;
}

void RMCALL_TxIsr(rmcall_t *_inst)
{
    RMCALL_TxStatusMachine(_inst);
}

void RMCALL_RxIsr(rmcall_t *_inst)
{
    RMCALL_RxStatusMachine(_inst);
        
    if(0U == (_inst->statusFlag & rmcall_statusFlag_rxBusy))
    {
        if(_inst->statusFlag & rmcall_statusFlag_rxDataDiscard)
        {
            // Data of last received command has been discard.
            // TODO: call default handle here, or resolve errors.
            // then clear flag and restart rx header
            _inst->statusFlag = _inst->statusFlag & (~rmcall_statusFlag_rxDataDiscard);
            RMCALL_CommandRecvEnable(_inst);
            return;
        }

#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
        uint32_t crcResult = CRC32_Calculate ((void*)&_inst->rxHeaderBuffer, sizeof(rmcall_header_t), 0xffffffffU);
        crcResult = CRC32_Calculate(_inst->rxDataBuffer, _inst->rxHeaderBuffer.dataSize, crcResult);
        if(crcResult != _inst->rxTailBuffer)
        {
            // TODO: call default handle here, or resolve errors.
            // restart rx header
            SYSLOG_W("Rx parity error. Recevied 0x%8.8x, expected 0x%8.8x.", _inst->rxTailBuffer, crcResult);
            SYSLOG_V("Header: 0x%8.8x 0x%4.4x 0x%4.4x", _inst->rxHeaderBuffer.magic, _inst->rxHeaderBuffer.handleId, _inst->rxHeaderBuffer.dataSize);
            SYSLOG_V("Data:");
            char *p = _inst->rxDataBuffer;
            for(int i = 0; i < _inst->rxHeaderBuffer.dataSize; ++i)
            {HITSIC_LOG_PRINTF("%2.2x ", *p); ++p;}
            HITSIC_LOG_PRINTF("\n");
            RMCALL_CommandRecvEnable(_inst);
            return;
        }
#endif // RMCALL_TRAILER_CRC32
        
        rmcall_handle_t **p_handle = rmcall_isrDict_get(_inst->isrDict, _inst->rxHeaderBuffer.handleId);
        if(NULL == p_handle)
        {
            // TODO: call default handle here, or resolve errors.
            SYSLOG_W("Rx handle ID 0x%4.4x not found.", _inst->rxHeaderBuffer.handleId);
        }
        else
        {
            SYSLOG_I("Execute handle 0x%4.4x.", _inst->rxHeaderBuffer.handleId);
            // run command
            ((*p_handle)->handler)(_inst->rxDataBuffer, _inst->rxHeaderBuffer.dataSize, (*p_handle)->userData);
        }
        // restart rx header
        RMCALL_CommandRecvEnable(_inst);
    }
}


/* @} */

#endif // HITSIC_USE_RMCALL
