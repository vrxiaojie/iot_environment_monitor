#pragma once

extern TaskHandle_t ntp_sync_task_handle;

void ntp_init();
void ntp_sync_task(void *arg);