#ifndef CHANNEL_H
#define CHANNEL_H

#include <client.h>
#include <json/json.h>

namespace Helix {

struct Search {
    std::string pagination;
    std::string keyword;
    std::string live_only;
};

class Channel {
  public:
    std::string broadcaster_id;
    std::string broadcaster_login;
    std::string broadcaster_name;
    std::string broadcaster_language;
    std::string game_id;
    std::string game_name;
    std::string title;
    int delay;

    Channel();
    ~Channel();
};

class ChannelList {
  public:
    std::vector<Channel> channels;

    ChannelList();
    ~ChannelList();
  private:
    Search _search;
};

class SearchedChannel {
  public:
    std::string broadcaster_language;
    std::string broadcaster_login;
    std::string game_id;
    std::string game_name;
    std::string id;
    std::string tag_ids;
    std::string thumbnail_url;
    std::string title;
    std::string started_at;
    std::array<std::string, 10> tags;
    bool is_live;

    SearchedChannel(const Json::Value &value);
    ~SearchedChannel();
};

class SearchedChannelList {
  public:
    std::vector<SearchedChannel> channels;
    static SearchedChannelList get(Client &client, const char *keyword, bool live_only = true);

    SearchedChannelList();
    ~SearchedChannelList();

    void search(Client &client, const char *keyword, bool live_only = true);
    void getNext(Client &client);
  private:
    Search _search;
};

} // namespace Helix

#endif /* CHANNEL_H */
