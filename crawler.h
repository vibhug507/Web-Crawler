#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <thread>
#include <regex>
#include <string>
#include <map>

#include "thread_safe/thread_safe_map.h"
#include "thread_safe/thread_safe_queue.h"

class Crawler {
    public:

    Crawler(std::vector<std::string> seed_urls, int max_threads = 5, int max_urls_cnt = 100);

    std::string ScrapeWebpage(std::string url) const;

    std::vector<std::string> ParseContent(std::string html_content) const;

    void Run();

    bool GetRuningStatus() const { return running_status_; }
    
    std::map<std::string, int> GetVisitedURLs() const { return visited_urls_.GetMap(); }
    
    private:

    // TODO: Make these thread-safe
    int visited_urls_cnt_ = 0;
    int max_threads_;
    int working_threads_ = 0;
    int max_urls_cnt_;
    bool running_status_ = false;

    ThreadSafeMap<std::string, int> visited_urls_;
    ThreadSafeQueue<std::string> urls_queue_;

    std::vector<std::string> seed_urls_;
    std::vector<std::thread> threads_running_;
};