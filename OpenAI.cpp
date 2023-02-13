#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <future>
#include <curl/curl.h>

enum OpenAI_Models {
  GPT3,
  DALL·E,
  CLIP,
  Codex,
  DALL·E2
};

class OpenAI_Request {
public:
  OpenAI_Request(OpenAI_Models model, const std::string& prompt)
    : model_(model), prompt_(prompt) {}

  std::string MakeRequest() {
    std::string endpoint;
    switch (model_) {
      case GPT3:
        endpoint = "https://api.openai.com/v1/engines/gpt-3/jobs";
        break;
      case DALL·E:
        endpoint = "https://api.openai.com/v1/images/generations";
        break;
      case CLIP:
        endpoint = "https://api.openai.com/v1/images/generations";
        break;
      case Codex:
        endpoint = "https://api.openai.com/v1/codex/jobs";
        break;
      case DALL·E2:
        endpoint = "https://api.openai.com/v1/images/generations";
        break;
      default:
        return "Error: Invalid model";
    }

    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    std::string data = "{\"prompt\":\"" + prompt_ + "\"}";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      return "Error: " + std::string(curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);

    return response;
  }

private:
  static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
 
  }

  OpenAI_Models model_;
  std::string prompt_;
};

class OpenAI {
public:
  OpenAI() {}

  std::string GetResponse(OpenAI_Models model, const std::string& prompt) {
    OpenAI_Request request(model, prompt);
    return request.MakeRequest();
  }

  std::future<std::string> GetResponseAsync(OpenAI_Models model, const std::string& prompt) {
    return std::async(std::launch::async, [=] { return GetResponse(model, prompt); });
  }

private:
  std::mutex mutex_;
};
