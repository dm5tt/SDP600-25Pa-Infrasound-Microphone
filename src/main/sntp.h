#ifndef SNTP_H_
#define SNTP_H_

void initialize_sntp();
void wait_for_sntp_sync();
void update_time_task(void *pvParameters);
#endif