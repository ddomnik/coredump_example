#include "freertos/FreeRTOS.h"
#include "esp_task_wdt.h"
#include <string.h>
#include <esp_log.h>
#include "esp_partition.h"
#include "esp_core_dump.h"

#define BUFFER_SIZE 4096
#define PARTITION_NAME "coredump"

const char TAG[] = "CoreDump:";


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

void CoreDump_Main();
const esp_partition_t* findCoreDumpPartition();
void readCoreDump(const esp_partition_t* part_CoreDump, char* content, long offset, long size);

COREDUMP_DRAM_ATTR uint8_t global_var;


typedef struct {
  int a;
  char *s;
} dummy_data_t;


void app_main(void)
{
	ESP_LOGI("X","START");

	CoreDump_Main();

    global_var = 25;

    assert(0);

/*//Uncomment this to provoke a crash (comment assert(0);)
    dummy_data_t *data = NULL;
    if(strlen(data->s) > 10){
        ESP_LOGE("XXX","111");
    }
*/
}



void CoreDump_Main()
{
    size_t out_cd_addr;
    size_t out_cd_size;

    esp_core_dump_image_get(&out_cd_addr, &out_cd_size);
    ESP_LOGI(TAG, "core dump addr: %d", out_cd_addr);
    ESP_LOGI(TAG, "core dump size: %d", out_cd_size);


    const esp_partition_t* partition = findCoreDumpPartition();

    if(NULL != partition)
    {
        ESP_LOGI(TAG, "partition found with size: %ld", partition->size);


        esp_core_dump_summary_t *s = malloc(sizeof(esp_core_dump_summary_t));
        if (s) {
            if (esp_core_dump_get_summary(s) == ESP_OK) {

                
                ESP_LOGI(TAG, "Summary:");

                                ESP_LOGI(TAG, "exc_task:");
                ESP_LOG_BUFFER_CHAR(TAG, s->exc_task, 16);

                ESP_LOGI(TAG, "exc_pc: %ld",s->exc_pc);

                ESP_LOGI(TAG, "core_dump_version: %ld",s->core_dump_version);

                ESP_LOGI(TAG, "app_elf_sha256:");
                ESP_LOG_BUFFER_CHAR(TAG, s->app_elf_sha256, APP_ELF_SHA256_SZ);

                ESP_LOGI(TAG, "exc_bt_info->bt:");
                ESP_LOG_BUFFER_HEX(TAG, s->exc_bt_info.bt, 16);

                ESP_LOGI(TAG, "exc_bt_info->depth: %ld",s->exc_bt_info.depth);

                ESP_LOGI(TAG, "exc_bt_info->corrupted: %d",s->exc_bt_info.corrupted);

                ESP_LOGI(TAG, "exc_info->exc_cause: %ld",s->ex_info.exc_cause);

                ESP_LOGI(TAG, "exc_info->exc_vaddr: %ld",s->ex_info.exc_vaddr);

                ESP_LOGI(TAG, "ex_info->exc_a:");
                ESP_LOG_BUFFER_HEX(TAG, s->ex_info.exc_a, 16);

                ESP_LOGI(TAG, "ex_info->epcx:");
                ESP_LOG_BUFFER_HEX(TAG, s->ex_info.epcx, EPCx_REGISTER_COUNT);

                /*
                ESP_LOGI(TAG, "exc_tcb: %ld",s->exc_tcb);

                ESP_LOGI(TAG, "exc_task:");
                ESP_LOG_BUFFER_CHAR(TAG, s->exc_task, 16);

                ESP_LOGI(TAG, "exc_pc: %ld",s->exc_pc);

                ESP_LOGI(TAG, "core_dump_version: %ld",s->core_dump_version);

                ESP_LOGI(TAG, "app_elf_sha256:");
                ESP_LOG_BUFFER_CHAR(TAG, s->app_elf_sha256, APP_ELF_SHA256_SZ);

                ESP_LOGI(TAG, "exc_bt_info->bt:");
                ESP_LOG_BUFFER_CHAR(TAG, s->exc_bt_info->bt, 16);

                ESP_LOGI(TAG, "exc_bt_info->depth: %ld",s->exc_bt_info->depth);

                ESP_LOGI(TAG, "exc_bt_info->corrupted: %d",s->exc_bt_info->corrupted);

                ESP_LOGI(TAG, "exc_info->exc_cause: %d",s->exc_info->exc_cause);

                ESP_LOGI(TAG, "exc_info->exc_vaddr: %d",s->exc_info->exc_vaddr);

                ESP_LOGI(TAG, "ex_info->exc_a:");
                ESP_LOG_BUFFER_CHAR(TAG, s->ex_info->exc_a, 16);

                ESP_LOGI(TAG, "ex_info->epcx:");
                ESP_LOG_BUFFER_CHAR(TAG, s->ex_info->epcx, EPCx_REGISTER_COUNT);
                */
                
                
                
            }
            else{
                ESP_LOGE(TAG,"Summary not found");
            }
        }
        else{
                ESP_LOGE(TAG,"Summary Null");
            }
        free(s);


        ESP_LOGE(TAG,"Actual Core Dump:");


        if(out_cd_size > 0)
        {
			static char buffer[BUFFER_SIZE];

			readCoreDump(partition, buffer, 0, BUFFER_SIZE);

			for(int i = 0; i < BUFFER_SIZE*2; i += BUFFER_SIZE)
			{
				memset(buffer, 0, BUFFER_SIZE);
				readCoreDump(partition, buffer, i, BUFFER_SIZE);

				ESP_LOGI(TAG, "OFFSET: %d \n", i);

				for (int n = 0 ; n < BUFFER_SIZE; n++ )
				{
				   printf("%02x", buffer[n]);
				}
				printf("\n");

			}
        }
        else
        {
        	ESP_LOGI(TAG, "partition found with size: %ld", partition->size);
        }
        

    }
    else
    {
        ESP_LOGW(TAG, "partition not found");
    }

}

const esp_partition_t* findCoreDumpPartition()
{
    esp_partition_type_t p_type = ESP_PARTITION_TYPE_DATA;
    esp_partition_subtype_t p_subtype = ESP_PARTITION_SUBTYPE_DATA_COREDUMP;
    const char *label = PARTITION_NAME;

    return esp_partition_find_first(p_type, p_subtype, label);
}

void readCoreDump(const esp_partition_t* part_CoreDump, char* content, long offset, long size)
{
	esp_partition_read(part_CoreDump, offset, content, size);
}

