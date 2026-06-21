#include <bits/stdc++.h>
using namespace std;

// Constants for Hash Table behavior
const int INITIAL_TABLE_SIZE = 13;
const double MAX_LOAD_FACTOR = 0.5;
const double MIN_LOAD_FACTOR = 0.25;

// --- Function from word_gen.cpp ---
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

// --- Template Class from hashing.cpp ---
template <typename K, typename V>
class HashTable
{
public:
    enum CollisionMethod
    {
        CHAINING,
        DOUBLE_HASHING,
        CUSTOM_PROBING
    };

private:
    struct DataEntry
    {
        K key;
        V value;
        bool isOccupied = false;
        bool isDeleted = false;
    };

    vector<DataEntry> table;
    vector<list<pair<K, V>>> chains;

    int currentSize, tableSize, hashchoice;
    CollisionMethod method;

    int n_at_last_resize;
    int insertions_since_resize;
    int deletions_since_resize;
    int C1, C2;
    long long totalCollisions = 0;

    int hash1(const K &key)
    {
        string s = (string)key;
        long long h = 0;
        for (char c : s)
            h = (h * 31 + c) % tableSize;
        return (int)h;
    }

    int hash2(const K &key)
    {
        string s = (string)key;
        unsigned long long h = 14695981039346656037ULL;
        for (char c : s)
        {
            h ^= c;
            h *= 1099511628211ULL;
        }
        return (int)(h % tableSize);
    }

    int auxProbe(const K &key)
    {
        string s = (string)key;
        int h = 0;
        for (char c : s)
            h += c;
        return 7 - (h % 7);
    }

    int getPrimaryHash(const K &key)
    {
        return (hashchoice == 1) ? hash1(key) : hash2(key);
    }

    int getProbeIndex(const K &key, int i, int primaryHash)
    {
        int aux = auxProbe(key);
        if (method == DOUBLE_HASHING)
            return (int)((primaryHash + (long long)i * aux) % tableSize);
        if (method == CUSTOM_PROBING)
            return (int)((primaryHash + (long long)C1 * i * aux + (long long)C2 * i * i) % tableSize);
        return primaryHash % tableSize;
    }

    bool isPrime(int n)
    {
        if (n <= 1)
            return false;
        for (int i = 2; i * i <= n; i++)
            if (n % i == 0)
                return false;
        return true;
    }

    int nextPrime(int n)
    {
        while (!isPrime(n))
            n++;
        return n;
    }

    int prevPrime(int n)
    {
        n--;
        while (n > INITIAL_TABLE_SIZE && !isPrime(n))
            n--;
        return n;
    }

public:
    HashTable(CollisionMethod method, int hashchoice) : method(method), hashchoice(hashchoice)
    {
        tableSize = INITIAL_TABLE_SIZE;
        currentSize = 0;
        n_at_last_resize = 0;
        insertions_since_resize = 0;
        deletions_since_resize = 0;

        C1 = 3;
        C2 = 5;

        if (method == CHAINING)
            chains.resize(tableSize);
        else
            table.resize(tableSize);
    }

    long long getTotalCollisions()
    {
        return totalCollisions;
    }

    int getTableSize()
    {
        return tableSize;
    }

    int getSize()
    {
        return currentSize;
    }

    void insert(K key, V value)
    {
        int h = getPrimaryHash(key);
        if (method == CHAINING)
        {
            if (!chains[h].empty())
                totalCollisions++;
            for (auto &entry : chains[h])
                if (entry.first == key)
                    return;
            chains[h].push_back({key, value});
        }
        else
        {
            // Logic: Increment collision for every failed probe (4 increments if 5 probes used)
            for (int i = 0; i < tableSize; i++)
            {
                int idx = getProbeIndex(key, i, h);
                if (table[idx].isOccupied && table[idx].key != key)
                    totalCollisions++;
                if (!table[idx].isOccupied || table[idx].isDeleted)
                {
                    table[idx] = {key, value, true, false};
                    break;
                }
                if (table[idx].key == key && !table[idx].isDeleted)
                    return;
            }
        }
        currentSize++;
        insertions_since_resize++;
        checkLoadFactorAndRehash();
    }

    int searchProbes(K key)
    {
        int probes = 0, h = getPrimaryHash(key);
        if (method == CHAINING)
        {
            probes++;
            for (auto &entry : chains[h])
            {
                if (entry.first == key)
                    return probes;
                probes++;
            }
        }
        else
        {
            for (int i = 0; i < tableSize; i++)
            {
                int idx = getProbeIndex(key, i, h);
                probes++;
                if (!table[idx].isOccupied)
                    break;
                if (!table[idx].isDeleted && table[idx].key == key)
                    return probes;
            }
        }
        return probes;
    }

    void checkLoadFactorAndRehash()
    {
        double lf = (double)currentSize / tableSize;
        if (lf > MAX_LOAD_FACTOR && (n_at_last_resize == 0 || insertions_since_resize >= n_at_last_resize / 2))
        {
            rehash(nextPrime(2 * tableSize));
        }
        else if (lf < MIN_LOAD_FACTOR && tableSize > INITIAL_TABLE_SIZE && deletions_since_resize >= n_at_last_resize / 2)
        {
            rehash(prevPrime(tableSize / 2));
        }
    }

    void rehash(int newSize)
    {
        int oldSize = tableSize;
        auto oldTable = table;
        auto oldChains = chains;
        tableSize = newSize;
        n_at_last_resize = currentSize;
        insertions_since_resize = deletions_since_resize = currentSize = 0;
        if (method == CHAINING)
        {
            chains.assign(tableSize, list<pair<K, V>>());
            for (int i = 0; i < oldSize; i++)
                for (auto &e : oldChains[i])
                    insert(e.first, e.second);
        }
        else
        {
            table.assign(tableSize, DataEntry());
            for (int i = 0; i < oldSize; i++)
                if (oldTable[i].isOccupied && !oldTable[i].isDeleted)
                    insert(oldTable[i].key, oldTable[i].value);
        }
    }

    V *search(K key)
    {
        int h = getPrimaryHash(key);
        if (method == CHAINING)
        {
            for (auto &entry : chains[h])
            {
                if (entry.first == key)
                    return &entry.second;
            }
        }
        else
        {
            for (int i = 0; i < tableSize; i++)
            {
                int idx = getProbeIndex(key, i, h);
                if (!table[idx].isOccupied)
                    return nullptr;
                if (!table[idx].isDeleted && table[idx].key == key)
                {
                    return &table[idx].value;
                }
            }
        }
        return nullptr;
    }

    void remove(K key)
    {
        int h = getPrimaryHash(key);
        bool found = false;
        if (method == CHAINING)
        {
            auto &lst = chains[h];
            for (auto it = lst.begin(); it != lst.end(); ++it)
            {
                if (it->first == key)
                {
                    lst.erase(it);
                    found = true;
                    break;
                }
            }
        }
        else
        {
            for (int i = 0; i < tableSize; i++)
            {
                int idx = getProbeIndex(key, i, h);
                if (!table[idx].isOccupied)
                    break;
                if (!table[idx].isDeleted && table[idx].key == key)
                {
                    table[idx].isDeleted = true;
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            currentSize--;
            deletions_since_resize++;
            checkLoadFactorAndRehash();
        }
    }

    void printProbeSequence(K key)
    {
        // only for double hashing and custom probing
        if (method == CHAINING)
        {
            cout << "Probe sequence not applicable for chaining method." << endl;
            return;
        }
        int h = getPrimaryHash(key);

        cout << "Probe sequence for key '" << key << "': ";
        for (int i = 0; i < tableSize; i++)
        {
            int idx = getProbeIndex(key, i, h);
            cout << idx << "->";
            if (!table[idx].isOccupied)
                break;
            if (!table[idx].isDeleted && table[idx].key == key)
                break;
        }
        cout << endl;
    }

    void getChainStats()
    {
        if (method != CHAINING)
        {
            cout << "Chain stats not applicable for non-chaining method." << endl;
            return;
        }

        // number of empty buckets, max chain length, average chain length of non-empty buckets
        int emptyBuckets = 0, maxChainLength = 0, totalChainLength = 0, nonEmptyBuckets = 0;
        for (auto &chain : chains)
        {
            if (chain.empty())
            {
                emptyBuckets++;
            }
            else
            {
                int len = chain.size();
                totalChainLength += len;
                nonEmptyBuckets++;
                maxChainLength = max(maxChainLength, len);
            }
        }

        double avgChainLength = nonEmptyBuckets > 0 ? (double)totalChainLength / nonEmptyBuckets : 0;
        cout << "Empty Buckets: " << emptyBuckets << endl;
        cout << "Max Chain Length: " << maxChainLength << endl;
        cout << "Average Chain Length (non-empty buckets): " << avgChainLength << endl;
    }

    void analyzeSearchWithDeletion()
    {
        if (method == CHAINING)
        {
            cout << "Analysis valid only for Open Addressing." << endl;
            return;
        }

        // Analyze search performance with deletions
        K Keyblocker = "", keyTarget = "";
        bool foundPair = false;
        int blockerIdx = -1;
    }

    void identifyClusters()
    {
        if (method == CHAINING)
        {
            cout << "Clustering analysis not applicable for chaining method." << endl;
            return;
        }

        // Identify and analyze clusters in the hash table
        vector<int> clusterSizes;
        int currentClusterSize = 0;

        for (int i = 0; i < tableSize; i++)
        {
            if (table[i].isOccupied && !table[i].isDeleted)
            {
                currentClusterSize++;
            }
            else
            {
                if (currentClusterSize > 0)
                {
                    clusterSizes.push_back(currentClusterSize);
                    currentClusterSize = 0;
                }
            }
        }
        // Check for cluster at the end of the table
        if (currentClusterSize > 0)
            clusterSizes.push_back(currentClusterSize);

        cout << "Identified Clusters: " << clusterSizes.size() << endl;
        if (!clusterSizes.empty())
        {
            cout << "Max Cluster Size: " << *max_element(clusterSizes.begin(), clusterSizes.end()) << endl;
            cout << "Average Cluster Size: " << accumulate(clusterSizes.begin(), clusterSizes.end(), 0.0) / clusterSizes.size() << endl;
        }
    }
};

// --- Combined Main Logic ---
int main()
{
    srand(time(0));
    int word_length = 10, num_words = 10000;

    // 1. Data Generation
    set<string> seenWords;
    vector<pair<string, int>> allData;
    int seq = 1;
    while (allData.size() < num_words)
    {
        string word = generate_word(word_length);
        if (seenWords.find(word) == seenWords.end())
        {
            seenWords.insert(word);
            allData.push_back({word, seq++});
        }
    }

    // 2. Demonstration: Building the Tables
    // Using Double Hashing (Method 1) and Custom Probing (Method 2) for demo
    HashTable<string, int> dh_table(HashTable<string, int>::DOUBLE_HASHING, 1);
    HashTable<string, int> cp_table(HashTable<string, int>::CUSTOM_PROBING, 1);
    HashTable<string, int> chain_h1(HashTable<string, int>::CHAINING, 1);
    HashTable<string, int> chain_h2(HashTable<string, int>::CHAINING, 2);
    /*
        cout << left << setw(15) << "Inserted"
             << setw(15) << "Load Factor"
             << setw(20) << "Total Collisions"
             << setw(15) << "Table Size" << endl;
        cout << string(65, '-') << endl;
    */
    int i = 0;
    for (auto &kv : allData)
    {
        dh_table.insert(kv.first, kv.second);
        /*
                if ((i + 1) % 1000 == 0) {
                    // We use a public helper or access internal logic to calculate stats
                    double currentLF = (double)dh_table.getSize() / dh_table.getTableSize();

                    cout << left << setw(15) << (i + 1)
                         << setw(15) << fixed << setprecision(3) << currentLF
                         << setw(20) << dh_table.getTotalCollisions()
                         << setw(15) << dh_table.getTableSize() << endl;
                }

                cp_table.insert(kv.first, kv.second);

                chain_h1.insert(kv.first, kv.second);
                chain_h2.insert(kv.first, kv.second);
                i++;
            }
        */
        cout << "Finished inserting 10,000 words.\n";

        // Generate the distribution report
        chain_h1.getChainStats();
        chain_h2.getChainStats();

        cout << endl;
        dh_table.identifyClusters();

        /*
            // 3. User Interaction for Probe Sequences
            int n;
            cout << "\nEnter the number of keys to check (n): ";
            if (!(cin >> n)) return 0;

            for (int i = 0; i < n; i++)
            {
                string key;
                cout << "Enter key " << i + 1 << ": ";
                cin >> key;

                cout << "\n[Double Hashing]" << endl;
                dh_table.printProbeSequence(key);

                cout << "[Custom Probing]" << endl;
                cp_table.printProbeSequence(key);
                cout << "---------------------------------------------------" << endl;
            }
        */

        return 0;
    }