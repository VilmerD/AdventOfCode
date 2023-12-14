#pragma once

#include <fstream>
#include <iostream>

#include <string>
#include <vector>

#include <thread>

namespace CCL
{

    namespace Array 
    {
    }

    /* Parsing input data */
    namespace IO
    {

        /* Useful constants */
        static const char *NUMBERS = "0123456789";

        /* Parsing lines */
        std::string GetFirstNotOf(const std::string &line, const char* ID);
        
        std::vector<std::string> SplitLine(const std::string &line, const char *ID);
        std::vector<long> SplitLongs(const std::string &line);
        std::vector<int> SplitInts(const std::string &line);

        class Parser
        {
        public:
            Parser() { input = ""; };
            Parser(const std::string &);

            void ParseInput();

        protected:
            std::string input;
            std::ifstream file;

            virtual void ParseHeader() {};
            virtual void ParseLine() {};
            virtual void ParseFooter() {};
        };

    }
}