/*
 * FileX.c
 *
 *  Created on: Mar 18, 2025
 *      Author: rameshchary.vadla
 */

#include <FileX.h>
#include <fx_api.h>
#include <app_filex.h>
#include <main.h>
#include <string.h>

ALIGN_32BYTES (uint32_t fx_mmc_media_memory[FX_STM32_MMC_DEFAULT_SECTOR_SIZE / sizeof(uint32_t)]);
FX_MEDIA        mmc_disk;

CHAR read_buffer[31];
UINT mmc_status = FX_SUCCESS;

//UCHAR fx_mmc_media_memory[FX_MEDIA_BUFFER_SIZE];

FX_FILE fx_file;

HAL_MMC_CardInfoTypeDef Card_info;

extern UART_HandleTypeDef huart1;
extern MMC_HandleTypeDef hmmc1;

void abort(void)
{
mmc_status = fx_media_abort(&mmc_disk);
}
void format(void)
{


	mmc_status = fx_media_format(&mmc_disk,					// eMMC_disk pointer
				fx_stm32_mmc_driver,								 // Driver entry
				(VOID*) FX_NULL,							// Device info pointer
				(VOID*) fx_mmc_media_memory,				// Media Buffer Pointer
				sizeof(fx_mmc_media_memory),					// Media Buffer Size
				FX_MMC_VOLUME_NAME,                            	   // Volume Name
				FX_MMC_NUMBER_OF_FATS,                             // Number of FATs
				32,                                             // Directory Entries
				FX_MMC_HIDDEN_SECTORS,                             // Hidden sectors
				MMC_NUM_OF_SECTORS, 								// Total sectors
				MMC_SECTORSIZE,		                               // Sector size
				1,                                            // Sectors per cluster
				1,                                                  // Heads
				1);                                             // Sectors per track

}

void Open_media(void)
{


	/* Open the disk driver */
	  mmc_status =  fx_media_open(&mmc_disk, FX_MMC_VOLUME_NAME, fx_stm32_mmc_driver, (VOID *)FX_NULL, (VOID *) fx_mmc_media_memory, sizeof(fx_mmc_media_memory));

	  /* Check the media open mmc_mmc_status */
	    if (mmc_status != FX_SUCCESS)
	    {
	      /* USER CODE BEGIN MMC open error */
	  		Error_Handler();
	      /* USER CODE END MMC open error */
	    }
}


void creating_file(char *ptr)
{
	mmc_status = fx_file_create(&mmc_disk, ptr);
		/* Check the create mmc_status.  */
		if (mmc_status != FX_SUCCESS) {
			/* Check for an already created mmc_status. This is expected on the\
			 second pass of this loop!  */


			if (mmc_status != FX_ALREADY_CREATED) {
				/* Create error, call error handler.  */
				printf("file already exists\n\r");
				Error_Handler();
			}
		}
		printf("file created with name %s\n\r",ptr);

}

void Open_N_write(char *ptr,char *dat){

char data[40];
	/* Open the test file.  */
		mmc_status = fx_file_open(&mmc_disk, &fx_file,  ptr, FX_OPEN_FOR_WRITE);
		/* Check the file open mmc_status.  */
		if (mmc_status != FX_SUCCESS) {
			/* Error opening file, call error handler.  */
			Error_Handler();
		}
		/* Seek to the beginning of the test file.  */
		mmc_status = fx_file_seek(&fx_file, 0);
		/* Check the file seek mmc_status.  */
		if (mmc_status != FX_SUCCESS) {
			/* Error performing file seek, call error handler.  */
			Error_Handler();
		}
		strcpy(data,dat);
		/* Write a string to the test file.  */
		mmc_status = fx_file_write(&fx_file, data, sizeof(data));
		/* Check the file write mmc_status.  */
		if (mmc_status != FX_SUCCESS) {
			/* Error writing to a file, call error handler.  */
			Error_Handler();
		}
		else
		{
			printf("data written : %s\n\r",data);
		}
		/* Close the test file.  */
		mmc_status = fx_file_close(&fx_file);
		/* Check the file close mmc_status.  */
		if (mmc_status != FX_SUCCESS) {
			/* Error closing the file, call error handler.  */
			Error_Handler();
		}
}

void flush(void)
{
	mmc_status = fx_media_flush(&mmc_disk);
		/* Check the media flush  status.  */
		if (mmc_status != FX_SUCCESS) {
			/* Error closing the file, call error handler.  */
			Error_Handler();
		}
}

void Open_N_read_file(char *ptr)
{
	ULONG bytes_read;
	/* Open the test file.  */
		mmc_status = fx_file_open(&mmc_disk, &fx_file, ptr, FX_OPEN_FOR_READ);
		/* Check the file open status.  */
		if (mmc_status != FX_SUCCESS) {
			/* Error opening file, call error handler.  */
			Error_Handler();
		}
//		strcpy(read_buffer,ptr);
		/* Read the first 28 bytes of the test file.  */
		mmc_status = fx_file_read(&fx_file, read_buffer, sizeof(read_buffer), &bytes_read);
		/* Check the file read status.  */
		if ((mmc_status != FX_SUCCESS) || (bytes_read != sizeof(read_buffer))) {
			/* Error reading file, call error handler.  */
			Error_Handler();
		}

//		/* Check if the read data is equal to the written data.  */
//		if (memcmp(read_buffer, data, sizeof(data)) != 0) {
//			/* Error reading file, call error handler.  */
//			Error_Handler();
//		} else {
//	//                                                               HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		printf("read data: %s\n\r",read_buffer);
//		}
		/* Close the test file.  */
		mmc_status = fx_file_close(&fx_file);
		/* Check the file close status.  */
		if (mmc_status != FX_SUCCESS) {
			/* Error closing the file, call error handler.  */
			Error_Handler();
		}

}
void close_media(void)
{
	/* Close the media.  */
			mmc_status = fx_media_close(&mmc_disk);
			/* Check the media close status.  */
			if (mmc_status != FX_SUCCESS) {
				/* Error closing the media, call error handler.  */
				Error_Handler();
			}
}

void available_space()
{
	ULONG available_bytes;
	get_card_info();
	fx_media_space_available(&mmc_disk, &available_bytes);
	printf("Available space: %lu bytes\n", available_bytes);
}

void Write_1MB_to_eMMC(char *filename) {
    UINT status;
    UCHAR write_buffer[CHUNK_SIZE];

    // Fill the buffer with test data
    for (int i = 0; i < CHUNK_SIZE; i++) {
        write_buffer[i] = 'A' + (i % 26); // Repeating A-Z
    }

    // Open the media
    Open_media();

    // Create the file if it doesn't exist
    status = fx_file_create(&mmc_disk, filename);
    if (status != FX_SUCCESS && status != FX_ALREADY_CREATED) {
        printf("File creation failed: %d\n", status);
        Error_Handler();
    }

    // Open the file for writing
    status = fx_file_open(&mmc_disk, &fx_file, filename, FX_OPEN_FOR_WRITE);
    if (status != FX_SUCCESS) {
        printf("Error opening file: %d\n", status);
        Error_Handler();
    }

    // Move to the beginning
    fx_file_seek(&fx_file, 0);
    available_space();
    available_space();
    // Write data in 8KB chunks
    for (int i = 0; i < CHUNK_COUNT; i++) {
        status = fx_file_write(&fx_file, write_buffer, CHUNK_SIZE);
        if (status != FX_SUCCESS) {

            printf("File write failed at chunk %d: %d\n", i, status);
            Error_Handler();
        }
    }

    // Flush and close
    available_space();
    fx_media_flush(&mmc_disk);
    fx_file_close(&fx_file);
    close_media();

    printf("1MB file %s written successfully!\n", filename);
}

void get_card_info(void)
{
	HAL_MMC_GetCardInfo(&hmmc1, &Card_info);
}
