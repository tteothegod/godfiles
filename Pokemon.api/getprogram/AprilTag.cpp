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
    string league;
};

string pokemon;
string Text_Data;
string full_url;
result overall_rank;
string iv;

size_t WriteCallback(char* ptr, size_t size, size_t nmemb, string* data) {
    data->append(ptr, size * nmemb);
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
            file_name = "/league/GreatLeague.csv";
        else if (k == 1)
        {
            file_name = "./league/UltraLeague.csv";
        }
        else if (k == 2)
        {
            file_name = "./league/MasterLeague.csv";
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
                else 
                {
                    if (k == 0)
                    {
                    ranking_g[2] = "0";
                    }
                    if (k == 1)
                    {
                    ranking_u[2] = "0";
                    }
                    if (k == 2)
                    {
                    ranking_m[2] = "0";
                    }
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
        overall_rank.league = "1500";
    }
    if (stof(ranking_u[2]) > stof(ranking_g[2]) && stof(ranking_u[2]) > stof(ranking_m[2]))
    {
        overall_rank.o_rank = "Ultra League";
        overall_rank.o_percent = stof(ranking_u[2]);
        overall_rank.league = "2500";
    }
    if (stof(ranking_m[2]) > stof(ranking_g[2]) && stof(ranking_m[2]) > stof(ranking_u[2]))
    {
        overall_rank.o_rank = "Master League";
        overall_rank.o_percent = stof(ranking_m[2]);
        overall_rank.league = "false";
    }
    return overall_rank;
}

void IV_RANKING(string full_url, string pokemon, string iv, result overall_rank) 
    {
        CURL* curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        full_url = "https://pvpivs.com/?mon=" + pokemon + "&r=0&cp=" + overall_rank.league + "&fel=mirror&IVs=" + iv;
        cout << full_url << endl;

        string Text_Data;
        if(curl) 
        {
            curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
            curl_easy_setopt(curl, CURLOPT_CAINFO, "./Curl/cacert-2023-01-10.pem");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &Text_Data);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if(res != CURLE_OK) 
            {
                cout << "cURL error: " << curl_easy_strerror(res) << endl;
            } else 
            {
                // cout << "HTML data: " << Text_Data << endl;
            }
        }
        curl_global_cleanup();
        string command = "open " + full_url;
        cout << command << "\n";
        int result = system(command.c_str());
        return;
    }


int main() 
{
    printf("Enter a pokemon: ");
    cin >> pokemon;
    printf("Enter Iv: ");
    cin >> iv;
    printf("\n");

    BL_RANK_CHECKER(pokemon);
    cout << overall_rank.o_rank << " " << overall_rank.o_percent << "\n";
    IV_RANKING(full_url, pokemon, iv, overall_rank);
    return 0;
}