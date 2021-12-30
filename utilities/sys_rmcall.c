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
        break;
    case rmcall_statusFlag_txHead:
        // tx data here.
        SYSLOG_D("Tx head Done, Tx data. Size = %4.4d.", _inst->txHeaderBuffer.dataSize);
        if(0U != _inst->txHeaderBuffer.dataSize)
        {
            _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_txHead)) | rmcall_statusFlag_txData;
            _inst->teleport->xfer_tx(_inst->txDataBuffer, _inst->txHeaderBuffer.dataSize);
        }
        else
        {
            SYSLOG_D("No tx data. Tx done.");
            _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_txBusy);
        }
        break;
    case rmcall_statusFlag_txData:
        // tx finished. go idle.
        SYSLOG_D("Tx data done. Tx done.");
        _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_txBusy);
        break;
    default:
        assert(0); // should never end up here.
        break;
    }
}

void RMCALL_RxStatusMachine(rmcall_t *_inst)
{
    switch(_inst->statusFlag & rmcall_statusFlag_rxBusy)
    {
    case 0: // no rx
        break;
    case rmcall_statusFlag_rxHead:
        // rx data here.
        if(HITSIC_RMCALL_HEADER_MAGIC != _inst->rxHeaderBuffer.magic)
        {
            uint8_t const *const ptr_end = ((uint8_t*)&_inst->rxHeaderBuffer) + sizeof(rmcall_header_t);
            uint8_t const *ptr = ((uint8_t*)&_inst->rxHeaderBuffer) + 1U; // The first DWord is not match for certain.
            for(; ptr < ptr_end; ++ptr)
            {
                uint32_t const cmp_val = HITSIC_RMCALL_HEADER_MAGIC;
                uint32_t copy_size = (ptr_end - ptr);
                uint32_t cmp_size = copy_size < 4U ? copy_size : 4U;
                if(0U == memcmp((void const*)ptr, (void const*)&cmp_val, (size_t*)cmp_size))
                {
                    // Header magic pattern is found. Copy valid bytes to the beginning, and wait for rest bytes of header.
                    memcpy((void*)&_inst->rxHeaderBuffer, (void const*)ptr, (size_t)copy_size);
                    _inst->teleport->xfer_rx(((uint8_t*)&_inst->rxHeaderBuffer) + copy_size, sizeof(rmcall_header_t) - copy_size);
                    SYSLOG_I("Rx head magic error, but found magic pattern at last %d bytes. Continue.", copy_size);
                    break;
                }
            }
            _inst->teleport->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
            SYSLOG_W("Rx head magic error. Expected 0x%8.8x, got 0x%8.8x.", HITSIC_RMCALL_HEADER_MAGIC, _inst->rxHeaderBuffer.magic);
            break;
        }
        
        SYSLOG_D("Rx head done. ID = 0x%4.4x, size = %4.4d.", _inst->rxHeaderBuffer.handleId, _inst->rxHeaderBuffer.dataSize);
        
        rmcall_handle_t **p_handle = rmcall_isrDict_get(_inst->isrDict, _inst->rxHeaderBuffer.handleId);
        
        if(NULL == p_handle)
        {
            SYSLOG_W("Rx handleID 0x%4.4x not found.", _inst->rxHeaderBuffer.handleId);
            _inst->rxHandle = NULL;
            //_inst->rxDataBuffer = NULL;

            if(0U == _inst->rxHeaderBuffer.dataSize) // No data. Start another rx head immediately.
            {
                SYSLOG_D("No dummy data to receive. Restart rx head.");
                if(kStatus_Success != _inst->teleport->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t)))
                {
                    SYSLOG_W("Restart rx head failed.");
                }
            }
            // recv dummy data.
            else 
            {
                _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_rxHead)) | rmcall_statusFlag_rxData | rmcall_statusFlag_rxIdMissing;
                
                if(_inst->rxHeaderBuffer.dataSize <= HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2) 
                  // If the dummy data size is within the capablity of a single transfer:
                  // Allocate (dataSize + 2) byte of ram, receive (dataSize) bytes of data and discard. 
                  // The first 2 bytes is used to indicate remaining bytes to be recveved,
                  // in this case, will always be 0.
                {


                    SYSLOG_D("Dummy data can be received with a single transfer. Start rx dummy data.");
                    *((uint16_t*)_inst->rxDataBuffer) = 0U;
                    _inst->teleport->xfer_rx((void*)(((uint8_t*)&_inst->rxDataBuffer) + 2U), _inst->rxHeaderBuffer.dataSize);
                }
                else
                  // if the dummy data size exceeds the capablity of a single transfer:
                  // Allocate (HITSIC_RMCALL_PUBLIC_BUF_SIZE) byte of ram, receive (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2) bytes of data and discard,
                  // then continue this process until no data is left.
                  // The first 2 bytes is used to indicate remaining bytes to be recveved, in this case,
                  // will be set to (dataSize - (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2)) and continue decreases.
                {                


                    SYSLOG_D("Dummy data can NOT be received with a single transfer. Start rx dummy data.");
                    *((uint16_t*)_inst->rxDataBuffer) = _inst->rxHeaderBuffer.dataSize - (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2U);
                    _inst->teleport->xfer_rx((void*)(((uint8_t*)&_inst->rxDataBuffer) + 2U), HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2U);
                }
            }
        }
        else 
        {
            _inst->rxHandle = *p_handle;

            if(0U == _inst->rxHeaderBuffer.dataSize) // No data. go idle. wait for handle execute.
            {
                // rx finished. go idle.
                _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
                SYSLOG_D("No data to receive. Run handler directly.");
            }
            else //recv data.
            {
                _inst->statusFlag  = (_inst->statusFlag & (~rmcall_statusFlag_rxBusy)) | rmcall_statusFlag_rxData;
                
                if(_inst->rxHeaderBuffer.dataSize > HITSIC_RMCALL_PUBLIC_BUF_SIZE) 
                {
                    // Error !
                    SYSLOG_E("Insufficent Rx handle buffer size. %4.4d byte(s) required, Got %4.4d byte(s).", _inst->rxHeaderBuffer.dataSize, HITSIC_RMCALL_PUBLIC_BUF_SIZE);
                    assert(0);
                }
                
                SYSLOG_D("Rx data Begin.");
                _inst->teleport->xfer_rx((void*)_inst->rxDataBuffer, _inst->rxHeaderBuffer.dataSize);
            }
        }
        
        break;
        
    case rmcall_statusFlag_rxData:
        if(_inst->statusFlag & rmcall_statusFlag_rxIdMissing)
        {
            if(0U == *((uint16_t*)_inst->rxDataBuffer)) // rx dummy data finished.
            {

                _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
                _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxIdMissing);
                _inst->statusFlag  = _inst->statusFlag | rmcall_statusFlag_rxHead;
                _inst->teleport->xfer_rx(&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
            }
            else if (*((uint16_t*)_inst->rxDataBuffer) <= (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2))
            {
                _inst->teleport->xfer_rx((void*)(((uint8_t*)&_inst->rxDataBuffer) + 2U), *((uint16_t*)_inst->rxDataBuffer));
                *((uint16_t*)_inst->rxDataBuffer) = 0U;
            }
            else
            {
                _inst->teleport->xfer_rx((void*)(((uint8_t*)&_inst->rxDataBuffer) + 2U), HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2U);
                *((uint16_t*)_inst->rxDataBuffer) -= (HITSIC_RMCALL_PUBLIC_BUF_SIZE - 2U);
            }
        }
        else
        {
            // rx finished. go idle.
            _inst->statusFlag  = _inst->statusFlag & (~rmcall_statusFlag_rxBusy);
            SYSLOG_D("Rx data done.");
        }
        break;
    default:
        assert(0); // should never end up here.
        break;
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

    SYSLOG_I("Init begin. v%d.%d.%d",HITSIC_VERSION_MAJOR(SYS_RMCALL_VERSION),HITSIC_VERSION_MINOR(SYS_RMCALL_VERSION), HITSIC_VERSION_PATCH(SYS_RMCALL_VERSION));

    _inst->teleport = _config->teleport;

    _inst->statusFlag = 0U;
    
    _inst->rxHandle = NULL;

    _inst->rxDataBuffer = malloc(HITSIC_RMCALL_PUBLIC_BUF_SIZE);
    if(NULL == _inst->rxDataBuffer)
    {
        //memory allocation error !
        assert(0);
    }

    rmcall_isrDict_init(_inst->isrDict);

    SYSLOG_I("Init comlpete.");
    
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
        SYSLOG_W("Insert fail. Handle ID out of range !");
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

    _inst->txHeaderBuffer.magic = HITSIC_RMCALL_HEADER_MAGIC;
    _inst->txHeaderBuffer.handleId = _handleId;
    _inst->txHeaderBuffer.dataSize = _dataSize;
    _inst->txDataBuffer = _data;

    _inst->statusFlag |= rmcall_statusFlag_txHead;
    SYSLOG_I("Tx head. ID = 0x%4.4x, size = %4.4d.", _handleId, _dataSize);
    ret = _inst->teleport->xfer_tx(&_inst->txHeaderBuffer, sizeof(rmcall_header_t));

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
            SYSLOG_I("Recv enabled.");
        }
        else
        {
            SYSLOG_E("Recv enable failed. Transfer error.");
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
        SYSLOG_I("Recv disabled.");
        return kStatus_Success;
    }
    
    SYSLOG_W("Recv disable failed. Rx is busy.");
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
        SYSLOG_I("Execute handle 0x%4.4x.", _inst->rxHeaderBuffer.handleId);
        // run command
        (*_inst->rxHandle->handler)(_inst->rxDataBuffer, _inst->rxHeaderBuffer.dataSize, _inst->rxHandle->userData);
        // restart rx header
        _inst->statusFlag |= rmcall_statusFlag_rxHead;
        _inst->teleport->xfer_rx((void*)&_inst->rxHeaderBuffer, sizeof(rmcall_header_t));
    }
}


/* @} */

#endif // HITSIC_USE_RMCALL
