#include <client.h>
#include <cstring>
#include <iostream>

#define URL_LEN 2048

namespace Helix {
Client::Client(const char *access_token, const char *client_id, const char *user_id, const char *user_login) {
    this->user_id = user_id;
    this->user_login = user_login;
    this->client_id = client_id;
    this->access_token = access_token;
    this->headers = NULL;
    this->curl_handle = curl_easy_init();

    this->resetHeaders();
}

Client::Client(const char *access_token, const char *client_id) {
    this->access_token = access_token;
    this->client_id = client_id;
    this->headers = NULL;

    this->resetHeaders();
}

Client::~Client() {
    curl_slist_free_all(this->headers);
    curl_easy_cleanup(this->curl_handle);
    this->headers = NULL;
}

bool Client::validateToken() {
    bool ret = false;
    std::string url = "https://id.twitch.tv/oauth2/validate";

    return ret;
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void Client::curlRequest(const char *url, CurlMethod method, Response &response) {
    switch (method) {
    case curl_GET:
        curl_easy_setopt(this->curl_handle, CURLOPT_CUSTOMREQUEST, "GET");
        break;
    case curl_POST:
        curl_easy_setopt(this->curl_handle, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(this->curl_handle, CURLOPT_POSTFIELDS, this->post_data.c_str());
        break;
    case curl_DELETE:
        curl_easy_setopt(this->curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(this->curl_handle, CURLOPT_POSTFIELDS, this->post_data.c_str());
        break;
    case curl_PATCH:
        curl_easy_setopt(this->curl_handle, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(this->curl_handle, CURLOPT_POSTFIELDS, this->post_data.c_str());
        break;
    default:
        curl_easy_setopt(this->curl_handle, CURLOPT_CUSTOMREQUEST, "GET");
    }

    curl_easy_setopt(this->curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(this->curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(this->curl_handle, CURLOPT_HTTPHEADER, this->headers);
    curl_easy_setopt(this->curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(this->curl_handle, CURLOPT_WRITEDATA, &response.readBuffer);
    curl_easy_setopt(this->curl_handle, CURLOPT_ERRORBUFFER, &response.errBuffer);
    curl_easy_setopt(this->curl_handle, CURLOPT_NOSIGNAL, 1L);

    // curl_easy_setopt(client->curl_handle, CURLOPT_VERBOSE, client->curl_handle);
    response.curl_code = curl_easy_perform(this->curl_handle);
    if (response.curl_code == CURLE_OK) {
        curl_easy_getinfo(this->curl_handle, CURLINFO_RESPONSE_CODE, &response.response_code);
    }
    if (!response.readBuffer.empty()) {
        Json::Reader reader;
        reader.parse(response.readBuffer, response.response);
    }
    this->resetHeaders();
}

Response::Response() {
    this->size = 0;
}

void Client::resetHeaders() {
    if (this->headers) {
        curl_slist_free_all(this->headers);
        this->headers = NULL;
    }
    std::string header;
    size_t len;

    this->headers = curl_slist_append(this->headers, "Content-Type: application/json");
    this->headers = curl_slist_append(this->headers, "Accept: application/json");

    header.append("Authorization: Bearer ");
    header.append(this->access_token);
    this->headers = curl_slist_append(this->headers, header.c_str());

    header.clear();
    header.append("Client-Id: ");
    header.append(this->client_id);
    this->headers = curl_slist_append(this->headers, header.c_str());
}

} // namespace Helix
