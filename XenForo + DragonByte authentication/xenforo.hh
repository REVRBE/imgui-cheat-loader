#ifndef XENFORO_H
#define XENFORO_H
#pragma once
#include "curl/curl.h"
#include "json/json.h"
#include <iostream>
#include <fstream>
#include "xorstr.h"

inline std::string user_rank;
inline std::string license_expiration;
inline std::vector<int> product_ids;

// You can remove this function, it's not being used, but can provide some help for debugging
bool checkInternetConnection() {
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // Use HEAD request to check only the response header
        CURLcode result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        return (result == CURLE_OK);
    }
    return false;
}

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
    std::string apiUrl = xorstr_("https://smokescreen.rip/forum/login_external.php"); // login.php file for server authentication of user/pass

    curl = curl_easy_init();
    if (curl) {
        // Prepare form data
        std::ostringstream formdataStream;
        formdataStream << xorstr_("username=") << curl_easy_escape(curl, username.c_str(), username.length())
            << xorstr_("&password=") << curl_easy_escape(curl, password.c_str(), password.length());
        std::string formdata = formdataStream.str();

        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, formdata.c_str()); // Send data as POST
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Set headers for the request
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Enable SSL/TLS
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // Verify the server's certificate
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); // Verify the server's identity

        // Optional: Set the path to the CA certificate bundle file
        // curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/cacert.pem");

        res = curl_easy_perform(curl);

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return false;
        }

        writeToLogFile(xorstr_("Server response: ") + readBuffer, xorstr_("log.txt")); // For debugging connection to database

        Json::CharReaderBuilder jsonReader;
        std::string errors;
        Json::Value jsonData;

        std::istringstream jsonStream(readBuffer);
        if (!Json::parseFromStream(jsonReader, jsonStream, &jsonData, &errors)) {
            return false;
        }

        if (jsonData[xorstr_("success")].asBool()) {
            std::string user_rank_received = jsonData[xorstr_("user_rank_data")][xorstr_("user_rank")].asString();
            user_rank = user_rank_received;

            if (user_rank_received == xorstr_("Public") || user_rank_received == xorstr_("Private")) {

            }
            else if (user_rank_received == xorstr_("Registered") || user_rank_received == xorstr_("Unregistered / Unconfirmed")) {
                return false;
            }

            return true;
        }
    }

    return false;
}

#endif