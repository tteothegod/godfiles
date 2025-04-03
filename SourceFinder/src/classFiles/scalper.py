import sys
from bs4 import BeautifulSoup
import requests

def fetch_and_save_html(url, output_file="scraped0.html"):
    """Fetch HTML content from a URL, format it, and save it to a specified file."""
    try:
        response = requests.get(url)
        response.raise_for_status()

        # Parse and format HTML
        soup = BeautifulSoup(response.text, "html.parser")
        formatted_html = soup.prettify()

        # Save formatted HTML to the specified output file
        with open(output_file, "w", encoding="utf-8") as file:
            file.write(formatted_html)

        print(f"Formatted HTML saved to {output_file}")
        return output_file
    except requests.exceptions.RequestException as e:
        print(f"Error fetching {url}: {e}")
        return None

if __name__ == "__main__":
    if len(sys.argv) > 1:
        url = sys.argv[1]
        output_file = sys.argv[2] if len(sys.argv) > 2 else "scraped0.html"
        fetch_and_save_html(url, output_file)
    else:
        print("Usage: python fetch_and_save_html.py <URL> <output_file>")
