#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "json.hpp" // nlohmann::json
#include <fstream>
using namespace std;
using json = nlohmann::json;

class DSU {
private:
    vector<int> sz, parent;
public:
    DSU(int n) {
        sz.resize(n, 1);
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find_set(int u) {
        return parent[u] == u ? u : parent[u] = find_set(parent[u]);
    }

    void union_set(int u, int v) {
        int a = find_set(u);
        int b = find_set(v);
        if (a != b) {
            if (sz[a] < sz[b]) swap(a, b);
            sz[a] += sz[b];
            parent[b] = a;
        }
    }
};

bool is_same_row(const json& a, const json& b) {
    int topA = a["y"], bottomA = a["y"].get<int>() + a["height"].get<int>();
    int topB = b["y"], bottomB = b["y"].get<int>() + b["height"].get<int>();
    return !(bottomA < topB || bottomB < topA); // Overlapping rows
}

json reorderJsonByRow(const json& frames) {
    int n = frames.size();
    DSU dsu(n);

    // Step 1: Union sets by row
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (is_same_row(frames[i], frames[j]))
                dsu.union_set(i, j);

    // Step 2: Group by DSU root
    unordered_map<int, vector<int>> groups;
    for (int i = 0; i < n; ++i) {
        groups[dsu.find_set(i)].push_back(i);
    }

    // Step 3: Extract and sort groups
    vector<vector<int>> groupedIndices;
    for (auto& group : groups) {
        sort(group.second.begin(), group.second.end(), [&](int i, int j) {
            return frames[i]["x"] < frames[j]["x"];
            });
        groupedIndices.push_back(group.second);
    }

    // Step 4: Sort groups top to bottom
    sort(groupedIndices.begin(), groupedIndices.end(), [&](const vector<int>& a, const vector<int>& b) {
        return frames[a[0]]["y"] < frames[b[0]]["y"];
        });

    // Step 5: Reconstruct output JSON
    json output = json::array();
    for (const auto& group : groupedIndices)
        for (int idx : group)
            output.push_back(frames[idx]);

    return output;
}



json loadJson(const string& path) {
    ifstream in(path);
    json j;
    in >> j;
    return j;
}

void saveJson(const string& path, const json& j) {
    ofstream out(path);
    out << j.dump(4);
}

json getProcessedSpriteJson(const string& inputPath) {
    cout << "Processing sprite JSON...\n";
    json original = loadJson(inputPath);
    json reordered = reorderJsonByRow(original);
    saveJson(inputPath, reordered);
    return reordered;
}

