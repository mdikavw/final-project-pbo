#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <random>

class Item{
    public:
        int idItem;
        std::string nama;
        int harga;
        Item(int x, std::string y, int z):idItem(x), nama(y), harga(z){};
};

class Menu{
    public:
        std::vector<Item> daftarMenu;

    Menu(){
        int switcher = 1;
        int id = 1;
        std::string menuString;
        std::ifstream menuFile("main.txt");
        while(std::getline(menuFile, menuString)){
            std::string namaHarga;
            std::string nama;
            std::string harga;
            std::istringstream iss(menuString);
            while(std::getline(iss, namaHarga, ',')){
                if(switcher == 1){
                    nama = namaHarga;
                    switcher++;
                } else if(switcher == 2){
                    harga = namaHarga;
                    switcher--;
                }
            }
            Item newItem(id, nama, stoi(harga));
            daftarMenu.push_back(newItem);
            id++;
        }
        menuFile.close();
    }
    
    void printMenu(){
        for(int i = 0; i < daftarMenu.size(); i++){
            std::cout<< daftarMenu[i].idItem << " " << daftarMenu[i].nama << " --- " << daftarMenu[i].harga << std::endl;
        }
    }
};

class Pelanggan{
    public:
        int idPelanggan;
        std::vector<Item> pesanan;
    Pelanggan(int x):idPelanggan(x){};
    
    void tambahPesanan(Menu menu){
        int idItem;
        char pilihan;
        do{
            system("CLS");
            menu.printMenu();
            std::cout << "Pilih pesanan untuk pelanggan " << idPelanggan << ": " << std::endl;
            std::cin >> idItem;
            pesanan.push_back(menu.daftarMenu[idItem - 1]);
            std::cout << "Apakah pelanggan " << idPelanggan << " ingin menambah pesanan?(y/n): ";
            std::cin >> pilihan;
        } while(pilihan == 'y');
    }
};

class Nota{
    public:
        int idNota;
        int idPemesanan;
        int total = 0;
        std::vector<Item> daftarPesanan;
        std::vector<int> jumlahPesanan;
    Nota(int x, int y):idNota(x), idPemesanan(y){}

    void printNota(){
        std::ofstream fileNota;
        fileNota.open("nota_" + std::to_string(idPemesanan) + ".txt", std::fstream::app);
        for(int i = 0; i < daftarPesanan.size() - 1; i++){
            for(int j = i + 1; j < daftarPesanan.size(); j++){
                if(daftarPesanan[i].idItem == daftarPesanan[j].idItem){
                    daftarPesanan.erase(daftarPesanan.begin() + j);
                    jumlahPesanan.erase(jumlahPesanan.begin() + j);
                    jumlahPesanan[i]++;
                    j--;
                }
            }
        }
        for(int i = 0; i < daftarPesanan.size(); i++){
            fileNota << daftarPesanan[i].nama << std::endl;
            std::cout << daftarPesanan[i].nama << std::endl;
            fileNota << "x" << jumlahPesanan[i] << "\t\t\t" << daftarPesanan[i].harga*jumlahPesanan[i]<< std::endl << std::endl;
            std::cout << "x" << jumlahPesanan[i] << "\t\t\t" << daftarPesanan[i].harga*jumlahPesanan[i]<< std::endl << std::endl;
        }
        fileNota << std::endl << "Total:\t\t\t" << total << std::endl;
        std::cout << std::endl << "Total:\t\t\t" << total << std::endl;
    }
    void generateDaftarPesanan(std::vector<Pelanggan> pelanggan){
        for(int i = 0; i < pelanggan.size(); i++){
            for(int j = 0; j < pelanggan[i].pesanan.size(); j++){
                daftarPesanan.push_back(pelanggan[i].pesanan[j]);
            }
        }
        for(int i = 0; i < daftarPesanan.size(); i++){
            jumlahPesanan.push_back(1);
        }
    }

    void printHeader(int x){
        std::ofstream fileNota;
        fileNota.open("nota_" + std::to_string(idPemesanan) + ".txt", std::fstream::app);
        fileNota << "----------Nota----------" << std::endl;
        std::cout << "----------Nota----------" << std::endl;
        fileNota << "Id Pemesanan: " << idPemesanan << std::endl;
        std::cout << "Id Pemesanan: " << idPemesanan << std::endl;
        fileNota << "Id Meja: " << idNota << std::endl;
        std::cout << "Id Meja: " << idNota << std::endl;
        fileNota << "Jumlah pelanggan: " << x << std::endl;
        std::cout << "Jumlah pelanggan: " << x << std::endl;
        fileNota << std::endl << std::endl;        
        std::cout << std::endl << std::endl;        
    }

    void hitungTotal(std::vector<Pelanggan> pelanggan){
        for(int i = 0; i < pelanggan.size(); i++){
            for(int j = 0; j < pelanggan[i].pesanan.size(); j++){
                total = total + pelanggan[i].pesanan[j].harga;
            }
        }
    }
};

class NotaIndividu : public Nota{
    public:
    int idNotaIndividu;
    NotaIndividu(int x, int y):Nota(x, y){
        idNotaIndividu = x;
    }
    void generateJumlahPesanan(std::vector<Item> pesanan){
        for(int i = 0; i < pesanan.size(); i++){
            jumlahPesanan.push_back(1);
        }
    }
    void printHeader(int x, int y){
        std::ofstream fileNota;
        fileNota.open("nota_" + std::to_string(idPemesanan) + ".txt", std::fstream::app); 
        fileNota << "----------Nota----------" << std::endl;
        std::cout << "----------Nota----------" << std::endl;
        fileNota << "Id Pemesanan: " << idPemesanan << std::endl;
        std::cout << "Id Pemesanan: " << idPemesanan << std::endl;
        fileNota << "Id Meja: " << x << std::endl;
        std::cout << "Id Meja: " << x << std::endl;
        fileNota << "Id Pelanggan: " << y << std::endl;
        std::cout << "Id Pelanggan: " << y << std::endl;
        fileNota << std::endl << std::endl;        
        std::cout << std::endl << std::endl;
        fileNota.close();        
    }
    void printNota(std::vector<Item> pesanan){
        std::ofstream fileNota;
        fileNota.open("nota_" + std::to_string(idPemesanan) + ".txt", std::fstream::app);
        for(int i = 0; i < pesanan.size() - 1; i++){
            for(int j = i + 1; j < pesanan.size(); j++){
                if(pesanan[i].idItem == pesanan[j].idItem){
                    pesanan.erase(pesanan.begin() + j);
                    jumlahPesanan.erase(jumlahPesanan.begin() + j);
                    jumlahPesanan[i]++;
                    j--;
                }
            }
        }
        for(int i = 0; i < pesanan.size(); i++){
            fileNota << pesanan[i].nama << std::endl;
            std::cout << pesanan[i].nama << std::endl;
            fileNota << "x" << jumlahPesanan[i] << "\t\t\t" << pesanan[i].harga*jumlahPesanan[i]<< std::endl << std::endl;
            std::cout << "x" << jumlahPesanan[i] << "\t\t\t" << pesanan[i].harga*jumlahPesanan[i]<< std::endl << std::endl;
        }
        fileNota << std::endl << "Total:\t\t\t" << total << std::endl << std::endl;
        std::cout << std::endl << "Total:\t\t\t" << total << std::endl << std::endl;
        fileNota.close();
    }

    void hitungTotal(std::vector<Item> pesanan){
        for(int i = 0; i < pesanan.size(); i++){
            total = total + pesanan[i].harga;
        }
    }
};

class Meja{
    public:
        int jumlahPelanggan;
        int idMeja;
        int idPemesanan;
        std::vector<Pelanggan> pelanggan;

    Meja(int x, int y, int z ,Menu menu):idMeja(x), jumlahPelanggan(y), idPemesanan(z){
        for(int i = 0; i < jumlahPelanggan; i++){
            tambahPelanggan(i + 1, menu);
        }
    }

    void tambahPelanggan(int id, Menu menu){
        Pelanggan newPelanggan(id);
        newPelanggan.tambahPesanan(menu);
        pelanggan.push_back(newPelanggan);
    }

    ~Meja(){
        Nota nota(idMeja, idPemesanan);
        nota.hitungTotal(pelanggan);
        nota.generateDaftarPesanan(pelanggan);
        nota.printHeader(jumlahPelanggan);
        nota.printNota();
        std::cout << std::endl;
        for(int i = 0; i < pelanggan.size(); i++){
            NotaIndividu newNotaIndividu(i + 1, idPemesanan);
            newNotaIndividu.hitungTotal(pelanggan[i].pesanan);
            newNotaIndividu.generateJumlahPesanan(pelanggan[i].pesanan);
            newNotaIndividu.printHeader(idMeja, i + 1);
            newNotaIndividu.printNota(pelanggan[i].pesanan);
        }
        system("PAUSE");
    }
    
};


int randomizer() {
    // Use a random device as the seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the distribution for 6-digit numbers
    std::uniform_int_distribution<int> dist(100000, 999999);

    // Generate a random number
    int randomNumber = dist(gen);

    return randomNumber;
}


int main(){
    //-----------------------Inisialisasi menu
    Menu menu;
    int pilihan;
    std::cout << "1. Tambah meja" << std::endl;
    std::cout << "2. Keluar" << std::endl;
    std::cout << "Masukkan pilihan: ";
    std::cin >> pilihan;
    switch(pilihan){
        case 1:
        {
            int idMeja;
            int jumlahPelanggan;
            std::cout << "Masukkan id meja: ";
            std:: cin >> idMeja;
            std::cout << "Masukkan jumlah pelanggan: ";
            std::cin >> jumlahPelanggan;
            Meja meja(idMeja, jumlahPelanggan, randomizer(), menu);
            break;
        }
        case 2:
        {
            std::cout << "Program selesai" << std::endl;
            break;
        }
        default:
        {
        std::cout << "Pilihan tidak tersedia" << std::endl;
        break;
        }
    }

    
    return 0;
}