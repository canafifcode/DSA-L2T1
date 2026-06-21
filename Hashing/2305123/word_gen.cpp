// random word generator
#include <bits/stdc++.h>
using namespace std;

string generate_word(int length)
{
    string word = "";
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < length; i++)
    {
        word += alphabet[rand() % 26];
    }
    return word;
}

int main()
{
    srand(time(0));  // Seed for random number generation
    int num_words;   // Number of words to generate
    int word_length; // Length of each word

    cin >> word_length;
    cin >> num_words;

   set<string> seenWords;
    
    // Pair: <Word (Key), Sequence Number (Value)>
    vector<pair<string, int>> keyValuePairs;

    int sequenceNumber = 1;

    while (keyValuePairs.size() < num_words) {
        string word = generate_word(word_length);

        // Check if the word is unique 
        if (seenWords.find(word) == seenWords.end()) {
            seenWords.insert(word);
            
            //The sequence number is the associated value 
            keyValuePairs.push_back({word, sequenceNumber});
            sequenceNumber++;
        }
    }

    // Output the generated words with their sequence numbers
    for (const auto& pair : keyValuePairs) {
        cout << pair.first << " " << pair.second << endl;
    }
    return 0;
}