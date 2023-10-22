#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

string operations = "+-*/";

struct list {
    string symbol;
    list* next;
};

struct Stack {
    list* tail;
    Stack() : tail(nullptr) {}  
    bool isEmpty() {
        if (tail == nullptr) return 1;
        return 0;
    }
    void output() {
        cout << "Операторы: ";
        list* curr = tail;
        if (!curr) {
            cout << "Пусто";
        }
        while (curr) {
            cout << curr->symbol << " ";
            curr = curr->next;
        }
        cout << "\n";
    }
    string pop(bool print = 1) {
        if (tail == nullptr) return "конец";
        if (print) cout << tail->symbol << " ";
        string answer = tail->symbol;
        list* p = new list;
        p = tail;
        tail = tail->next;
        p->next = nullptr;
        delete p;
        return answer;
    }
    void push(string sym) {
        list* p = new list;
        p->symbol = sym;
        p->next = tail;
        tail = p;
    }
};

int prior(char c) {
    switch (c) {
    case '(': return 1;
    case '+': case '-': return 2;
    case '*': case '/': return 3;
    default: return 0;
    }
}

bool correctInputInf(string& data) {
    bool flag_oper = 0, flag_bracket = 0;
    int count_num = 0, count_oper = 0;
    for (int i = 0; i < data.length(); ++i) {
        if (data[i] == ' ') data.erase(i, 1);
    }
    for (int i = 0; i < data.length(); ++i) {
        if (data[i] == '(') {
            if (!flag_oper && i != 0 || flag_bracket || operations.find(data[i + 1]) != -1) {
                return 0;
            }
            flag_bracket = 1;
        }
        else if (data[i] == ')') {
            if (!flag_bracket || flag_oper || i != data.length() - 1 && operations.find(data[i + 1]) == -1) {
                return 0;
            }
            flag_bracket = 0;
        }
        else if (data[i] >= '0' && data[i] <= '9' || data[i] == '.') {
            flag_oper = 0;
        }
        else if (operations.find(data[i]) != -1) {
            if (i == 0 || i == data.length() - 1) {
                return 0;
            }
            if (flag_oper) {
                return 0;
            }
            flag_oper = 1;
        }
        else if (data[i] == '\0') continue;
        else {
            return 0;;
        }
    }
    return 1;
}

bool correctInputPo(string& data) {
    int count_oper = 0, count_num = 0;
    for (int i = 0; i < data.length(); ++i) {
        if (operations.find(data[i]) != -1) {
            if (i == 0) {
                return 0;
            }
            count_oper++;
        }
        else if (data[i] == ' ') {
            if (operations.find(data[i - 1]) == -1)
                count_num++;
        }
        else if (data[i] >= '0' && data[i] <= '9') {
            if (i == data.length() - 1) {
                return 0;
            }
            continue;
        }
        else if (data[i] == '.' || data[i] == '\0')continue;
        else {
            return 0;
        }
    }
    if (count_oper != count_num - 1) {
        return 0;
    }
    return 1;
}

bool correctInputPre(string& data) {
    int count_oper = 0, count_num = 0;
    if (data[0] == ' ') data.erase(0, 1);
    for (int i = 0; i < data.length(); ++i) {
        if (operations.find(data[i]) != -1) {
            if (i == data.length() - 1) {
                return 0;
            }
            count_oper++;
        }
        else if (data[i] == ' ' || data[i] == '\0') {
            if (operations.find(data[i - 1]) == -1)
                count_num++;
        }
        else if (data[i] >= '0' && data[i] <= '9') {
            if (i == 0) {
                return 0;
            }
            continue;
        }
        else if (data[i] == '.')continue;
        else {
            return 0;
        }
    }
    if (count_oper != count_num - 1) {
        return 0;
    }
    return 1;
}

int action(int& value1, int& value2, string& oper) {
    switch (oper[0]) {
    case '+':
        return value1 + value2;
        break;
    case '-':
        return value1 - value2;
        break;
    case '*':
        return value1 * value2;
        break;
    case '/':
        return value1 / value2;
        break;
    default:
        break;
    }
}

void calculate(string& data, bool prefix) {
    if (prefix) {
        if (!correctInputPre(data)) {
            cout << "ОШИБКА\n";
            return;
        }
        for (int j = data.length() - 1, i = 0; j >= i; --j, ++i) {
            swap(data[j], data[i]);
        }
    }
    else {
        if (!correctInputPo(data)) {
            cout << "ОШИБКА\n";
            return;
        }
    }
    Stack numbers;
    string num;
    string  sym;
    int val_1, val_2;
    for (int i = 0; i <= data.length(); i++) {
        sym = data[i];
        if (operations.find(sym) != -1) {
            if (prefix) {
                val_1 = stod(numbers.pop(0));
                val_2 = stod(numbers.pop(0));
            }
            else {
                val_2 = stod(numbers.pop(0));
                val_1 = stod(numbers.pop(0));
            }

            numbers.push(to_string(action(val_1, val_2, sym)));
        }
        if (sym >= "0" && sym <= "9" || sym == ".") {
            num += sym;
        }
        if (sym == " ") {
            if (operations.find(data[i - 1]) != -1) continue;
            if (prefix) {
                for (int j = num.length() - 1, i = 0; j >= i; --j, ++i) {
                    swap(num[j], num[i]);
                }
            }
            numbers.push(num);
            num.erase(0, num.length());
        }
    }
    numbers.pop(1);
    cout << "\n";
}

void revPolishNotation(string& data, bool print = 1)
{
    Stack stack;
    string note;
    if (!correctInputInf(data))
    {
        cout << "ОШИБКА\n";
        return;
    }
    string sym;
    for (int i = 0; i <= data.length(); ++i) {
        if (data[i] == ' ') data.erase(i, 1);
    }
    for (int i = 0; i < data.length(); i++) {
        sym = data[i];
        if (sym >= "0" && sym <= "9" || sym == ".") {
            note += sym;
            if (sym == "." || data[i + 1] == '.' || data[i + 1] >= '0' && data[i + 1] <= '9') continue;
            note += " ";
            if (print) {
                cout << "\nЧисла: " << note << "\n";
                stack.output();
            }
        }
        if (sym == "(") {
            stack.push(sym);
            if (print) {
                cout << "\nЧисла: " << note << "\n";
                stack.output();
            }
            continue;
        }
        if (sym == ")") {
            while (stack.tail->symbol != "(") {
                note += stack.pop(0);
                note += " ";
            }
            stack.pop(0);
            if (print) {
                cout << "\nЧисла: " << note << "\n";
                stack.output();
            }
            continue;
        }
        if (operations.find(sym) != -1) {

            if (stack.isEmpty() || prior(stack.tail->symbol[0] < prior(sym[0]))) {
                stack.push(sym);
                if (print) {
                    cout << "\nЧисла: " << note << "\n";
                    stack.output();
                }
            }
            else {
                while (!stack.isEmpty() && prior(stack.tail->symbol[0]) >= prior(sym[0])) {
                    note += stack.pop(0);
                    note += " ";
                }
                stack.push(sym);
                if (print) {
                    cout << "\nЧисла: " << note << "\n";
                    stack.output();

                }
            }
        }
    }
    if (print) {
        cout << "\nЧисла: " << note << "\n";
        stack.output();

    }
    while (!stack.isEmpty()) {
        note += stack.pop(0);
        note += " ";
    };
    if (print) {
        cout << "\nСтек: ";
    }
    cout << note << "\n";
    data = note;
}

void PolishNotation(string& data) {
    if (!correctInputInf(data)) {
        cout << "ОШИБКА\n";
        return;
    }
    string note;
    for (int j = data.length() - 1; j >= 0; --j) {
        if (data[j] == '(') {
            note += ')';
        }
        else if (data[j] == ')') {
            note += '(';
        }
        else if (data[j] == ' ') continue;
        else {
            note += data[j];
        }
    }
    cout << "Инвертированная строка: " << note << "\n" << "Алгоритм: ";
    revPolishNotation(note, 0);
    data.erase(0, data.length());
    for (int j = note.length() - 1; j >= 0; --j) {
        data += note[j];
    }
    data += '\0';
    cout << "Стек: " << data << "\n";
}


struct Node 
{
    int data;
    Node* prev;
    Node* next; 
    Node(int data)
    {
        this->data = data;
        this->prev = this->next = NULL;
    }
};

struct TipList
{
    Node* head, * tail;
    TipList()
    {
        head = tail = NULL;

    }

    ~TipList() {
        while (head != NULL)
        {
            delete_felem();

        }
    }

    Node* add_elem(int data) 
    {
        Node* ptr = new Node(data); 
        ptr->prev = tail;
        if (tail != NULL)
            tail->next = ptr;
        if (head == NULL)
            head = ptr;
        tail = ptr;
        return ptr;
    }
    Node* add_elem2(int data) {  
        Node* ptr = new Node(data);
        ptr->next = head;
        if (head != NULL)
            head->prev = ptr;
        if (tail == NULL)
            tail = ptr;
        return ptr;
    }

    void delete_felem() 
    {
        if (head == NULL) return;
        Node* ptr = head->next; 
        if (ptr != NULL) 
            ptr->prev = NULL;

        else   
            tail = NULL;

        delete head;
        head = ptr;
    }
    void delete_lelem() 
    {
        if (tail == NULL)return;
        Node* ptr = tail->next;
        if (ptr != NULL)
            ptr->next = NULL;

        else
            head = NULL;

        delete tail;
        tail = ptr;
    }

    void erase_elem(int index) {
        Node* ptr = get_elem(index);
        if (ptr == NULL)return;

        if (ptr->prev == NULL)
        {
            delete_felem();
            return;
        }

        if (ptr->next == NULL)
        {
            delete_lelem();
            return;
        }
        Node* left = ptr->prev;
        Node* right = ptr->next;
        left->next = right;
        right->prev = left;
        delete ptr;
    }

    Node* get_elem(int index) 
    {
        Node* ptr = head;
        int n = 0;
        while (n != index)
        {
            if (ptr == NULL)
                return ptr;
            ptr = ptr->next;
            n++;
        }
        return ptr;
    }

    Node* operator[] (int index) { 
        return get_elem(index);
    }

    Node* insert_elem(int index, int data) { 
        Node* right = get_elem(index);
        if (right == NULL)
            return add_elem(data);
        Node* left = right->prev;
        if (left == NULL)
            return add_elem2(data);

        Node* ptr = new Node(data);

        ptr->prev = left;
        ptr->next = right;
        left->next = ptr;
        right->prev = ptr;

        return ptr;
    }

    void show() {
        Node* temp = head;
        while (temp != NULL)
        {
            cout << temp->data << " ";
            temp = temp->next;
        }
    }

    void swap(int index, int index2) {
        Node* temp = head;
        for (int i = 0; i < index; i++) {
            temp = temp->next;
        }
        Node* temp1 = head;
        for (int i = 0; i < index2; i++) {
            temp1 = temp1->next;
        }
        int k = temp->data;
        temp->data = temp1->data;
        temp1->data = k;
    }

};


class dynamic_array
{
private:
    int size;
    int* array;

public:
    dynamic_array()
    {
        size = 0;
        array = NULL;
        cout << "Введите количество элементов" << endl;
        cin >> size;
        array = new int[size];
        for (int i = 0; i < size; i++) {
            array[i] = rand() % 199 - 99;
        }
    }
    ~dynamic_array() {
        delete[] array;
        array = nullptr;
    }

    void show()
    {
        for (int i = 0; i < size; i++) {
            cout << array[i] << " ";
        }
        cout << endl;
    }

    void insertArray()
    {
        show();
        cout << endl;
        int Index, Data;
        int* array2 = new int[size++];

        cout << "Введите индекс для вставки" << endl;
        cin >> Index;
        cout << "Введите значение для вставки" << endl;
        cin >> Data;
        for (int i = 0; i < size; i++)
        {
            if (i < Index)
            {
                array2[i] = array[i];
            }

            if (i >= Index)
            {
                array2[i + 1] = array[i];

            }
            if (i == Index) {

                array2[i] = Data;
            }

        }

        for (int i = 0; i < size; i++)
        {
            array[i] = array2[i];
        }
        for (int i = 0; i < size; i++)
        {
            cout << array[i] << " ";
        }
        cout << endl;
    }

    void deleteArray()
    {
        show();
        cout << "Введите индекс элемента" << endl;
        int index;
        cin >> index;
        for (int i = index; i < size - 1; i++)
        {
            array[i] = array[i + 1];
        }
        size--;

        for (int i = 0; i < size; i++)
        {
            cout << array[i] << " ";
        }
        cout << endl;
    }
    int GetElemIndex(int Index, int Data)
    {

        for (int i = 0; i < size; i++)
        {
            if (i == Index)
            {
                Data = array[i];
            }
        }

        return Data;
    }
    int GetElemData(int Index, int Data)
    {

        for (int i = 0; i < size; i++)
        {
            if (array[i] == Data)
            {
                Index = i;
            }

        }

        return Index;
    }
};

int main()
{
    setlocale(LC_ALL, "RUS");
    TipList spisok;
    int ch = 0;
    int choose = 0;
    int deystv = 0;
    int vibor = 0;
    string data = "";
    int switcher = 0;
    do {
        cout << "Выберите вариант: " << endl;
        cout << "1 - Работа с двусвязным линейным списком" << endl;
        cout << "2 - Стек" << endl;
        cout << "3 - Динамический массив" << endl;
        cout << "4 - Выход из программы" << endl;
        cin >> choose;
        int length = 0;
        switch (choose)
        {

        case 1:
            do
            {
                cout << "Выберите вариант: " << endl;
                cout << "1 - Введите количество элементов в списке" << endl;
                cout << "2 - введите в консоль элементы списка" << endl;
                cout << "3 - Работа со списком" << endl;
                cout << "4 - Выход из программы" << endl;

                cin >> vibor;
                switch (vibor)
                {
                case 1:
                    cout << "Введите длину списка: " << endl;
                    cin >> length;
                    cout << "Список: ";
                    for (int i = 0; i < length; i++)
                    {
                        int data = rand() % 100;
                        spisok.add_elem(data);

                    }
                    spisok.show();
                    system("pause");
                    break;

                case 2:
                    int data;
                    cin >> data;
                    while (data != -1)
                    {
                        spisok.add_elem(data);
                        cin >> data;
                    }
                    system("pause");
                    break;
                case 3:
                    do
                    {
                        cout << endl << "Выберите действие со списком: " << endl;
                        cout << "1 - Вывести элемент списка: " << endl;
                        cout << "2 - Вставить элемент в любую позицию списка: " << endl;
                        cout << "3 - Поменять 2 любых элемента в списке: " << endl;
                        cout << "4 - Удалить любой элемент в списке: " << endl;
                        cout << "5 - Выход из операций со списком: " << endl;


                        cin >> deystv;
                        switch (deystv)
                        {
                        case 1:
                            cout << "Исходный список" << endl;
                            spisok.show();
                            cout << endl;
                            cout << "Введите индекс элемента, который хотите вывести" << endl;
                            int index;
                            cin >> index;
                            cout << spisok[index]->data << endl;
                            system("pause");
                            break;
                        case 2:
                            cout << "Исходный список" << endl;
                            spisok.show();
                            cout << endl;
                            cout << "Введите адрес , а после саму переменную" << endl;
                            int data, head;
                            cin >> index;
                            cin >> data;
                            spisok.insert_elem(index, data);
                            spisok.show();
                            system("pause");
                            break;
                        case 3:
                            cout << "Исходный список" << endl;
                            spisok.show();
                            cout << endl;
                            int index2;
                            cout << "Первый индекс" << endl;
                            cin >> index;
                            cout << "Второй индекс" << endl;
                            cin >> index2;
                            spisok.swap(index, index2);
                            cout << "Измененный список" << endl;
                            spisok.show();
                            system("pause");
                            break;
                        case 4:
                        {
                            cout << "Исходный список" << endl;
                            spisok.show();
                            int del;
                            cin >> del;
                            spisok.erase_elem(del);
                            spisok.show();
                            system("pause");
                        }
                        break;
                        }
                    } while (deystv != 5);
                    break;
                }
            } while (vibor != 4);
            break;
        
        case 2: 
            cout << "Выберите действие\n" \
            "1. Конвертировать выражение в обратную польскую нотацию\n" \
            "2. Вычислить выражение в обратной польской нотации\n" \
            "3. Конвертировать выражение в польскую нотацию\n" \
            "4. Вычислить выражение в польской нотации\n" ;
            do
            {
                cout << "Выполнить пункт: ";
                cin >> switcher;
                switch (switcher)
                {
                case 1:
                    data.erase(0, data.length());
                    cin.ignore();
                    cout << "Введите выражение: ";
                    getline(cin, data);
                    revPolishNotation(data);
                    break;
                case 2:
                    if (!correctInputPo(data)) {
                        cout << "Введите новую обратную польскую нотацию: ";
                        cin.ignore();
                        getline(cin, data);
                        data += '\0';
                    }
                    else {
                        cout << "Хотите новую обратную польскую нотацию? (\'да\'=1) (\'нет\'=0) ";
                        cin >> switcher;
                        if (switcher) {
                            data.erase(0, data.length());
                            cin.ignore();
                            getline(cin, data);
                            data += '\0';
                        }
                    }
                    calculate(data, 0);
                    break;
                case 3:
                    data.erase(0, data.length());
                    cin.ignore();
                    cout << "Введите выражение: ";
                    getline(cin, data);
                    PolishNotation(data);
                    break;
                case 4:
                    if (!correctInputPre(data)) {
                        cout << "Ввидите польскую нотацию: ";
                        cin.ignore();
                        getline(cin, data);
                        data += '\0';
                    }
                    else {
                        cout << "Вы хотите новую польскую нотацию? (\'да\'=1) (\'нет\'=0) ";
                        cin >> switcher;
                        if (switcher) {
                            data.erase(0, data.length());
                            cin.ignore();
                            getline(cin, data);
                            data += '\0';
                        }
                        else
                            cin.ignore();
                    }
                    calculate(data, 1);
                    break;
                }
            }while (switcher != 5);
            break;
        case 3:
            cout << "Поехали" << endl;
            dynamic_array my_array;
            int e;
            do {
                cout << "Выберите действие: " << endl;
                cout << "1) Вставка" << endl;
                cout << "2) Получение значения по индексу " << endl;
                cout << "3) Получение значения по значению элемента " << endl;
                cout << "4) Удаление элемента " << endl;
                cout << "5) Выход " << endl;
                cin >> e;
                switch (e)
                {
                case 1:
                {
                    my_array.insertArray();

                }
                break;
                case 2:
                {
                    int Index, Data = 0;
                    my_array.show();
                    cout << "Введите индекс элемента котрый хотите получить " << endl;
                    cin >> Index;

                    Data = my_array.GetElemIndex(Index, Data);
                    cout << endl << "Ваш элемент = " << Data << endl;

                }
                break;
                case 3:
                {
                    int Index = -5, Data = 0;
                    my_array.show();
                    cout << "Введите элемент котрый хотите получить " << endl;
                    cin >> Data;
                    Index = my_array.GetElemData(Index, Data);
                    if (Index != -5) {
                        cout << endl << "Ваш элемент находится под номером " << Index + 1 << endl;
                    }
                    else
                    {
                        cout << "Элемент не найден!" << endl;
                    }
                }
                break;
                case 4:
                {
                    my_array.deleteArray();
                }
                break;
                case 5:
                {
                    break;
                }
                break;
                default:cout << "Ошибка! Повторите попытку" << endl;
                    break;
                }

            } while (e != 5);
        }
    }while (choose != 4);
    return 0;
}
