#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <curl/curl.h>

#include "crawler.h"

int main() {
    std::vector<std::string> seed_urls = {"https://www.flipkart.com",
                                          "https://www.amazon.com" };
    
    Crawler crawler(seed_urls, 5, 100);
    crawler.Run();
    while (crawler.GetRuningStatus());

    std::map<std::string, int> urls_crawled = crawler.GetVisitedURLs();
    for (const auto url : urls_crawled) {
        std::cout << url.first << std::endl;
    }

    return 0;
}