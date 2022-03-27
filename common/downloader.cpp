#include "stdafx.h"

#include "downloader.h"

INT nDownloaderFlag = 0;

void init_curl()
{
	curl_global_init(CURL_GLOBAL_ALL);
}


void finish_curl()
{
	curl_global_cleanup();
}

static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
{
	(void)ptr;
	(void)data;
	/* we are not interested in the headers itself,
	so we only return the size we would have saved ... */
	return (size_t)(size * nmemb);
}

INT get_download_size(const char* url)
{
	INT ret = 0;

	CURL *curl;
	CURLcode res;
	long filetime = -1;
	double filesize = 0.0;
	//const char *filename = strrchr(ftpurl, '/') + 1;

	curl = curl_easy_init();
	if (curl) {
		curl_version_info_data * vinfo = curl_version_info(CURLVERSION_NOW);

		if (vinfo->features & CURL_VERSION_SSL) {
			int a = 0;//enable
		}
		else {
			int a = 0;//disable
		}

		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		/* No download if the file */
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		/* Ask for filetime */
		curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
		/* Switch on full protocol/debug output */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);

		if (CURLE_OK == res) {
			res = curl_easy_getinfo(curl, CURLINFO_FILETIME, &filetime);
			if ((CURLE_OK == res) && (filetime >= 0)) {
				time_t file_time = (time_t)filetime;
			}
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,
				&filesize);
			if ((CURLE_OK == res) && (filesize > 0.0))
			{
				ret = (int)filesize;
			}
		}
		else {
			/* we failed */
			//fprintf(stderr, "curl told us %d\n", res);
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	return ret;
}