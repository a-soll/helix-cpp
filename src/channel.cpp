#include <channel.h>
#include <client.h>
#include <iostream>

namespace Helix {

Channel::Channel(){};

SearchedChannel::SearchedChannel(const Json::Value &value) {
    this->broadcaster_language = value["broadcaster_language"].asString();
    this->broadcaster_login = value["broadcaster_login"].asString();
    this->game_id = value["game_id"].asString();
    this->game_name = value["game_name"].asString();
    this->id = value["id"].asString();
    this->tag_ids = value["tag_ids"][0].asString();
    this->thumbnail_url = value["thumbnail_url"].asString();
    this->title = value["title"].asString();
    this->started_at = value["started_at"].asString();
    this->is_live = value["is_live"].asBool();
    for (int i = 0; i < value["tags"].size(); i++) {
        this->tags[i].append(value["tags"][i].asString());
    }
}

SearchedChannel::~SearchedChannel(){};

SearchedChannelList::SearchedChannelList(){};
SearchedChannelList::~SearchedChannelList(){};

void SearchedChannelList::search(Client &client, const char *keyword, bool live_only) {
    std::string url = "https://api.twitch.tv/helix/search/channels?first=100&live_only=";
    Response response;
    const char *live_flag = (live_only == true) ? "true" : "false";
    url.append(live_flag);
    url.append("&query=");
    url.append(keyword);
    client.curlRequest(url.c_str(), curl_GET, response);
    const Json::Value &val = response.response["data"];
    for (int i = 0; i < response.response["data"].size(); i++) {
        SearchedChannel c = val[i];
        this->channels.push_back(c);
    }
    this->_search.pagination = response.response["pagination"]["cursor"].asString();
    this->_search.keyword = keyword;
    this->_search.live_only = live_flag;
}

void SearchedChannelList::getNext(Client &client) {
    std::string url = "https://api.twitch.tv/helix/search/channels?first=100&live_only=";
    Response response;
    url.append(this->_search.live_only);
    url.append("&query=");
    url.append(this->_search.keyword);
    url.append("&after=");
    url.append(this->_search.pagination);
    client.curlRequest(url.c_str(), curl_GET, response);

    const Json::Value &val = response.response["data"];
    for (int i = 0; i < val.size(); i++) {
        SearchedChannel c = val[i];
        this->channels.push_back(c);
    }
    this->_search.pagination = response.response["pagination"]["cursor"].asString();
}

SearchedChannelList SearchedChannelList::get(Client &client, const char *keyword, bool live_only) {
    SearchedChannelList chans;
    chans.search(client, keyword, live_only);
    return chans;
}

} // namespace Helix
