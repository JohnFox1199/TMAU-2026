#include "LinearModel12.h"
#include "NonlinearModel26.h"
#include "ODE310.h"
#include "Simulator.h"
#include <iostream>
#include <memory>
#include <string>
#include <limits>
#include <cstdlib>
#include <set>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <array>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

double readDouble(const std::string& prompt) {
    double x = 0.0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Ошибка ввода, попробуйте снова.\n";
    }
}

int readInt(const std::string& prompt) {
    int x = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Ошибка ввода, попробуйте снова.\n";
    }
}

SignalType readSignalType() {
    while (true) {
        std::cout << "Тип сигнала: 1 - step, 2 - impulse, 3 - harmonic: ";
        if (int c = 0; std::cin >> c) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (c == 1) {
                return SignalType::STEP;
            }
            if (c == 2) {
                return SignalType::IMPULSE;
            }
            if (c == 3) {
                return SignalType::HARMONIC;
            }
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Неверный выбор, введите 1, 2 или 3.\n";
    }
}

std::string signalName(SignalType t) {
    switch (t) {
        case SignalType::STEP:
            return "step";
        case SignalType::IMPULSE:
            return "impulse";
        case SignalType::HARMONIC:
            return "harmonic";
    }
    return "unknown";
}

std::string findPlotScript() {
    const std::array<std::string, 3> candidates = {
        "plot_single.py",
        "..\\plot_single.py",
        "../plot_single.py"
    };
    for (const std::string& c : candidates) {
        std::ifstream f(c);
        if (f.good()) {
            return c;
        }
    }
    return std::string("plot_single.py");
}

void runAndPlot(std::unique_ptr<Model> model, int n,
                SignalType sigType, const std::string& csvName) {
    SignalGenerator signal(sigType, 1.0);
    Simulator sim(std::move(model), n);

    auto y = sim.run(signal);
    sim.printTable(y, signal);
    sim.exportCSV(y, signal, csvName);
    std::cout << "\nCSV сохранён: " << csvName << "\n";

    std::string script = findPlotScript();
    std::string cmd = "python \"" + script + "\" " + csvName;
    std::cout << "[Построение графика...]\n";
    int rc = std::system(cmd.c_str());
    if (rc != 0) {
        std::cout << "  Не удалось построить график.\n"
                  << "  Проверьте, что Python и matplotlib установлены:\n"
                  << "    pip install pandas matplotlib\n";
    }
}

void runModel12() {
    std::cout << "\nModel 1.2: y(t+1) = a*y(t) + b*u(t-k)\n";
    double a = readDouble("a (|a| < 1 для устойчивости): ");
    double b = readDouble("b: ");
    int    k = readInt   ("k (задержка, >= 1): ");
    int    n = readInt   ("Количество шагов n: ");
    SignalType s = readSignalType();
    runAndPlot(std::make_unique<LinearModel12>(a, b, k), n, s,
               "out_Model_1_2_" + signalName(s) + ".csv");
}

void runModel26() {
    std::cout << "\nModel 2.6: y(t+1) = a1*y^3 - a2*y(t-1) + b*u^2\n";
    double a1 = readDouble("a1: ");
    double a2 = readDouble("a2: ");
    double b  = readDouble("b: ");
    int    n  = readInt   ("Количество шагов n: ");
    SignalType s = readSignalType();
    runAndPlot(std::make_unique<NonlinearModel26>(a1, a2, b), n, s,
               "out_Model_2_6_" + signalName(s) + ".csv");
}

void runModel310() {
    std::cout << "\nModel 3.10: dy/dt = -a*y + b + u (метод Эйлера)\n";
    double a  = readDouble("a: ");
    double b  = readDouble("b: ");
    double dt = readDouble("dt (шаг интегрирования, > 0): ");
    int    n  = readInt   ("Количество шагов n: ");
    SignalType s = readSignalType();
    runAndPlot(std::make_unique<ODE310>(a, b, dt), n, s,
               "out_Model_3_10_" + signalName(s) + ".csv");
}

std::set<int> readModelChoice() {
    std::cout << "Выберите модели для симуляции:\n";
    std::cout << "  1 - Model 1.2 (линейная с задержкой)\n";
    std::cout << "  2 - Model 2.6 (нелинейная полиномиальная)\n";
    std::cout << "  3 - Model 3.10 (ODE, метод Эйлера)\n";
    std::cout << "  0 - все модели\n";
    std::cout << "Введите номера через запятую (например, 1,3 или 0): ";

    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::string token;
    std::set<int> chosen;
    while (std::getline(ss, token, ',')) {
        try {
            const int v = std::stoi(token);
            if (v >= 0 && v <= 3) {
                chosen.insert(v);
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "  Пропущен нечисловой токен '" << token
                      << "': " << e.what() << "\n";
        } catch (const std::out_of_range& e) {
            std::cerr << "  Число вне диапазона '" << token
                      << "': " << e.what() << "\n";
        }
    }
    if (chosen.empty() || chosen.count(0) > 0) {
        chosen = {1, 2, 3};
    }
    return chosen;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "Лабораторная работа №1, вариант 12\n";
    std::cout << "Студент: as06712\n";
    std::cout << "Модели: 1.2, 2.6, 3.10\n\n";
    auto chosen = readModelChoice();

    if (chosen.count(1) > 0) {
        runModel12();
    }
    if (chosen.count(2) > 0) {
        runModel26();
    }
    if (chosen.count(3) > 0) {
        runModel310();
    }

    std::cout << "\nГотово\n";
    return 0;
}
