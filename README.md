# Библиотека работы с матрицами (OpenMP, C++)
---

## Описание
Проект представляет собой библиотеку для работы с матрицами и векторами, реализованную на C++ с использованием OpenMP для распараллеливания вычислений. 

---
## Требования

- Microsoft Visual Studio версии, поддерживающей OpenMP версии 2.0.

---
## Реализованный функционал
- **Класс матриц**: 
  - Оператор сложения матриц
  - Оператор умножение матриц
  - Обращение матрицы (через LU-разложение).
- **Класс вектора**:
  - Наследуется от класса матриц.
- **Тестирование**:
  - Проверка корректности обращения матрицы на данных разных размеров.
  - Оценка производительности операций при различном числе потоков.

---
## Результаты
- Проведены тесты на матрицах разных размеров.
- Построены таблицы и графики, демонстрирующие:
  - Зависимость времени выполнения от числа потоков
  - Зависимость нормы ошибки от параметров задачи (в случае обращения матрицы)

---
## Шаги по сборке

1. **Клонируйте репозиторий**:

   ```bash
   git clone https://github.com/Vsevolod86/Matrix_2024.git
   cd Matrix_2024
   ```

2. **Откройте проект в Visual Studio** (Откройте файл Matrix_2024.sln).

3. **Настройте поддержку OpenMP**:

* Перейдите в свойства проекта:
    * В меню Visual Studio выберите Project → Properties.

* Для всех конфигураций (Debug, Release) и платформ включите поддержку OpenMP:
    * Project → Properties → C/C++ → Language → OpenMP support → Yes.

4. **Соберите проект**:

Нажмите Ctrl+Shift+B или выберите Build → Build Solution.

5. **Запустите программу:**

Перейдите в директорию с скомпилированной программой, например: x64/Release.

---
## Запуск программы [`Matrix_2024.cpp`](Matrix_2024.cpp)

### Описание параметров запуска
`NUM_THREADS`: количество потоков для выполнения (по умолчанию: максимальное количество потоков на устройстве).
`N_MIN`: минимальный размер матриц (по умолчанию: `64`).
`NUM_MATRICES`: количество матриц для обработки (по умолчанию: `4`).
`TEST_MODE`: режим тестирования (по умолчанию: `SUM`, возможные режимы: `SUM`, `PROD`, `INV`).
`PATH_OUTPUT`: путь к выходному файлу для сохранения результатов (по умолчанию: `results.txt`).

### Описание программы

1. Генерация длин сторон квадратных матриц: `N_MIN`, `2*N_MIN`, `4*N_MIN`, ..., `2^(NUM_MATRICES)*N_MIN`.
2. В зависимости от режима для каждого размера генерируются матрицы с рандомными величинами от -1 до +1.
   1. Для режимов `SUM`, `PROD`: 2 матрицы;
   2. Для режима `INV`: 1 матрица.
3. Происходит выполнение соответствующей операции со сгенерированными матрицами с использованием разного числа потоков (от 1 до `NUM_THREADS`). Для каждой операции замеряется время потраченое на неё, а в случае `INV` ещё и норма ошибки обращения.
4. После очередной выполненной операции над матрицами, результаты выводятся в косоль, а также сохраняются в файл `PATH_OUTPUT`.

### Параметры запуска
Скомпилированный файл можно запустить (с параметрами по умолчанию) с консоли командой:

```bash
.\Matrix_2024.exe
```

Также при запуске можно менять некоторые параметры кода, например выполнив следующую команду в косоли.

```bash
.\Matrix_2024.exe NUM_THREADS=4 N_MIN=100 NUM_MATRICES=5 TEST_MODE="INV" PATH_OUTPUT="inv_results.txt"
```

---
## Полученные результаты

Результаты моих запусков расположены в [`output`](https://github.com/Vsevolod86/Matrix_2024/blob/main/output),
оформленный в единый отчет - [`results.md`](output/results.md)
