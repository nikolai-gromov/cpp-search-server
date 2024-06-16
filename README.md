# Search Server
This is a search server implementation that provides efficient search functionality for documents. The server supports various features, such as:

* Adding and removing documents
* Searching for top relevant documents based on a given query
* Handling stop words and query parsing
* Parallel processing of queries
* Matching documents with a given query

The server uses an inverted index data structure to store and retrieve document information efficiently. It also supports parallel processing of queries to achieve high performance.
___
<!--A block of information about the repository in badges-->
![Static Badge](https://img.shields.io/badge/Nikolai_Dev-Search_Server-blue?style=plastic) ![GitHub License](https://img.shields.io/github/license/nikolai-gromov/cpp-search-server?style=plastic) ![GitHub top language](https://img.shields.io/github/languages/top/nikolai-gromov/cpp-search-server?style=plastic) ![GitHub language count](https://img.shields.io/github/languages/count/nikolai-gromov/cpp-search-server?style=plastic) ![GitHub repo stars](https://img.shields.io/github/stars/nikolai-gromov/cpp-search-server) ![GitHub issues](https://img.shields.io/github/issues/nikolai-gromov/cpp-search-server?style=plastic)

[![Logotype](/docs/logo.jpg)](https://github.com/nikolai-gromov/cpp-search-server/tree/main/search-server)
___


The development was carried out in the Windows Subsystem for Linux (WSL) from Visual Studio Code.
<!--Setting-->
## Setting up the development environment

* [Using C++ and WSL in VS Code](https://code.visualstudio.com/docs/cpp/config-wsl)
* [Get started with CMake Tools on Linux](https://code.visualstudio.com/docs/cpp/cmake-linux)
* Intel Thread Building Blocks (TBB) supporting library
    * Ubuntu/Debian:
    ```sudo apt-get install libtbb-dev```