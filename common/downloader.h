#pragma once

#include "../common/Inc/curl/curl.h"
#include "../common/TRegistry.h"
#include "../common/regstr.h"

#define  UM_RETRY_DOWNLOAD		WM_USER + 0x20
extern INT nDownloaderFlag;

void init_curl();
void finish_curl();

INT get_download_size(const char* url);