#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mutex_cout;

// Esta función es la que se va a encargar de decodificar cada letra del mensaje en ASCII a decimal.
// No tiene nada de especial, solo recibe una letra, la transforma y luego imprime el resultado.
void decodificar_a_decimal(char letra, int indice) {
    std::lock_guard<std::mutex> guardia(mutex_cout);
    int valor_decimal = static_cast<int>(letra);
    std::cout << "Letra en la posicion " << indice << "| <" << letra << "> en decimal: " << valor_decimal << std::endl;
}

// Aquí decodificamos el mensaje a hexadecimal. Exactamente igual que el anterior, pero con hex en lugar de decimal.
void decodificar_a_hexadecimal(char letra, int indice) {
    std::lock_guard<std::mutex> guardia(mutex_cout);
    int valor_hexadecimal = static_cast<int>(letra);
    std::cout << "Letra en la posicion " << indice << "| <" << letra << "> en hexadecimal: " << std::hex << valor_hexadecimal << std::endl;
}

int main() {
    std::string mensaje;

    
    std::cout << "Ingresa un mensaje menor o igual a 100 caracteres: ";
    std::getline(std::cin, mensaje);

    if (mensaje.length() > 100) {
        std::cout << "Muy largo..." << std::endl;
        return 1;
    }

    
    std::vector<std::thread> hilos;

    // Inicia la decodificación a decimal, un hilo por cada letra.
    for (int i = 0; i < mensaje.length(); ++i) {
        hilos.push_back(std::thread(decodificar_a_decimal, mensaje[i], i));
    }

   
    for (auto& hilo : hilos) {
        hilo.join();
    }

    std::cout << "Decodificacion a decimal completa." << std::endl;

   
    hilos.clear();

   
    for (int i = 0; i < mensaje.length(); ++i) {
        hilos.push_back(std::thread(decodificar_a_hexadecimal, mensaje[i], i));
    }

    
    for (auto& hilo : hilos) {
        hilo.join();
    }

    std::cout << "Decodificacion a hexadecimal completa." << std::endl;

    return 0;
}