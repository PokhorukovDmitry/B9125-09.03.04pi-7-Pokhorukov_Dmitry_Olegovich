#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

// Структура для описания товара
struct T {
    string name;     // Название товара
    int price;       // Стоимость
    int count;       // Количество на складе
    int purchases;   // Количество покупок
    int n5, n4, n3, n2, n1; // Количество отзывов по категориям
    int rating;      // Вычисленный округленный рейтинг
};

int main() {
    setlocale(LC_ALL, "Russian");

    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "Ошибка: некорректные входные данные" << endl;
        return 0;
    }

    T items[1000]; // Массив товаров (множество T)
    int n = 0;     // Счетчик количества записей
    bool hasError = false;

    // Считывание данных 
    while (file >> items[n].name >> items[n].price >> items[n].count >> items[n].purchases
        >> items[n].n5 >> items[n].n4 >> items[n].n3 >> items[n].n2 >> items[n].n1){

        if (items[n].price <= 0 || items[n].count <= 0 || items[n].purchases <= 0 ||
            items[n].n5 < 0 || items[n].n4 < 0 || items[n].n3 < 0 || items[n].n2 < 0 || items[n].n1 < 0) {
            hasError = true;
            break;
        }
        

        // Вычисление рейтинга 
        int totalReviews = items[n].n5 + items[n].n4 + items[n].n3 + items[n].n2 + items[n].n1;
        if (totalReviews <= 0) { // Защита от деления на ноль
            hasError = true;
            break;
        }

        double calcRating = (5.0 * items[n].n5 + 4.0 * items[n].n4 + 3.0 * items[n].n3 + 2.0 * items[n].n2 + 1.0 * items[n].n1) / totalReviews;
        items[n].rating = round(calcRating);

        if (items[n].rating < 1 || items[n].rating > 5) {
            hasError = true;
            break;
        }

        n++;
        if (n >= 1000) break;
    }
    file.close();

    // Вывод ошибки 
    if (n == 0 || hasError) {
        cout << "Ошибка: некорректные входные данные" << endl;
        return 0;
    }

    // Поиск товара с макс. рейтингом и самого дорогого
    int maxRatingIdx = 0;
    int maxPriceIdx = 0;

    for (int i = 0; i < n; i++) {
        if (items[i].rating > items[maxRatingIdx].rating) {
            maxRatingIdx = i;
        }
        if (items[i].price > items[maxPriceIdx].price) {
            maxPriceIdx = i;
        }
    }

    // Вывод результатов
    cout << "Товар с максимальным рейтингом: " << items[maxRatingIdx].name << ", рейтинг = " << items[maxRatingIdx].rating << endl;
    cout << "Самый дорогой товар: " << items[maxPriceIdx].name << ", количество покупок = " << items[maxPriceIdx].purchases << endl;

    // Формирование списка с рейтингом ниже 3
    bool hasLowRating = false;
    for (int i = 0; i < n; i++) {
        if (items[i].rating < 3) {
            hasLowRating = true;
            break;
        }
    }

    if (!hasLowRating) {
        cout << "Нет товаров с рейтингом ниже 3" << endl; 
    }
    else {
        ofstream fout("output.txt");
        fout << "Товары с рейтингом ниже 3:" << endl;
        for (int i = 0; i < n; i++) {
            if (items[i].rating < 3) {
                fout << items[i].name << endl; 
            }
        }
        fout.close();
    }

    return 0;
}