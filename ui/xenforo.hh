#pragma once
#include "../globals.hh"

#include <curl/curl.h>
#include <json/json.h>
#include <iostream>
#include <fstream>

void writeToLogFile(const std::string& content, const std::string& filename) {
    std::ofstream logFile(filename, std::ios_base::app);
    if (logFile.is_open()) {
        logFile << content << std::endl;
        logFile.close();
    }
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool checkLoginCredentials(const std::string& username, const std::string& password) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string apiUrl = "https://your-xenforo-forum-link.com/forum/login.php"; // login.php file for server authentication of user/pass

    curl = curl_easy_init();
    if (curl) {
        // Prepare form data
        std::ostringstream formdataStream;
        formdataStream << "username=" << curl_easy_escape(curl, username.c_str(), username.length())
                       << "&password=" << curl_easy_escape(curl, password.c_str(), password.length());
        std::string formdata = formdataStream.str();

        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, formdata.c_str()); // Send data as POST
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Set headers for the request
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return false;
        }

        writeToLogFile("Server response: " + readBuffer, "log.txt");

        Json::CharReaderBuilder jsonReader;
        std::string errors;
        Json::Value jsonData; // Define the jsonData variable

        std::istringstream jsonStream(readBuffer);
        if (!Json::parseFromStream(jsonReader, jsonStream, &jsonData, &errors)) {
            return false;
        }

        if (jsonData["success"].asBool()) {
            std::string user_rank_received = jsonData["user_rank"].asString();
            std::cout << "Received user rank: " << user_rank_received << std::endl; // Debug statement
            globals.user_rank = user_rank_received;
            return true;
        }
    }

    return false;
}
