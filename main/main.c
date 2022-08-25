#include "freertos/FreeRTOS.h"
#include "esp_task_wdt.h"
#include <string.h>
#include <esp_log.h>
#include "esp_partition.h"


/*******************************************************/
/***************** Local Macros ************************/
/*******************************************************/

#define BUFFER_SIZE 4096
#define PARTITION_NAME "coredump"


/* SDK Configuration
 *
 * Component config > Core dump:
 * 		Destination: Flash
 * 		Format: Binary
 * 		Check integrity: YES
 * 		Max num. of task: 64
 *
 * Component config > ESP System Settings:
 * 		Panic handler behaviour: Print registers and halt
 *
 *
 * Watchdog disabled to print flash
 * Component config > ESP System Settings:
 *
 */

void PMA_Main();
const esp_partition_t* findPMACoreDumpPartition();
void readPMACoreDump(const esp_partition_t* part_CoreDump, char* content, long offset, long size);

COREDUMP_DRAM_ATTR uint8_t global_var;


typedef struct {
  int a;
  char *s;
} dummy_data_t;


void app_main(void)
{
	ESP_LOGI("X","START");

	PMA_Main();

    global_var = 25;

    assert(0);

/*//Uncomment this to provoke a crash (comment assert(0);)
    dummy_data_t *data = NULL;
    if(strlen(data->s) > 10){
        ESP_LOGE("XXX","111");
    }
*/
}



void PMA_Main()
{
    const esp_partition_t* partition = findPMACoreDumpPartition();

    if(NULL != partition)
    {
        ESP_LOGI("PMA", "PMA partition found with size: %d", partition->size);

        static char buffer[BUFFER_SIZE];

        readPMACoreDump(partition, buffer, 0, BUFFER_SIZE);

        for(int i = 0; i < partition->size; i += BUFFER_SIZE)
        {
            memset(buffer, 0, BUFFER_SIZE);
            readPMACoreDump(partition, buffer, i, BUFFER_SIZE);

            ESP_LOGI("PMA", "OFFSET: %d \n", i);

            for (int n = 0 ; n < BUFFER_SIZE; n++ )
            {
               printf("%02x", buffer[n]);
            }
            printf("\n");

        }

    }
    else
    {
        ESP_LOGW("PMA", "PMA partition not found");
    }

}

const esp_partition_t* findPMACoreDumpPartition()
{
    esp_partition_type_t p_type = ESP_PARTITION_TYPE_DATA;
    esp_partition_subtype_t p_subtype = ESP_PARTITION_SUBTYPE_DATA_COREDUMP;
    const char *label = PARTITION_NAME;

    return esp_partition_find_first(p_type, p_subtype, label);
}

void readPMACoreDump(const esp_partition_t* part_CoreDump, char* content, long offset, long size)
{
  esp_err_t func_ret;
  func_ret = esp_partition_read(part_CoreDump, offset, content, size);
}

