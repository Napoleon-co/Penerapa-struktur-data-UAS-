#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

// Struktur data lengkap sesuai deskripsi dataset Kaggle di dokumen Anda
struct CryptoData {
    string tanggal;
    float open, high, low, close, volume, marketCap;
};

unordered_map<string, vector<CryptoData>> cryptoMap;
const string fileName = "crypto_data.csv";

// --- FUNGSI MANAJEMEN FILE ---

void saveToFile() {
    ofstream file(fileName);
    if (file.is_open()) {
        for (auto const& [key, listData] : cryptoMap) {
            for (auto const& d : listData) {
                file << key << "," << d.tanggal << "," << d.open << "," 
                     << d.high << "," << d.low << "," << d.close << "," 
                     << d.volume << "," << d.marketCap << "\n";
            }
        }
        file.close();
    }
}

void loadFromFile() {
    ifstream file(fileName);
    string line, key, tgl, o, h, l, c, v, mc;
    if (!file.is_open()) return;
    cryptoMap.clear();
    while (getline(file, line)) {
        stringstream ss(line);
        if (getline(ss, key, ',') && getline(ss, tgl, ',') && 
            getline(ss, o, ',') && getline(ss, h, ',') && 
            getline(ss, l, ',') && getline(ss, c, ',') && 
            getline(ss, v, ',') && getline(ss, mc, ',')) {
            cryptoMap[key].push_back({tgl, stof(o), stof(h), stof(l), stof(c), stof(v), stof(mc)});
        }
    }
    file.close();
}

// --- FUNGSI SESUAI FLOWCHART HALAMAN 6-9 ---

void addData(string key, string tgl, float o, float h, float l, float c, float v, float mc) {
    cryptoMap[key].push_back({tgl, o, h, l, c, v, mc});
    saveToFile();
    cout << "\n[Sukses] Data " << key << " berhasil ditambahkan dan disimpan ke file.\n";
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
        sumClose += data[i].close;
    }

    cout << "\n--- STATISTIK " << key << " ---" << endl;
    cout << "Max High        : " << maxHigh << endl;
    cout << "Min Low         : " << minLow << endl;
    cout << "Rata-rata Close : " << fixed << setprecision(2) << (sumClose / size) << endl;
}

void removeCrypto(string key) {
    if (cryptoMap.find(key) != cryptoMap.end()) {
        cryptoMap.erase(key);
        saveToFile();
        cout << "[Sukses] Koin " << key << " berhasil dihapus dari sistem.\n";
    } else {
        cout << "[Error] data tidak ada\n";
    }
}

void printAll() {
    if (cryptoMap.empty()) {
        cout << "[Info] data tidak ada\n";
        return;
    }
    cout << "\n=== DAFTAR SEMUA DATA CRYPTO ===";
    for (auto const& [key, listData] : cryptoMap) {
        cout << "\nKoin: " << key;
        for (auto const& d : listData) {
            cout << "\n  > Tgl: " << d.tanggal << " | Open: " << d.open << " | High: " << d.high 
                 << " | Low: " << d.low << " | Close: " << d.close;
        }
        cout << endl;
    }
}

// --- MAIN PROGRAM ---

int main() {
    loadFromFile();
    int pil;

    while (true) {
        cout << "\n===============================";
        cout << "\n   MENU KELOLA CRYPTO (UAS)";
        cout << "\n===============================";
        cout << "\n1. Input Data Baru";
        cout << "\n2. Hitung Statistik (Max/Min/Avg)";
        cout << "\n3. Hapus Koin";
        cout << "\n4. Tampilkan Semua Koin";
        cout << "\n0. Keluar";
        cout << "\nPilih Menu: ";
        
        if (!(cin >> pil)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[Error] Masukkan angka!\n";
            continue;
        }

        if (pil == 0) break;

        string k, t;
        float o, h, l, c, v, mc;

        switch (pil) {
            case 1:
                cout << "Simbol Koin: "; cin >> k;
                cout << "Tanggal: "; cin >> t;
                cout << "Open: "; cin >> o;
                cout << "High: "; cin >> h;
                cout << "Low: "; cin >> l;
                cout << "Close: "; cin >> c;
                cout << "Volume: "; cin >> v;
                cout << "Market Cap: "; cin >> mc;
                addData(k, t, o, h, l, c, v, mc);
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