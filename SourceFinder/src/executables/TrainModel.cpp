#include <torch/torch.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>     // For std::random_device and std::mt19937



// Simple feedforward model
struct CitationClassifier : torch::nn::Module {
    torch::nn::Linear fc1{nullptr}, fc2{nullptr};
    
    CitationClassifier() {
        fc1 = register_module("fc1", torch::nn::Linear(500, 256));  // Input size 500 for max length
        fc2 = register_module("fc2", torch::nn::Linear(256, 1));
    }
    
    torch::Tensor forward(torch::Tensor x) {
        x = torch::relu(fc1(x));
        return torch::sigmoid(fc2(x));  // No need for unsqueeze(1)
    }
};

// Simple preprocessing function (converts string to tensor directly)
torch::Tensor preprocess(const std::string& citation, int max_length) {
    std::vector<float> input_vector(max_length, 0.0f);
    int length = std::min(static_cast<int>(citation.size()), max_length);
    for (int i = 0; i < length; ++i) {
        input_vector[i] = static_cast<float>(citation[i]);  // Encoding each character by its ASCII value
    }
    return torch::tensor(input_vector, torch::dtype(torch::kFloat)).unsqueeze(0);  // Add batch dimension
}

int main() {
    std::vector<std::pair<std::string, int>> labeledCitations;

    std::ifstream file("labeled_citations.dat", std::ios::binary);

    // Check if file is open
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));  // Read the size of the vector

    // Load each pair (citation and label)
    for (size_t i = 0; i < size; ++i) {
        size_t citationLength;
        file.read(reinterpret_cast<char*>(&citationLength), sizeof(citationLength));  // Read the length of the citation
        std::string citation(citationLength, '\0');
        file.read(&citation[0], citationLength);  // Read the citation
        int label;
        file.read(reinterpret_cast<char*>(&label), sizeof(label));  // Read the label
        labeledCitations.push_back({citation, label});
    }

    // Close file
    file.close();

    std::cout << "Data loaded from labeled_citations.dat" << std::endl;

    std::cout << "Data set size: " << labeledCitations.size() << std::endl;


    CitationClassifier model;
    
    // Example citations
    // std::vector<std::pair<std::string, int>> dataset = {
    //     {"^Richards, Sam (8 April 2009),\"Album review: Radiohead Reissues – Collectors Editions\",Uncut,archivedfrom the original on 6 December 2010, retrieved29 August2011", 0},
    //     {"^Jonathan, Emma.\"BBC Worldwide takes exclusive Radiohead performance to the world\".BBC. 3 May 2011. Retrieved 30 November 2023.", 0},
    //     {"^abMcLean, Craig (14 July 2003).\"Don't worry, be happy\".The Sydney Morning Herald.Archivedfrom the original on 1 October 2019. Retrieved25 December2007.", 0},
    //     {"^Ohtomo, Yoko; Kakegawa, Takeshi; Ishida, Akizumi; Nagase, Toshiro; Rosing, Minik T. (January 2014). \"Evidence for biogenic graphite in early Archaean Isua metasedimentary rocks\".Nature Geoscience.7(1):25–28.Bibcode:2014NatGe...7...25O.doi:10.1038/ngeo2025.", 1},
    //     {"^Schopf, J. William (29 June 2006).\"Fossil evidence of Archaean life\".Philosophical Transactions of the Royal Society B.361(1470). London:Royal Society:869–885.doi:10.1098/rstb.2006.1834.PMC1578735.PMID16754604.", 1},
    //     {"^Manhes, Gérard; Allègre, Claude J.; Dupré, Bernard; Hamelin, Bruno (May 1980). \"Lead isotope study of basic-ultrabasic layered complexes: Speculations about the age of the earth and primitive mantle characteristics\".Earth and Planetary Science Letters.47(3):370–382.Bibcode:1980E&amp;PSL..47..370M.doi:10.1016/0012-821X(80)90024-2.", 1},
    // };
    
    // Optimizer & Loss function
    torch::optim::Adam optimizer(model.parameters(), torch::optim::AdamOptions(0.000001));  // Adjusted learning rate
    auto criterion = torch::nn::BCELoss();
    
#include <algorithm>  // For std::shuffle
#include <random>     // For std::random_device and std::mt19937

// Training loop
std::random_device rd;
std::mt19937 g(rd());  // Random number generator

for (int epoch = 0; epoch < 1000; ++epoch) {
    float total_loss = 0;

    // Shuffle the dataset at the start of each epoch
    std::shuffle(labeledCitations.begin(), labeledCitations.end(), g);

    for (const auto& [citation, label] : labeledCitations) {
        torch::Tensor input = preprocess(citation, 500);  // Using 500 as the max length for input
        torch::Tensor target = torch::tensor({static_cast<float>(label)}).view({-1, 1});  // Correctly reshape target to [batch_size, 1]
        
        optimizer.zero_grad();
        torch::Tensor output = model.forward(input);
        torch::Tensor loss = criterion(output, target);
        loss.backward();
        optimizer.step();
        
        total_loss += loss.item<float>();
    }

    if (epoch % 10 == 0) std::cout << "Epoch " << epoch << " Loss: " << total_loss << std::endl;
}

    std::string testData = "^ostanza, Robert; d'Arge, Ralph; de Groot, Rudolf; Farber, Stephen; Grasso, Monica; Hannon, Bruce; Limburg, Karin; Naeem, Shahid; O'Neill, Robert V.; Paruelo, Jose; Raskin, Robert G.; Sutton, Paul; van den Belt, Marjan (May 1997).\"The value of the world's ecosystem services and natural capital\".Nature.387(6630):253–260.Bibcode:1997Natur.387..253C.doi:10.1038/387253a0."; // 1
    // std::string testData = "Raven, Peter H.;Johnson, George B.(2002).Biology(6th ed.). Boston, MA:McGraw-Hill. p. 68.ISBN0-07-112261-3.LCCN2001030052.OCLC45806501."; // 1
    // std::string testData = "Lambert, Jonathan (4 September 2020).\"Protecting half of Earth may help solve climate change, save species\".ScienceNews."; // 0
    torch::Tensor testInput = preprocess(testData, 500);  // Using 500 as the max length for input
    torch::Tensor estimation = model.forward(testInput);
    std::cout << "Expected 1 and got: " << estimation.item<float>() << std::endl;
    
    // Save model parameters
    torch::save(model.parameters(), "citation_model.pt");
    // std::cout << "Model saved!" << std::endl;

    return 0;
}
