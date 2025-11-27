
#include <curl/curl.h>
#include <stdexcept>

#include <modules/ftp/CurlHandle.h>

namespace embeddedmz {
   CurlHandle::CurlHandle() {
      if (const auto eCode = curl_global_init(CURL_GLOBAL_ALL); eCode != CURLE_OK) {
         throw std::runtime_error{"Error initializing libCURL"};
      }
   }

   CurlHandle::~CurlHandle() { curl_global_cleanup(); }

   CurlHandle &CurlHandle::instance() {
      static CurlHandle inst{};
      return inst;
   }
} // namespace embeddedmz
