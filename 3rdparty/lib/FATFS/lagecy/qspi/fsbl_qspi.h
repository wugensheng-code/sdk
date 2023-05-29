#ifndef _FSBL_QSPI_H_
#define _FSBL_QSPI_H_




#ifdef   QSPI_EXTENSION_FUNCTION
int32_t Qspi_Sste(QSPI_CORE_HANDLE *pdev, uint32_t sste_mode);
void Qspi_ClkStretchEn(QSPI_CORE_HANDLE *pdev, uint32_t val);
#endif




#ifdef QSPI_DEBUG
void Qspi_Info(QSPI_CORE_HANDLE *pdev);
#endif

#endif