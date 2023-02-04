#include <iostream>
#include <fstream>
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

result BL_RANK_CHECKER(string pokemon)
{
    string line;
    vector<string> data;
    string file_name;
    string delimiter = ",";
    string delim_shadow = "Shadow";
    size_t delim_shadow_pose = 0;
    size_t delimiter_pose = 0;
    string delim_pokemon;
    string process_line;
    string ranking_g[17];
    string ranking_u[17];
    string ranking_m[17];

    for(int k = 0; k < 3; k++)
    {
        int l = 1;

        if (k == 0)
        {
            file_name = "./league/GreatLeague.csv";
        }
        if (k == 1)
        {
            file_name = "./league/UltraLeague.csv";
        }
        else if (k == 2)
        {
            file_name = "./league/MasterLeague.csv";
        }

        if ((delim_shadow_pose = pokemon.find(delim_shadow)) != std::string::npos)
        {
            pokemon.erase(0, delim_shadow_pose + delim_shadow.length() + 1);
            delim_pokemon = pokemon + " (Shadow)";
        }
        else
        {
            delim_pokemon = pokemon;
        }

        ifstream file(file_name);
        if (file.is_open()) 
        {
            while (getline(file, line)) 
            {
                data.push_back(line);
            }
            for (int i = 0; i < data.size(); ++i) 
            {
                const string &entry = data[i];
                if (entry.find(delim_pokemon) != string::npos) 
                {
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
            std::cout << "Error opening file\n";
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

void IV_RANKING(string pokemon, string iv, result overall_rank) 
    {
        string shadow = "";
        iv = iv + ",";
        string delim_comma = ",";
        size_t delim_comma_pose = 0;
        string delim_shadow = "Shadow";
        size_t delim_shadow_pose = 0;
        string ivs[3];
        int i = 0;
        while ((delim_comma_pose = iv.find(delim_comma)) != std::string::npos)
        {
            ivs[i] = iv.substr(0, delim_comma_pose);
            iv.erase(0, delim_comma_pose + delim_comma.length());
            i = i + 1;
        }
        if (pokemon.find(delim_shadow) != std::string::npos)
        {
            shadow = "_true";
            pokemon.erase(0, delim_shadow_pose + delim_shadow.length() + 1);

        }
        iv = ivs[0] + "_" + ivs[1] + "_" + ivs[2];
        string full_url = "https://pvpivs.com/?mon=" + pokemon + "&r=0&cp=" + overall_rank.league + "&fel=mirror&IVs=" + iv + shadow;
        string command = "open '" + full_url + "'";

        if (overall_rank.o_percent >= 70)
        {
            system(command.c_str());
        }
        else
        {
            std::cout << pokemon + "'s league rating is too low \n";
        }
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
    std::cout << overall_rank.o_rank << " " << overall_rank.o_percent << "\n";
    IV_RANKING(pokemon, iv, overall_rank);
    return 0;
}