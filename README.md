# OpenAI-C-SDK
An advanced, asynchronous and synchronous C++ wrapper for OpenAI's api. 

# Example
```cplusplus
int main() {
  OpenAI openai;

  // Synchronous request
  std::cout << "Synchronous request:" << std::endl;
  std::string response = openai.GetResponse(GPT3, "What is the meaning of life?");
  std::cout << response << std::endl << std::endl;

  // Asynchronous request
  std::cout << "Asynchronous request:" << std::endl;
  std::future<std::string> future = openai.GetResponseAsync(GPT3, "What is the meaning of life?");
  std::cout << "Waiting for response..." << std::endl;
  response = future.get();
  std::cout << response << std::endl;

  return 0;
}
```
