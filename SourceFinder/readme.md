# Source Finder W

## Overview
**Source Finder W** is a tool designed to help users quickly find and classify sources from Wikipedia. It scrapes and parses Wikipedia articles based on a search query and applies a neural network to determine whether each source is academically credible or not.

## Features
- **Web Scraping**: Uses Python and Beautiful Soup to scrape and parse Wikipedia's HTML for external sources.
- **Neural Network Classification**: A trained neural network (implemented with Libtorch) analyzes sources to differentiate academic from non-academic references.
- **Object-Oriented Design**: Implements OOP principles to create reusable classes for web scraping and HTML parsing.
- **Dataset Creation**: Includes a growing dataset to improve the model's accuracy in classifying sources.
- **Website Development**: Working towards integrating the tool into a functional website for broader accessibility.

## Technologies Used
- **C++** (Libtorch for neural network training)
- **Python** (Beautiful Soup for web scraping)
- **HTML Parsing** (Extracting links and source data)
- **Website Development** (In progress)

## Installation & Usage
### Prerequisites
- Python 3.x
- C++ Compiler (for Libtorch)
- Beautiful Soup (`pip install beautifulsoup4`)
- Libtorch (Follow installation guide: https://pytorch.org/cppdocs/installing.html)

### Running the Project
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/source-finder-w.git
   cd source-finder-w
   ```
2. Set up dependencies (Python and C++ libraries as required).
3. Compile the program: make DataPrep or make TrainModel (If needed use PYTORCH_LIB=/custom/path/to/libtorch/lib PYTORCH_INCLUDE=/custom/path/to/libtorch/include)
4. Process and classify sources using the neural network.

## Future Improvements
- Expand and refine the training dataset for better classification accuracy.
- Implement a web-based interface for easier user interaction.
- Optimize scraping efficiency and error handling.
- Improve model performance with additional deep learning techniques.

## Author
Matteo - University of California, Davis
