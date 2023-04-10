#include "client.hpp"
#include <iostream>
#include <json/json.h>

#define URL_LEN 2048

Client::Client(const char *access_token, const char *client_id, const char *user_id, const char *user_login) {
    this->user_id = user_id;
    this->user_login = user_login;
    this->client_id = client_id;
    this->access_token = access_token;

    std::string header;
    this->headers = curl_slist_append(this->headers, "Content-Type: application/json");
    this->headers = curl_slist_append(this->headers, "Accept: application/json");
    header.append("Authorization: Bearer ");
    header.append(this->access_token);
    this->headers = curl_slist_append(this->headers, header.c_str());
}

Client::~Client() {
    curl_slist_free_all(this->headers);
}

bool Client::validateToken() {
    bool ret = false;
    std::string url = "https://id.twitch.tv/oauth2/validate";

    return ret;
}

Response *curl_request(Client *client, std::string &url, CurlMethod method) {
    Response *response = malloc(sizeof(Response));
    response->memory = malloc(sizeof(char) * 1);
    response->size = 0;
    client->curl_handle = curl_easy_init();
    response->error[0] = 0;
    response->data_len = 0;

    switch (method) {
    case curl_GET:
        curl_easy_setopt(client->curl_handle, CURLOPT_CUSTOMREQUEST, "GET");
        break;
    case curl_POST:
        curl_easy_setopt(client->curl_handle, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(client->curl_handle, CURLOPT_POSTFIELDS, client->post_data);
        break;
    case curl_DELETE:
        curl_easy_setopt(client->curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(client->curl_handle, CURLOPT_POSTFIELDS, client->post_data);
        break;
    case curl_PATCH:
        curl_easy_setopt(client->curl_handle, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(client->curl_handle, CURLOPT_POSTFIELDS, client->post_data);
        break;
    default:
        curl_easy_setopt(client->curl_handle, CURLOPT_CUSTOMREQUEST, "GET");
    }

    curl_easy_setopt(client->curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(client->curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(client->curl_handle, CURLOPT_HTTPHEADER, client->headers);
    curl_easy_setopt(client->curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(client->curl_handle, CURLOPT_WRITEDATA, (void *)response);
    curl_easy_setopt(client->curl_handle, CURLOPT_ERRORBUFFER, response->error);
    curl_easy_setopt(client->curl_handle, CURLOPT_NOSIGNAL, 1L);

    // curl_easy_setopt(client->curl_handle, CURLOPT_VERBOSE, client->curl_handle);

    response->res = curl_easy_perform(client->curl_handle);
    if (response->res == CURLE_OK) {
        curl_easy_getinfo(client->curl_handle, CURLINFO_RESPONSE_CODE, &response->response_code);
    }
    if (response->memory) {
        response->response = json_tokener_parse(response->memory);
    }
    curl_easy_cleanup(client->curl_handle);
    return response;
}
