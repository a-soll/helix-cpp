#ifndef CLIENT_H
#define CLIENT_H

#include <curl/curl.h>
#include <json/json.h>
#include <string>
#include <vector>

namespace Helix {

enum CurlMethod {
    curl_POST,
    curl_GET,
    curl_DELETE,
    curl_PATCH
};

class Response {
  public:
    int size;
    std::string readBuffer;
    Json::Value response;
    CURLcode curl_code;
    long response_code;
    std::string errBuffer;

    Response();
};

class Client {
  public:
    std::string client_secret;
    std::string access_token;
    std::string user_id;
    std::string user_login;
    std::string base_url;
    std::string client_id;
    struct curl_slist *headers;
    CURL *curl_handle;
    std::string post_data;

    Client(const char *access_token, const char *client_id, const char *user_id, const char *user_login);
    Client(const char *access_token, const char *client_id);
    ~Client();

    bool validateToken();
    void curlRequest(const char *url, CurlMethod method, Response &response);
    void resetHeaders();
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
} // namespace Helix
#endif /* CLIENT_H */
