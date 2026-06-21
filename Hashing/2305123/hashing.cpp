#include <bits/stdc++.h>
using namespace std;

const int INITIAL_TABLE_SIZE = 13;
const double MAX_LOAD_FACTOR = 0.5;
const double MIN_LOAD_FACTOR = 0.25;

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

        // input c1 and c2 for custom probing
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

                // empty
                if (!table[idx].isOccupied)
                    return nullptr;

                // ooccupied but deleted
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
                    break; // notfnd

                if (!table[idx].isDeleted && table[idx].key == key)
                {
                    table[idx].isDeleted = true; // lz
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
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    vector<pair<string, int>> allData;
    string k;
    int v;
    while (cin >> k >> v)
        allData.push_back({k, v});
    if (allData.empty())
        return 0;

    vector<string> searchKeys;
    srand(123);
    for (int i = 0; i < 1000; i++)
        searchKeys.push_back(allData[rand() % allData.size()].first);

    struct
    {
        long long col1, col2;
        double hit1, hit2;
    } rows[3];

    string names[] = {"Chaining Method", "Double Hashing", "Custom Probing"};

    for (int m = 0; m < 3; m++)
    {
        for (int h = 1; h <= 2; h++)
        {
            HashTable<string, int> ht((typename HashTable<string, int>::CollisionMethod)m, h);
            for (auto &kv : allData)
                ht.insert(kv.first, kv.second);

            long long hits = 0;

            for (auto &sk : searchKeys)
                hits += ht.searchProbes(sk);

            if (h == 1)
            {
                rows[m].col1 = ht.getTotalCollisions();
                rows[m].hit1 = (double)hits / 1000;
            }
            else
            {
                rows[m].col2 = ht.getTotalCollisions();
                rows[m].hit2 = (double)hits / 1000;
            }
        }
    }

    cout << "\n"
         << setfill('-') << setw(95) << "-" << endl
         << setfill(' ');
    cout << left << setw(20) << "Method" << " | " << setw(33) << "Hash 1" << " | " << setw(33) << "Hash 2" << " |" << endl;
    cout << setw(20) << "" << " | " << string(33, '-') << " | " << string(33, '-') << " |" << endl;
    cout << left << setw(20) << "" << " | " << "Collisions" << setw(10) << "" << "Avg Hits" << "      | " << "Collisions" << setw(10) << "" << "Avg Hits" << "      |" << endl;
    cout << setfill('-') << setw(95) << "-" << endl
         << setfill(' ');

    for (int i = 0; i < 3; i++)
    {
        cout << left << setw(20) << names[i] << " |" << right << setw(15) << rows[i].col1 << " |" << fixed << setprecision(3) << setw(15) << rows[i].hit1
             << " |" << setw(18) << rows[i].col2 << " |" << setw(15) << rows[i].hit2 << " |" << endl;
    }

    cout << setfill('-') << setw(90) << "-" << endl
         << setfill(' ');

    return 0;
}

/*
# Compile
g++ word_gen.cpp -o word_gen
g++ hashing.cpp -o hashing

# Run (Piping in PowerShell)
"10`n10000" | ./word_gen | ./hashing
*/