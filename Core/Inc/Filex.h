/*
 * Filex.h
 *
 *  Created on: Mar 18, 2025
 *      Author: rameshchary.vadla
 */

#ifndef INC_FILEX_H_
#define INC_FILEX_H_

#include "fx_stm32_mmc_driver.h"


#define FX_MEDIA_BUFFER_SIZE   8192  // 8KB buffer for FileX
#define FILE_SIZE_MB    5        // 1MB
#define CHUNK_SIZE      8192       // 8KB chunks
#define TOTAL_SIZE      (FILE_SIZE_MB * 1024 * 1024) // 1MB in bytes
#define CHUNK_COUNT     (TOTAL_SIZE / CHUNK_SIZE)  // Number of chunks



 #define FX_MMC_VOLUME_NAME "STM32_MMC_DISK"
  #define FX_MMC_NUMBER_OF_FATS             1

#define MMC_NUM_OF_SECTORS            7733248     /* Total number of sectors   */

#define FX_MMC_HIDDEN_SECTORS             0

#define MMC_SECTORSIZE   ((uint32_t)512U) /*!< Sector size is 512 bytes */

//#define MMC_SIZE        ((MMC_SECOTRSIZE * NUM_OF_SECTORS) >> 2) /* Total data size in bytes */



void Open_media(void);
void format(void);
void abort(void);
void Open_N_write(char *ptr,char *dat);
void Open_N_read_file(char *ptr);
void close_media(void);
void flush(void);
void creating_file(char *ptr);

void get_card_info(void);

void Write_1MB_to_eMMC(char *filename);


#endif /* INC_FILEX_H_ */
