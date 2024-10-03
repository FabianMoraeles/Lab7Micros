#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex mutex_dispensador;
std::mutex mutex_molino;

int agua_disponible = 1000;
int trigo_molido[3] = {0, 0, 0};


// Solo muele si hay suficiente agua y se detiene cuando ya no queda mas trigo para moler.
void trabajar_en_molino(int tipo_molino, int bolsas) {
    int cantidad_trigo_necesaria = bolsas * 220;
    int agua_necesaria = cantidad_trigo_necesaria / 20;

    while (cantidad_trigo_necesaria > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        /////// Seccion critica: distribucion de agua ///////
        std::lock_guard<std::mutex> guardia_agua(mutex_dispensador);
        if (agua_disponible >= agua_necesaria) {
            agua_disponible -= agua_necesaria;
            std::cout << "Molino " << tipo_molino + 1 << " recibio " << agua_necesaria << " litros de agua." << std::endl;
        } else {
            std::cout << "No hay suficiente agua disponible para el molino " << tipo_molino + 1 << "." << std::endl;
            return;
        }
        /////// Fin de la seccion critica ///////

        /////// Seccion critica: control de molinos ///////
        std::lock_guard<std::mutex> guardia_molino(mutex_molino);
        int cantidad_molida = (tipo_molino == 0) ? 18.33 : (tipo_molino == 1) ? 27.5 : 14.66;
        trigo_molido[tipo_molino] += cantidad_molida;
        cantidad_trigo_necesaria -= cantidad_molida;

        std::cout << "Molino " << tipo_molino + 1 << " molio " << cantidad_molida << " libras de trigo. Faltan: " << cantidad_trigo_necesaria << " libras." << std::endl;
        /////// Fin de la seccion critica ///////
    }
}



void empaquetar(int tipo_molino, int bolsas) {
    for (int i = 0; i < bolsas; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "Empacadora del molino " << tipo_molino + 1 << " empaco 1 costal de harina." << std::endl;
    }
}

int main() {
    int bolsas_soft, bolsas_hdw, bolsas_integral;

    std::cout << "Cuantas bolsas de harina soft (1 qq) deseas producir? ";
    std::cin >> bolsas_soft;

    std::cout << "Cuantas bolsas de harina HDW (1 qq) deseas producir? ";
    std::cin >> bolsas_hdw;

    std::cout << "Cuantas bolsas de harina integral (1 qq) deseas producir? ";
    std::cin >> bolsas_integral;

    std::vector<std::thread> hilos_molinos;
    std::vector<std::thread> hilos_empacadoras;

    // Inicia la produccion de harina en los tres molinos.
    hilos_molinos.push_back(std::thread(trabajar_en_molino, 0, bolsas_soft));
    hilos_molinos.push_back(std::thread(trabajar_en_molino, 1, bolsas_hdw));
    hilos_molinos.push_back(std::thread(trabajar_en_molino, 2, bolsas_integral));

    for (auto& hilo : hilos_molinos) {
        hilo.join();
    }

    // Inicia el proceso de empaquetado en paralelo.
    hilos_empacadoras.push_back(std::thread(empaquetar, 0, bolsas_soft));
    hilos_empacadoras.push_back(std::thread(empaquetar, 1, bolsas_hdw));
    hilos_empacadoras.push_back(std::thread(empaquetar, 2, bolsas_integral));

    for (auto& hilo : hilos_empacadoras) {
        hilo.join();
    }

    std::cout << "Produccion completa." << std::endl;

    return 0;
}