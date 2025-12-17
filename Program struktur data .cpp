#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

struct CryptoData {
    string tanggal;
    float high, low, close;
};

unordered_map<string, vector<CryptoData>> cryptoMap;
const string fileName = "crypto_data.csv";

// --- FUNGSI MANAJEMEN FILE ---

void saveToFile() {
    ofstream file(fileName);
    if (file.is_open()) {
        for (auto const& [key, listData] : cryptoMap) {
            for (auto const& d : listData) {
                file << key << "," << d.tanggal << "," << d.high << "," 
                     << d.low << "," << d.close << "\n";
            }
        }
        file.close();
    }
}

void loadFromFile() {
    ifstream file(fileName);
    string line, key, tgl, h, l, c;
    if (!file.is_open()) return;
    cryptoMap.clear();
    while (getline(file, line)) {
        stringstream ss(line);
        if (getline(ss, key, ',') && getline(ss, tgl, ',') && 
            getline(ss, h, ',') && getline(ss, l, ',') && getline(ss, c, ',')) {
            cryptoMap[key].push_back({tgl, stof(h), stof(l), stof(c)});
        }
    }
    file.close();
}

// --- FUNGSI UTAMA SESUAI FLOWCHART UAS ---

void addData(string key, string tgl, float h, float l, float c) {
    cryptoMap[key].push_back({tgl, h, l, c});
    saveToFile();
    cout << "\n[Sukses] Data " << key << " berhasil disimpan ke RAM dan File.\n";
}

void calculateStatistics(string key) {
    if (cryptoMap.find(key) == cryptoMap.end()) {
        cout << "[Error] data tidak ada\n";
        return;
    }
    vector<CryptoData>& data = cryptoMap[key];
    int size = data.size();
    if (size == 0) {
        cout << "[Error] data kosong\n";
        return;
    }

    float maxHigh = data[0].high;
    float minLow = data[0].low;
    float sumClose = 0;

    for (int i = 0; i < size; i++) {
        if (data[i].high > maxHigh) maxHigh = data[i].high;
        if (data[i].low < minLow) minLow = data[i].low;
        sumClose = sumClose + data[i].close;
    }

    float ratarata_close = sumClose / size;
    cout << "\n--- STATISTIK " << key << " ---" << endl;
    cout << "Max High        : " << maxHigh << endl;
    cout << "Min Low         : " << minLow << endl;
    cout << "Rata-rata Close : " << fixed << setprecision(2) << ratarata_close << endl;
}

void removeCrypto(string key) {
    if (cryptoMap.find(key) != cryptoMap.end()) {
        cryptoMap.erase(key);
        saveToFile();
        cout << "[Sukses] Semua data " << key << " telah dihapus.\n";
    } else {
        cout << "[Error] data tidak ada\n";
    }
}

void printAll() {
    if (cryptoMap.empty()) {
        cout << "[Info] data tidak ada\n";
        return;
    }
    cout << "\nDAFTAR KOIN TERSEDIA:\n";
    for (auto const& [key, val] : cryptoMap) 
        cout << "- " << key << " (" << val.size() << " record)" << endl;
}



int main() {
    loadFromFile();
    int pil;

    while (true) {
        cout << "\n=== MENU KELOLA CRYPTO (UAS) ===";
        cout << "\n1. Input Data Baru";
        cout << "\n2. Hitung Statistik (Max/Min/Avg)";
        cout << "\n3. Hapus Koin";
        cout << "\n4. Tampilkan Semua Koin";
        cout << "\n0. Keluar";
        cout << "\nPilih Menu: ";
        
        
        if (!(cin >> pil)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[Error] Masukkan angka 0-4!\n";
            continue;
        }

        if (pil == 0) break;

        string k, t;
        float h, l, c;

        switch (pil) {
            case 1:
                cout << "Simbol Koin (misal BTC): "; cin >> k;
                cout << "Tanggal: "; cin >> t;
                cout << "High: "; cin >> h;
                cout << "Low: "; cin >> l;
                cout << "Close: "; cin >> c;
                addData(k, t, h, l, c);
                break;
            case 2:
                cout << "Masukkan Simbol Koin: "; cin >> k;
                calculateStatistics(k);
                break;
            case 3:
                cout << "Masukkan Simbol Koin: "; cin >> k;
                removeCrypto(k);
                break;
            case 4:
                printAll();
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    }
    return 0;
}
