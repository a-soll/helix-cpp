#ifndef CLIENT_H
#define CLIENT_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string>

enum CurlMethod {
    curl_POST,
    curl_GET,
    curl_DELETE,
    curl_PATCH
};

class Client {
  private:
    std::string response_memory;
    int response_size;
    CURLcode response_code;

    std::string client_secret;
    std::string access_token;
    struct curl_slist *headers;
    CURL *curl_handle;
    std::string post_data;
  public:
    Client(const char *access_token, const char *client_id, const char *user_id, const char *user_login);
    ~Client();
    bool validateToken();

    std::string user_id;
    std::string user_login;
    std::string base_url;
    std::string client_id;
    Response *curl_request(Client *client, std::string &url, CurlMethod method)
};

#endif /* CLIENT_H */
