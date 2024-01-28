#include <curl/curl.h>

#include "crawler.h"

Crawler::Crawler(std::vector<std::string> seed_urls, int max_threads, int max_urls_cnt) {
    seed_urls_ = seed_urls;
    for (const std::string& url : seed_urls) {
        urls_queue_.Push(url);
        visited_urls_.Set(url, 1);
    }
    max_threads_ = max_threads;
    max_urls_cnt_ = max_urls_cnt;
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

static size_t callback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    ((std::string*) output)->append(static_cast<char*>(contents), total_size);
    return total_size;
}

std::string Crawler::ScrapeWebpage(std::string url) const {
    CURL* curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        std::string website_content;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &website_content);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            std::cout << "Content for " + url << " scraped successfully\n"; 
            return website_content;
        } else {
            std::cerr << url + " : " << curl_easy_strerror(res) << std::endl;
            return "Empty";
        }
    } else {
        return "Failed to create CURL handle";
    }
}

std::vector<std::string> Crawler::ParseContent(std::string html_content) const {
    // TODO: More robust regex
    std::regex url_regex("(https?://\\S+)");

    std::sregex_iterator url_iterator(html_content.begin(), html_content.end(), url_regex);
    std::sregex_iterator end;

    std::vector<std::string> urls_found;

    while (url_iterator != end) {
        std::smatch match = *url_iterator;
        urls_found.push_back(match.str());
        url_iterator++;
    }

    return urls_found;
}

void Crawler::Run() {
    running_status_ = true;
    // TODO: Alternate termination
    while (visited_urls_cnt_ < max_urls_cnt_) {
        if (working_threads_ < max_threads_ && !(urls_queue_.Front().empty())) {
            working_threads_++;
            std::thread url_thread([this] {
                std::string new_url = urls_queue_.GetAndPop();
                std::string html_content = ScrapeWebpage(new_url);
                visited_urls_cnt_++;
                std::vector<std::string> linked_urls = ParseContent(html_content);
                for (const std::string& linked_url : linked_urls) {
                    if (!visited_urls_.Get(linked_url)) {
                        visited_urls_.Set(linked_url, 1);
                        urls_queue_.Push(linked_url);
                    }
                }
                working_threads_--;
            });
            url_thread.detach();
        }
    }
    running_status_ = false;
}



