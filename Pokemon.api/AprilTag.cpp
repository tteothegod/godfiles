#include <iostream>
#include <string>
#include "curl/curl.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct result
{
    string o_rank;
    float o_percent;
};

string pokemon;
string Text_Data;
string full_url;
result overall_rank;

size_t WriteCallback(char* buf, size_t size, size_t nmemb, void* userp) {
    Text_Data.append(buf, size * nmemb);
    return size * nmemb;
}

result BL_RANK_CHECKER(string pokemon)
{
    // cout << pokemon;
    string line;
    vector<string> data;
    string file_name;
    string delimiter = ",";
    size_t delimiter_pose = 0;
    string process_line;

    string ranking_g[17];
    string ranking_u[17];
    string ranking_m[17];

    for(int k = 0; k < 3; k++)
    {

        int l = 1;

        if (k == 0)
        {
            file_name = "GreatLeague.csv";
        }
        if (k == 1)
        {
            file_name = "UltraLeague.csv";
        }
        else if (k == 2)
        {
            file_name = "MasterLeague.csv";
        }
        ifstream file(file_name);
        if (file.is_open()) 
        {
            // cout << "file open \n";
            while (getline(file, line)) 
            {
                data.push_back(line);
                // cout << "haha \n";
            }

            for (int i = 0; i < data.size(); ++i) 
            {
                // cout << i << " ";
                const auto &entry = data[i];
                if (entry.find(pokemon) != string::npos) 
                {
                    // cout << "Found Pokemon: " << entry << '\n';
                    process_line = entry;

                    while ((delimiter_pose = process_line.find(delimiter)) != std::string::npos)
                    {
                        if(k == 0)
                        {
                            ranking_g[l] = process_line.substr(0, delimiter_pose);
                        }
                        if(k == 1)
                        {
                            ranking_u[l] = process_line.substr(0, delimiter_pose);
                        }
                        if(k == 2)
                        {
                            ranking_m[l] = process_line.substr(0, delimiter_pose);
                        }
                        process_line.erase(0, delimiter_pose + delimiter.length());
                        l = l + 1;
                    }
                    break;
                }
            }
            data.clear();
        }

        else 
        {
            cout << "Error opening file\n";
        }
    }

    if (stof(ranking_g[2]) > stof(ranking_u[2]) && stof(ranking_g[2]) > stof(ranking_m[2]))
    {
        overall_rank.o_rank = "Great League";
        overall_rank.o_percent = stof(ranking_g[2]);
    }
    if (stof(ranking_u[2]) > stof(ranking_g[2]) && stof(ranking_u[2]) > stof(ranking_m[2]))
    {
        overall_rank.o_rank = "Ultra League";
        overall_rank.o_percent = stof(ranking_u[2]);
    }
    if (stof(ranking_m[2]) > stof(ranking_g[2]) && stof(ranking_m[2]) > stof(ranking_u[2]))
    {
        overall_rank.o_rank = "Master League";
        overall_rank.o_percent = stof(ranking_m[2]);
    }
    return overall_rank;
}

void IV_RANKING(string full_url, string pokemon) 
    {
        CURL* curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        full_url = "https://www.stadiumgaming.gg/rank-checker?pokemon=";
        full_url = full_url + pokemon;

        if(curl) 
        {
            curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
            curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\StarG\\Documents\\April Tag\\Curl\\cacert-2023-01-10.pem");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            // To verify SSL certificate
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if(res != CURLE_OK) 
            {
                cout << "cURL error: " << curl_easy_strerror(res) << endl;
            } else 
            {
                //cout << "Text data: " << Text_Data << endl;
            }
        }
        curl_global_cleanup();
    }


int main() 
{
    printf("Enter a pokemon: ");
    cin >> pokemon;
    //printf("Enter a website url: ");
    //cin >> full_url;
    printf("\n");

    BL_RANK_CHECKER(pokemon);
    cout << overall_rank.o_rank << " " << overall_rank.o_percent << "\n";
    IV_RANKING(full_url, pokemon);
    return 0;
}