<p align="center">Министерство образования Республики Беларусь</p>
<p align="center">Учреждение образования</p>
<p align="center">“Брестский Государственный технический университет”</p>
<p align="center">Кафедра ИИТ</p>
<br><br><br><br><br><br><br>
<p align="center">Лабораторная работа №1</p>
<p align="center">По дисциплине “Теория и моделирование автоматического управления”</p>
<p align="center">Тема: “Моделирование объектов управления”</p>
<br><br><br><br><br>
<p align="right">Выполнил:</p>
<p align="right">Студент 3 курса</p>
<p align="right">Группы АС-67</p>
<p align="right">Лискович И. Г.</p>
<p align="right">Проверил:</p>
<p align="right">Дворанинович Д. А.</p>
<br><br><br><br><br>
<p align="center">Брест 2026</p>

---

## Вариант 12

- Линейная модель: **Model 1.2**
- Нелинейная модель: **Model 2.6**
- Дифференциальное уравнение: **Model 3.10**

## Реализованные модели

### Model 1.2 (Linear with delay)

$$y_{\tau+1} = a \cdot y_{\tau} + b \cdot u_{\tau-k}, \quad k \ge 1$$

Параметры: $a = 0.8$, $b = 0.5$, $k = 2$

### Model 2.6 (Nonlinear polynomial)

$$y_{\tau+1} = a_1 \cdot y_{\tau}^3 - a_2 \cdot y_{\tau-1} + b \cdot u_{\tau}^2$$

Параметры: $a_1 = 0.01$, $a_2 = 0.3$, $b = 1.0$

### Model 3.10 (ODE, Euler)

$$\frac{dy}{dt} = -a \cdot y + b + u \quad\Rightarrow\quad y_{\tau+1} = y_{\tau} + \Delta t \cdot (-a \cdot y_{\tau} + b + u_{\tau})$$

Параметры: $a = 0.5$, $b = 1.0$, $\Delta t = 0.1$

## Входные воздействия

1. **Ступенчатое** (Step): $u_{\tau} = \text{const} = 1$
2. **Импульсное** (Impulse): $u_0 = 1,\ u_{\tau > 0} = 0$
3. **Гармоническое** (Harmonic): $u_{\tau} = \sin(\tau)$

## ООП Архитектура

Создан абстрактный базовый класс `Model` с чистыми виртуальными методами
`step()` и `reset()`, а также методом `name()` для идентификации модели.

От базового класса наследуются:
- `LinearModel12` — система с задержкой
- `NonlinearModel26` — нелинейная полиномиальная
- `ODE310` — дифференциальное уравнение

## Результаты моделирования

### 1. Переходные процессы линейной модели (Model 1.2)

![Model 1.2 step](<Model 1.2 (Linear with delay)_step.png>)

![Model 1.2 impulse](<Model 1.2 (Linear with delay)_impulse.png>)

![Model 1.2 harmonic](<Model 1.2 (Linear with delay)_harmonic.png>)

### 2. Переходные процессы нелинейной модели (Model 2.6)

![Model 2.6 step](<Model 2.6 (Nonlinear polynomial)_step.png>)

![Model 2.6 impulse](<Model 2.6 (Nonlinear polynomial)_impulse.png>)

![Model 2.6 harmonic](<Model 2.6 (Nonlinear polynomial)_harmonic.png>)

### 3. Переходные процессы дифференциальной модели (Model 3.10)

![Model 3.10 step](<Model 3.10 (ODE, Euler)_step.png>)

![Model 3.10 impulse](<Model 3.10 (ODE, Euler)_impulse.png>)

![Model 3.10 harmonic](<Model 3.10 (ODE, Euler)_harmonic.png>)

## UML Диаграмма классов

```mermaid
classDiagram
    class Model {
        <<abstract>>
        +step(double u)* double
        +reset()* void
        +name()* const char*
        +setInitialState(vector~double~ init) void
    }

    class LinearModel12 {
        -double a_
        -double b_
        -int k_
        -double y_
        -deque~double~ uHistory_
        +step(double u) double
        +reset() void
        +name() const char*
    }

    class NonlinearModel26 {
        -double a1_
        -double a2_
        -double b_
        -double y_
        -double yPrev_
        +step(double u) double
        +reset() void
        +name() const char*
    }

    class ODE310 {
        -double a_
        -double b_
        -double dt_
        -double y_
        +step(double u) double
        +reset() void
        +name() const char*
    }

    class SignalGenerator {
        -SignalType type_
        -double amplitude_
        +get(double tau) double
    }

    class Simulator {
        -unique_ptr~Model~ model_
        -int n_
        +run(SignalGenerator signal) vector~double~
        +printTable(vector~double~ y, SignalGenerator signal) void
        +exportCSV(vector~double~ y, SignalGenerator signal, string fname) void
    }

    Model <|-- LinearModel12
    Model <|-- NonlinearModel26
    Model <|-- ODE310
    Simulator --> Model : owns
    Simulator --> SignalGenerator : uses
