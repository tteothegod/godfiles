#include <iostream>
#include <cstdlib>
#include "../classFiles/SourceRetriever.h"
#include <algorithm>




int main()
{

    ////////* Source Retriever *////////
    SourceRetriever retriever;
    retriever.setLinkMethod(SourceRetriever::LINK_INPUT);
    retriever.scalp();
    retriever.parseLinksAndCitations();
    SourceData elements = retriever.getElements();

    // Remove ^ from beginning of citations
    elements.removeSpecificChar('^', 0, 0, 0);

    // Credible website domains
    const std::vector<std::string> credibleDatabases = {
        "doi.org", "jstor.org", "springer.com", "sciencedirect.com",
        "researchgate.net", "avrxiv.org"
    };

    // Partition by credibility
    SourceData databasePartitionVec = elements;
    SourceData remainingSources;
    databasePartitionVec.partition( remainingSources, PartByCredibility(credibleDatabases, 1, false));

    SourceData possiblyCredibleSources = remainingSources;
    possiblyCredibleSources.partition( remainingSources, PartByConsistentChar('\"', 0, 0, true));



    std::vector<std::pair<std::string, int>> loadedCitations;

    bool load = false;
    {
        std::string strInput;
        while(strInput != "yes" && strInput != "no") {
            std::cout << "Would you like to load the previous data? (yes or no)\n";
            std::cin >> strInput;
            if(strInput == "yes") load = true;
        }  
    }

    SourceData possibleSourcesCopy = SourceData(possiblyCredibleSources);
    if(load) {
        std::ifstream inputFile("labeled_citations.dat", std::ios::binary);

        // Check if inputFile is open
        if (inputFile.is_open()) {
            size_t size;
            inputFile.read(reinterpret_cast<char*>(&size), sizeof(size));  // Read the size of the vector

            // Load each pair (citation and label)
            for (size_t i = 0; i < size; ++i) {
                size_t citationLength;
                inputFile.read(reinterpret_cast<char*>(&citationLength), sizeof(citationLength));  // Read the length of the citation
                std::string citation(citationLength, '\0');
                inputFile.read(&citation[0], citationLength);  // Read the citation
                int label;
                inputFile.read(reinterpret_cast<char*>(&label), sizeof(label));  // Read the label
                loadedCitations.push_back({citation, label});
            }

            std::cout << "Data loaded from labeled_citations.dat" << std::endl;

            for(int i = 0; i < possibleSourcesCopy.size(); i++) {
                std::vector<std::vector<std::string>>& element = possibleSourcesCopy[i];
                for(std::pair<std::string, int>& pair: loadedCitations) {
                    if(element[0][0] == pair.first) {
                        possibleSourcesCopy.erase(possibleSourcesCopy.begin() + i);
                    }
                }
            }
        }

        inputFile.close();
    }







    // Prompt user to label sources as 0 fro not credible and 1 for credible
    std::vector<int> userInputs;
    int intInput;
    for(int i = 0; i < possibleSourcesCopy.size(); i++) {
        const std::vector<std::vector<std::string>>& element = possibleSourcesCopy[i];
        std::cout << "Possible Source " << i + 1 << std::endl;
        for(int l = 0; l < element.size(); l++) {
            const std::vector<std::string>& subelement = element[l];
            std::cout << "  sub-Element " << l + 1 << ":" << std::endl;
            for(std::string item: subelement) {
                std::cout << "    - "<< item << std::endl;
            }
        }
        std::cout << "----------------\n";
        std::cout << "Is the reference credible? (0 or 1): ";
        system(("open " + element[1][0]).c_str());
        std::cin >> intInput;
        std::cout << "----------------\n";
        // if (intInput != int) return -1;
        if (intInput != 1 && intInput != 0) break;
        else if (intInput == -1) return -1; // if you put 1 in itll abort
        loadedCitations.push_back({ element[0][0], intInput});
    }

    for (const auto& citation : loadedCitations) {
        std::cout << "Citation: " << citation.first << " | Label: " << citation.second << std::endl;
    }


    {    
        std::string strInput;
        while(strInput != "yes" && strInput != "no") {
            std::cout << "Here are the citations to be saved^\n" << "Would you like to save them? (yes or no)\n";
            std::cin >> strInput;
            if(strInput == "no") return -1;
        }  
    }



    std::ofstream outputFile("labeled_citations.dat", std::ios::binary);
    // Save the size of the vector first
    size_t size = loadedCitations.size();
    outputFile.write(reinterpret_cast<char*>(&size), sizeof(size));

    // Save each pair (citation and label)
    for ( auto& pair : loadedCitations) {
        size_t citationLength = pair.first.size();
        outputFile.write(reinterpret_cast<char*>(&citationLength), sizeof(citationLength));  // Write length of the citation
        outputFile.write(pair.first.c_str(), citationLength);  // Write the citation
        outputFile.write(reinterpret_cast<char*>(&pair.second), sizeof(pair.second));  // Write the label
    }

    // Finalize
    outputFile.close();
    std::cout << "Data saved to labeled_citations.dat" << std::endl;

    // Print Statements
    // SourceRetriever::print( remainingSources, "Unfiltered Source #");
    // SourceRetriever::print(possiblyCredibleSources, "Possibly Credible #");
    // SourceRetriever::print(credibleSources, "Credible #");
    // std::cout << credibleSources.size() << " " << possiblyCredibleSources.size() << " " << remainingSources.size() << std::endl;
}