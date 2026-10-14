# Лабораторная работа №1. Моделирование управляемого объекта

**Студент:** as06712  
**Вариант:** 12  
**Модели:** Model 1.2, Model 2.6, Model 3.10

## 1. Цель работы

Изучить методы моделирования линейных, нелинейных и дифференциальных
систем с использованием объектно-ориентированного подхода в C++.
Реализовать программу, которая:

- имитирует поведение управляемого объекта на дискретных шагах времени τ;
- поддерживает настройку количества шагов симуляции `n`;
- использует три типа входных воздействий (ступенчатое, импульсное, гармоническое);
- выводит результаты в табличном виде и экспортирует в CSV;
- визуализирует результаты через Python/Matplotlib;
- собирается через CMake с помощью `.bat`-файла.

## 2. Постановка задачи

Для варианта 12 заданы три модели:

### Model 1.2 — линейная система с чистым запаздыванием по управлению

$$y_{\tau+1} = a \cdot y_{\tau} + b \cdot u_{\tau-k}, \quad k \ge 1$$

### Model 2.6 — нелинейная асимметричная полиномиальная модель

$$y_{\tau+1} = a_1 \cdot y_{\tau}^3 - a_2 \cdot y_{\tau-1} + b \cdot u_{\tau}^2$$

### Model 3.10 — дифференциальное уравнение с внешним смещением

$$\frac{dy}{dt} = -a \cdot y + b + u$$

Решается методом Эйлера с шагом $\Delta t$:

$$y_{\tau+1} = y_{\tau} + \Delta t \cdot \left(-a \cdot y_{\tau} + b + u_{\tau}\right)$$

## 3. Архитектура программы

Программа реализована на основе принципов ООП:

- **Базовый абстрактный класс `Model`** содержит чисто виртуальные методы
  `step()`, `reset()`, `name()`.
- **Наследники** реализуют конкретные уравнения:
  `LinearModel12`, `NonlinearModel26`, `ODE310`.
- **`SignalGenerator`** генерирует три типа входных воздействий.
- **`Simulator`** управляет процессом: прогоняет модель по шагам,
  собирает результаты, выводит таблицу и экспортирует в CSV.

### UML-диаграмма классов

```mermaid
classDiagram
    class Model {
        <<abstract>>
        +step(u: double) double
        +reset() void
        +name() const char*
        +setInitialState(init) void
    }
    class LinearModel12 {
        -a_: double
        -b_: double
        -k_: int
        -y_: double
        -uHistory_: deque
        +step(u) double
        +reset() void
        +name() const char*
    }
    class NonlinearModel26 {
        -a1_: double
        -a2_: double
        -b_: double
        -y_: double
        -yPrev_: double
        +step(u) double
        +reset() void
        +name() const char*
    }
    class ODE310 {
        -a_: double
        -b_: double
        -dt_: double
        -y_: double
        +step(u) double
        +reset() void
        +name() const char*
    }
    class SignalGenerator {
        -type_: SignalType
        -amplitude_: double
        +get(tau) double
    }
    class Simulator {
        -model_: unique_ptr~Model~
        -n_: int
        +run(signal) vector~double~
        +printTable(y, signal) void
        +exportCSV(y, signal, fname) void
    }
    Model <|-- LinearModel12
    Model <|-- NonlinearModel26
    Model <|-- ODE310
    Simulator --> Model : owns
    Simulator --> SignalGenerator : uses


