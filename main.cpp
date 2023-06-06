#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

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
        int total = 0;
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
            total = total + menu.daftarMenu[idItem - 1].harga;
            std::cout << "Apakah pelanggan " << idPelanggan << " ingin menambah pesanan?(y/n): ";
            std::cin >> pilihan;
        } while(pilihan == 'y');
    }
};

class Nota{
    private:
        std::vector<Item> daftarPesanan;
    public:
        int id;
        int total;
        std::vector<int> jumlahPesanan;
    Nota(int x, int y):id(x), total(y){}

    void printNota(){
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
            std::cout << daftarPesanan[i].nama << std::endl;
            std::cout << "x" << jumlahPesanan[i] << "\t\t\t" << daftarPesanan[i].harga*jumlahPesanan[i]<< std::endl << std::endl;
        }
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
        std::cout << "----------Nota----------" << std::endl;
        std::cout << "Id Meja: " << id << std::endl;
        std::cout << "Jumlah pelanggan: " << x << std::endl;
        std::cout << std::endl << std::endl;        
    }
};

class NotaIndividu : public Nota{
    public:
    int id;
    int total;
    NotaIndividu(int x, int y):Nota(x, y){
        id = x;
        total = y;
    }
    void generateJumlahPesanan(std::vector<Item> pesanan){
        for(int i = 0; i < pesanan.size(); i++){
            jumlahPesanan.push_back(1);
        }
    }
    void printHeader(int x, int y){
        std::cout << "----------Nota----------" << std::endl;
        std::cout << "Id Meja: " << x << std::endl;
        std::cout << "Id Pelanggan: " << y << std::endl;
        std::cout << std::endl << std::endl;        
    }
    void printNota(std::vector<Item> pesanan){
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
            std::cout << pesanan[i].nama << std::endl;
            std::cout << "x" << jumlahPesanan[i] << "\t\t\t" << pesanan[i].harga*jumlahPesanan[i]<< std::endl << std::endl;
        }
        std::cout << std::endl << "Total:\t\t\t" << total << std::endl;
    }
};

class Meja{
    public:
        int jumlahPelanggan;
        int id = 1;
        int total = 0;
        std::vector<Pelanggan> pelanggan;

    Meja(int x, int y, Menu menu):id(x), jumlahPelanggan(y){
        for(int i = 0; i < jumlahPelanggan; i++){
            tambahPelanggan(i + 1, menu);
        }
        total = hitungTotal();
    }

    void tambahPelanggan(int id, Menu menu){
        Pelanggan newPelanggan(id);
        newPelanggan.tambahPesanan(menu);
        pelanggan.push_back(newPelanggan);
    }

    int hitungTotal(){
        for(int i = 0; i < pelanggan.size(); i++){
            total = total + pelanggan[i].total;
        }
        return total;
    }

    ~Meja(){
        Nota nota(id, total);
        nota.generateDaftarPesanan(pelanggan);
        nota.printHeader(jumlahPelanggan);
        nota.printNota();
        std::cout << std::endl;
        for(int i = 0; i < pelanggan.size(); i++){
            NotaIndividu newNotaIndividu(i + 1, pelanggan[i].total);
            newNotaIndividu.generateJumlahPesanan(pelanggan[i].pesanan);
            newNotaIndividu.printHeader(id, i + 1);
            newNotaIndividu.printNota(pelanggan[i].pesanan);
        }
        system("PAUSE");
    }
    
};



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
        int idMeja;
        int jumlahPelanggan;
        std::cout << "Masukkan id meja: ";
        std:: cin >> idMeja;
        std::cout << "Masukkan jumlahPelanggan: ";
        std::cin >> jumlahPelanggan;
        Meja meja(idMeja, jumlahPelanggan, menu);
    }

    
    return 0;
}