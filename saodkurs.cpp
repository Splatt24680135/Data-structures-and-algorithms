#include <iostream>
#include <string>
#include <iomanip>
#include <Windows.h>

using namespace std;

struct Resident
{
	int year;
	char passport[12];
	char fullName[100];
	char address[100];
	char destination[100];
	Resident *next;
};

struct Inn
{
	int seats;
	int rooms;
	bool bathroom;
	bool isRoomEmpty;
	string equipment;
	string roomNum;
	Inn *left;
	Inn *right;
	int height = 1;
};

struct InOut
{
	string regNum;
	char regPas[12];
	char enteringDate[11];
	char leavingDate[11];
	InOut *next;
	InOut *prev;
};

Inn* AddRoom(Inn *workingRoom, Inn *newRoom);
Inn* DeleteRoom(Inn *workingRoom, Inn *newRoom);
Inn* MinLeaf(Inn *workingRoom);
Inn* MinLeafRemoved(Inn *workingRoom);
Inn* Balancing(Inn *workingRoom);
Inn* RotateRight(Inn *workingRoom);
Inn* RotateLeft(Inn *workingRoom);
void FixHeight(Inn *workingRoom);
void PrintRoom(Inn *workingRoom);
void FreeRooms(Inn *&workingRoom);
void SearchRoomByEquip(Inn *workingRoom, string equip);
void SearchRoomByNum(InOut *inOutSearch, Inn *workingRoom, Resident **person, string roomNum);
bool SearchRoom(Inn *workingRoom, Inn *newRoom);
int BMAlg(string workingRoom, string equip);
int BalanceFactor(Inn *workingRoom);
int Height(Inn *workingRoom);

Resident* AddRes(Resident **hashtable, Resident *additionalRes, int hashKey);
Resident* DeleteRes(Resident **hashtable, char *passport, int hashKey);
void PrintRes(Resident **hashtable);
void FreeRes(Resident **hashtable);
void SearchResByPassport(InOut *inOutSearch, Resident **hashtable, char *passport);
void SearchResByFullName(Resident **hashtable, char *fullName);
bool SearchRes(Resident **hashtable, char *passport, int hashKey);
int HashFunc(char *passport);

InOut* AddResToInn(InOut *workingReg, InOut *join);
InOut* DelResFromInn(InOut *workingReg, InOut *join);
void RoomSwitchStatus(InOut *entrance, Inn *&workingRoom, string roomNum);

string CheckRoomNum(string x);
char CheckPassport(char *x);
char CheckDate(char *x);
int CheckYear(int x);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	InOut *newInOut = new InOut;
	strcpy_s(newInOut->enteringDate, "");
	strcpy_s(newInOut->leavingDate, "");
	strcpy_s(newInOut->regPas, "");
	newInOut->regNum = "";
	newInOut->next = newInOut->prev = NULL;
	Inn *workingRoom = new Inn;
	workingRoom = NULL;
	Resident **newResident = new Resident*[100];
	for (int i = 0; i < 100; i++) {
		newResident[i] = new Resident[3];
		strcpy_s(newResident[i]->address, "");
		strcpy_s(newResident[i]->destination, "");
		strcpy_s(newResident[i]->fullName, "");
		strcpy_s(newResident[i]->passport, "");
		newResident[i]->year = 0;
		newResident[i]->next = NULL;
	}
	int hashKey = 0;
	start_point:
	system("cls");
	cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
		<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
		<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
		<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
		<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
	int menu;
	cin >> menu;
	menu = CheckYear(menu);
	while (menu != 0) {
		switch (menu) {
		case 1: {
			system("cls");
			InOut *addInOut = new InOut;
			Inn *tempInn = new Inn;
			Resident *tempRes = new Resident;
			string InOutRoomClass, InOutRoomNum;
			int temp;
			cout << "Введите дату заселения в формате \"ДД.ММ.ГГГГ\"" << endl;
			cin >> addInOut->enteringDate;
			*addInOut->enteringDate = CheckDate(addInOut->enteringDate);
			cout << "Введите дату выселения в формате \"ДД.ММ.ГГГГ\"" << endl;
			cin >> addInOut->leavingDate;
			*addInOut->leavingDate = CheckDate(addInOut->leavingDate);
			cout << "Введите паспорт в формате \"0000-000000\"" << endl;
			cin >> addInOut->regPas;
			*addInOut->regPas = CheckPassport(addInOut->regPas);
			strcpy_s(tempRes->passport, addInOut->regPas);
			hashKey = HashFunc(tempRes->passport);
			if (SearchRes(newResident, tempRes->passport, hashKey) == 0) {
				cout << "Постоялец с таким паспортом не найден" << endl;
				system("pause");
				goto start_point;
			}
			cout << "Выберите класс комнаты: \n1 - Люкс \n2 - Полулюкс \n3 - Одноместный \n4 - Многоместный" << endl;
			cin >> temp;
			switch (temp) {
			case 1: {
				InOutRoomClass = 'Л';
				break;
			}
			case 2: {
				InOutRoomClass = 'П';
				break;
			}
			case 3: {
				InOutRoomClass = 'О';
				break;
			}
			case 4: {
				InOutRoomClass = 'М';
				break;
			}
			}
			cout << "Введите номер комнаты" << endl;
			cin >> InOutRoomNum;
			InOutRoomNum = CheckRoomNum(InOutRoomNum);
			addInOut->regNum = InOutRoomNum + InOutRoomClass;
			tempInn->roomNum = addInOut->regNum;
			if (SearchRoom(workingRoom, tempInn) == 0 || (SearchRoom(workingRoom, tempInn) == 1 && workingRoom->isRoomEmpty == 0)) {
				cout << "Комнаты с таким номером не существует или она занята" << endl;
				system("pause");
				goto start_point;
			}
			RoomSwitchStatus(addInOut, workingRoom, addInOut->regNum);
			newInOut = AddResToInn(newInOut, addInOut);
			cout << "Поселенец заселён" << endl;
			delete tempRes;
			delete tempInn;
			delete addInOut;
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 2: {
			system("cls");
			InOut *delInOut = new InOut;
			Inn *tempInn = new Inn;
			Resident *tempRes = new Resident;
			string delInOutClass, delInOutNum;
			int temp;
			strcpy_s(delInOut->enteringDate, "");
			strcpy_s(delInOut->leavingDate, "");
			cout << "Введите паспорт в формате \"0000-000000\"" << endl;
			cin >> delInOut->regPas;
			*delInOut->regPas = CheckPassport(delInOut->regPas);
			strcpy_s(tempRes->passport, delInOut->regPas);
			hashKey = HashFunc(tempRes->passport);
			if (SearchRes(newResident, tempRes->passport, hashKey) == 0) {
				cout << "Постоялец с таким паспортом не найден" << endl;
				system("pause");
				goto start_point;
			}			
			cout << "Выберите класс удаляемой комнаты: \n1 - Люкс \n2 - Полулюкс \n3 - Одноместный \n4 - Многоместный" << endl;
			cin >> temp;
			switch (temp) {
			case 1: {
				delInOutClass = 'Л';
				break;
			}
			case 2: {
				delInOutClass = 'П';
				break;
			}
			case 3: {
				delInOutClass = 'О';
				break;
			}
			case 4: {
				delInOutClass = 'М';
				break;
			}
			}
			cout << "Введите номер комнаты" << endl;
			cin >> delInOutNum;
			delInOut->regNum = delInOutNum + delInOutClass;
			tempInn->roomNum = delInOut->regNum;
			if (SearchRoom(workingRoom, tempInn) == 0 || (SearchRoom(workingRoom, tempInn) == 1 && workingRoom->isRoomEmpty == 1)) {
				cout << "Комнаты с таким номером не существует или она пуста" << endl;
				system("pause");
				goto start_point;
			}
			RoomSwitchStatus(delInOut, workingRoom, delInOut->regNum);
			newInOut = DelResFromInn(newInOut, delInOut);
			cout << "Поселенец выселен" << endl;
			delete tempRes;
			delete tempInn;
			delete delInOut;
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 3: {
			system("cls");
			Inn *elseRoom = new Inn;
			string addNum, addClass;
			int bath;
			elseRoom->isRoomEmpty = 1;
			cout << "Есть ли в комнате душ? \n1 - Да \n2 - Нет" << endl;
			cin >> bath;
			if (bath == 1) {
				elseRoom->bathroom = 1;
			}
			else {
				elseRoom->bathroom = 0;
			}
			cout << "Введите количество комнат" << endl;
			cin >> elseRoom->rooms;
			cout << "Введите количество мест для посетителей" << endl;
			cin >> elseRoom->seats;
			cout << "Введите список оборудования в номере" << endl;
			cin.ignore();
			getline(cin, elseRoom->equipment);
			if (elseRoom->seats > 1) {
				addClass = 'М';
			}
			else {
				cout << "Выберите класс комнаты: \n1 - Люкс \n2 - Полулюкс \n3 - Одноместный" << endl;
				int temp;
				cin >> temp;
				switch (temp) {
				case 1: {
					addClass = 'Л';
					break;
				}
				case 2: {
					addClass = 'П';
					break;
				}
				case 3: {
					addClass = 'О';
					break;
				}
				}
			}
			cout << "Введите номер комнаты" << endl;
			cin >> addNum;
			addNum = CheckRoomNum(addNum);
			elseRoom->roomNum = addNum + addClass;
			if (workingRoom != NULL) {
				if (SearchRoom(workingRoom, elseRoom) == 1) {
					cout << "Такая комната уже существует" << endl;
					system("pause");
					break;
				}
				else {
					workingRoom = AddRoom(workingRoom, elseRoom);
					cout << "Комната добавлена в список" << endl;
				}
			}
			else {
				workingRoom = AddRoom(workingRoom, elseRoom);
				cout << "Комната добавлена в список" << endl;
			}
			delete elseRoom;
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 4: {
			system("cls");
			int tempSearch;
			string delNum, delClass;
			Inn *delRoom = new Inn;
			cout << "Выберите класс удаляемой комнаты: \n1 - Люкс \n2 - Полулюкс \n3 - Одноместный \n4 - Многоместный" << endl;
			cin >> tempSearch;
			switch (tempSearch) {
			case 1: {
				delClass = 'Л';
				break;
			}
			case 2: {
				delClass = 'П';
				break;
			}
			case 3: {
				delClass = 'О';
				break;
			}
			case 4: {
				delClass = 'М';
				break;
			}
			}
			cout << "Введите номер комнаты" << endl;
			cin >> delNum;
			delNum = CheckRoomNum(delNum);
			delRoom->roomNum = delNum + delClass;
			if (SearchRoom(workingRoom, delRoom) == 0) {
				cout << "Такая комната не найдена" << endl;
				system("pause");
				goto start_point;
			}
			else {
				workingRoom = DeleteRoom(workingRoom, delRoom);
				cout << "Удалено" << endl;
			}			
			delete delRoom;
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 5: {
			system("cls");
			PrintRoom(workingRoom);
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 6: {
			system("cls");
			FreeRooms(workingRoom);
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 7: {
			system("cls");
			int tempSearch;
			string searchClass, searchNum, searchString;
			cout << "Выберите класс искомой комнаты: \n1 - Люкс \n2 - Полулюкс \n3 - Одноместная \n4 - Многоместная" << endl;
			cin >> tempSearch;
			switch (tempSearch) {
			case 1: {
				searchClass = 'Л';
				break;
			}
			case 2: {
				searchClass = 'П';
				break;
			}
			case 3: {
				searchClass = 'О';
				break;
			}
			case 4: {
				searchClass = 'М';
				break;
			}
			}
			cout << "Введите номер комнаты" << endl;
			cin >> searchNum;
			searchNum = CheckRoomNum(searchNum);
			searchString = searchNum + searchClass;
			SearchRoomByNum(newInOut, workingRoom, newResident, searchString);
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 8: {
			system("cls");
			string equip;
			cout << "Введите искомое оборудование" << endl;
			cin.ignore();
			getline(cin, equip);
			SearchRoomByEquip(workingRoom, equip);
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 9: {
			system("cls");
			Resident *elseRes = new Resident;
			cout << "Введите ФИО" << endl;
			cin.ignore();
			cin.getline(elseRes->fullName, 100);
			cout << "Введите год рождения" << endl;
			cin >> elseRes->year;
			elseRes->year = CheckYear(elseRes->year);
			cout << "Введите адрес" << endl;
			cin.ignore();
			cin.getline(elseRes->address, 100);
			cout << "Введите цель прибытия" << endl;
			cin.clear();
			cin.getline(elseRes->destination, 100);
			cout << "Введите серию и номер паспора в формате \"0000-000000\"" << endl;
			cin >> elseRes->passport;
			*elseRes->passport = CheckPassport(elseRes->passport);
			elseRes->next = NULL;
			hashKey = HashFunc(elseRes->passport);
			if (SearchRes(newResident, elseRes->passport, hashKey) == 1) {
				cout << "Постоялец с таким паспортом уже существует" << endl;
			}
			else {
				if (newResident[hashKey]->year == 0) {
					newResident[hashKey] = AddRes(newResident, elseRes, hashKey);
					cout << "Постоялец добавлен" << endl;
				}
				else {
					newResident[hashKey]->next = AddRes(newResident, elseRes, hashKey);
					cout << "Постоялец добавлен" << endl;
				}
			}
			delete elseRes;
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 10: {
			system("cls");
			char temp[12];
			cout << "Введите номер паспорта в формате \"0000-000000\"" << endl;
			cin >> temp;
			*temp = CheckPassport(temp);
			hashKey = HashFunc(temp);
			if (SearchRes(newResident, temp, hashKey) == 0) {
				cout << "Постоялец с таким паспортом не найден" << endl;
				system("pause");
				goto start_point;
			}
			else {
				newResident[hashKey] = DeleteRes(newResident, temp, hashKey);
				cout << "Удалено" << endl;
			}			
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 11: {
			system("cls");
			PrintRes(newResident);
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 12: {
			system("cls");
			FreeRes(newResident);
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 13: {
			system("cls");
			char temp[12];
			cout << "Введите серию и номер паспора в формате \"0000-000000\"" << endl;
			cin >> temp;
			*temp = CheckPassport(temp);
			SearchResByPassport(newInOut, newResident, temp);
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 14: {
			system("cls");
			char temp[100];
			cout << "Введите ФИО" << endl;
			cin.ignore();
			cin.getline(temp, 100);
			SearchResByFullName(newResident, temp);
			system("pause");
			system("cls");
			cout << "1 - Добавление постояльца в комнату \n2 - Выселение постояльца из комнаты \n3 - Добавить новую комнату в список" << endl
				<< "4 - Убрать комнату из списка \n5 - Список всех существующих комнат \n6 - Убрать все комнаты из списка" << endl
				<< "7 - Поиск комнаты по номеру \n8 - Поиск комнаты по оборудованию \n9 - Добавить нового постояльца" << endl
				<< "10 - Убрать запись о постояльце \n11 - Список всех постояльцев \n12 - Убрать записи о всех постояльцах" << endl
				<< "13 - Поиск постояльца по паспорту \n14 - Поиск постояльца по имени \n0 - Выход" << endl;
			cin >> menu;
			menu = CheckYear(menu);
			break;
		}
		case 0: {
			break;
		}
		default: {
			cout << "Повторите ввод" << endl;
			cin >> menu;
			menu = CheckYear(menu);
		}
		}
	}
	system("pause");
	return 0;
}

Inn* AddRoom(Inn *workingRoom, Inn *newRoom)
{
	if (!workingRoom) {
		workingRoom = new Inn;
		workingRoom->bathroom = newRoom->bathroom;
		workingRoom->equipment = newRoom->equipment;
		workingRoom->roomNum = newRoom->roomNum;
		workingRoom->seats = newRoom->seats;
		workingRoom->rooms = newRoom->rooms;
		workingRoom->isRoomEmpty = newRoom->isRoomEmpty;
		workingRoom->left = workingRoom->right = NULL;
		return workingRoom;
	}
	if (newRoom->roomNum < workingRoom->roomNum) {
		workingRoom->left = AddRoom(workingRoom->left, newRoom);
	}
	else {
		workingRoom->right = AddRoom(workingRoom->right, newRoom);
	}
	return Balancing(workingRoom);
}

Inn* DeleteRoom(Inn *workingRoom, Inn *newRoom)
{
	if (!workingRoom) {
		return 0;
	}
	else if (newRoom->roomNum < workingRoom->roomNum) {
		workingRoom->left = DeleteRoom(workingRoom->left, newRoom);
	}
	else if (newRoom->roomNum > workingRoom->roomNum) {
		workingRoom->right = DeleteRoom(workingRoom->right, newRoom);
	}
	else if (newRoom->roomNum == workingRoom->roomNum) {
		Inn *leftBranch = workingRoom->left;
		Inn *rightBranch = workingRoom->right;
		delete workingRoom;
		if (rightBranch == NULL) {
			return leftBranch;
		}
		Inn *min = MinLeaf(rightBranch);
		min->right = MinLeafRemoved(rightBranch);
		min->left = leftBranch;
		return Balancing(min);
	}
	return Balancing(workingRoom);
}
Inn* MinLeaf(Inn* workingRoom)
{
	if (workingRoom->left == NULL) {
		return workingRoom;
	}
	else {
		MinLeaf(workingRoom->left);
	}
}
Inn* MinLeafRemoved(Inn* workingRoom)
{
	if (workingRoom->left == NULL) {
		return workingRoom->right;
	}
	workingRoom->left = MinLeafRemoved(workingRoom->left);
	return Balancing(workingRoom);
}

bool SearchRoom(Inn *workingRoom, Inn *newRoom)
{
	if (!workingRoom) {
		return 0;
	}
	else if (workingRoom->roomNum == newRoom->roomNum) {
		return 1;
	}
	else if (workingRoom->roomNum > newRoom->roomNum) {
		SearchRoom(workingRoom->left, newRoom);
	}
	else if (workingRoom->roomNum < newRoom->roomNum) {
		SearchRoom(workingRoom->right, newRoom);
	}
}
void SearchRoomByNum(InOut *inOutSearch, Inn *workingRoom, Resident **person, string roomNum)
{
	InOut *tempInOut = new InOut;
	tempInOut = inOutSearch;
	Resident *tempRes = new Resident;
	int hashKey = 0;
	if (!workingRoom) {
		return;
	}
	else if (workingRoom->roomNum == roomNum) {
		cout << endl << "Найдена комната: \nНомер комнаты: " << workingRoom->roomNum << endl << "Количество комнат: " << workingRoom->rooms
			<< ", количество мест: " << workingRoom->seats << endl << "Полный список оборудования: " << workingRoom->equipment << endl;
		if (workingRoom->bathroom == 1) {
			cout << "В номере имеется санузел" << endl;
		}
		else {
			cout << "В номере отстствует санузел" << endl;
		}
		while (tempInOut) {
			if (tempInOut->regNum == roomNum) {
				cout << "Номер паспорта заселённого постояльца: " << tempInOut->regPas << endl;
				hashKey = HashFunc(tempInOut->regPas);
				tempRes = person[hashKey];
				while (tempRes != NULL) {
					if (strcmp(tempRes->passport, tempInOut->regPas) == 0) {
						cout << "ФИО постояльца: " << tempRes->fullName << endl;
						tempRes = tempRes->next;
					}
				}
			}
			tempInOut = tempInOut->next;
		}
		cout << endl;
		return;
	}
	else if (workingRoom->roomNum > roomNum) {
		SearchRoomByNum(inOutSearch, workingRoom->left, person, roomNum);
	}
	else if (workingRoom->roomNum < roomNum) {
		SearchRoomByNum(inOutSearch, workingRoom->right, person, roomNum);
	}
}
void SearchRoomByEquip(Inn *workingRoom, string equip)
{
	if (workingRoom) {
		SearchRoomByEquip(workingRoom->left, equip);
		SearchRoomByEquip(workingRoom->right, equip);
		if (BMAlg(workingRoom->equipment, equip) == 1) {
			cout << endl << "Номер комнаты: " << workingRoom->roomNum << endl << "Количество комнат: " << workingRoom->rooms
				<< ", количество мест: " << workingRoom->seats << endl << "Полный список оборудования: " << workingRoom->equipment << endl << endl;
			return;
		}
	}
}

int BMAlg(string workingRoom, string equip)
{	
	int sourceLen = workingRoom.length();
	int equipLen = equip.length();
	if (equipLen > sourceLen) {
		return -1;
	}
	int i = equipLen; 
	int j = i, k = i;
	string *temp = new string[equipLen + 1];
	string tmp2;
	for (i; i <= sourceLen; i++) {
		j = equipLen;
		k = i;
		while ((j > 0) && (workingRoom[k - 1] == equip[j - 1])) {
			temp[j] = workingRoom[k - 1];
			k--;
			j--;
		}
		for (int z = 0; z < equipLen + 1; z++) {
			if (temp[z] == "" || temp[z] == ",") {
				tmp2.clear();
			}
			else {
				tmp2 += temp[z];
			}
			if (tmp2 == equip) {
				return 1;
			}
		}
	}
}

void FreeRooms(Inn *&workingRoom) {
	if (workingRoom != NULL) {
		FreeRooms(workingRoom->left);
		FreeRooms(workingRoom->right);
		delete workingRoom;
		workingRoom = NULL;
	}
}

void PrintRoom(Inn *workingRoom)
{
	if (workingRoom) {
		PrintRoom(workingRoom->left);
		cout << "Комната номер: " << workingRoom->roomNum << endl << "Количество комнат: " << workingRoom->rooms << ", количество мест: " 
			<< workingRoom->seats << endl << "Список оборудования: " << workingRoom->equipment << endl;
		if (workingRoom->isRoomEmpty == 1) {
			cout << "В комнате есть свободные места" << endl << endl;
		}
		else {
			cout << "Комната занята" << endl << endl;
		}
		PrintRoom(workingRoom->right);
	}
	else {
		return;
	}
}

Inn* Balancing(Inn *workingRoom)
{
	FixHeight(workingRoom);
	if (BalanceFactor(workingRoom) == 2) {
		if (BalanceFactor(workingRoom->right) < 0) {
			workingRoom->right = RotateRight(workingRoom->right);
		}
		return RotateLeft(workingRoom);
	}
	if (BalanceFactor(workingRoom) == -2) {
		if (BalanceFactor(workingRoom->left) > 0) {
			workingRoom->left = RotateLeft(workingRoom->left);
		}
		return RotateRight(workingRoom);
	}
	return workingRoom;
}
Inn* RotateRight(Inn *workingRoom)
{
	Inn *rotate = workingRoom->left;
	workingRoom->left = rotate->right;
	rotate->right = workingRoom;
	FixHeight(workingRoom);
	FixHeight(rotate);
	return rotate;
}
Inn* RotateLeft(Inn *workingRoom)
{
	Inn *rotate = workingRoom->right;
	workingRoom->right = rotate->left;
	rotate->left = workingRoom;
	FixHeight(workingRoom);
	FixHeight(rotate);
	return rotate;
}
void FixHeight(Inn *workingRoom)
{
	int h1 = Height(workingRoom->left);
	int h2 = Height(workingRoom->right);
	workingRoom->height = (h1 > h2 ? h1 : h2) + 1;
}
int BalanceFactor(Inn *workingRoom)
{
	return Height(workingRoom->right) - Height(workingRoom->left);
}
int Height(Inn *workingRoom)
{
	return workingRoom ? workingRoom->height : 0;
}

int HashFunc(char *passport)
{
	int summ = 0;
	for (int i = 0; i < 11; i++) {
		if (i == 4) {
			continue;
		}
		else {
			summ += passport[i];
		}
	}
	return (summ % 17);
}

Resident* AddRes(Resident **hashtable, Resident *additionalRes, int hashKey)
{
	Resident *person = new Resident;
	person = hashtable[hashKey];
	if (person->year == 0) {
		for (int i = 0; i < 100; i++) {
			person->address[i] = additionalRes->address[i];
			person->destination[i] = additionalRes->destination[i];
			person->fullName[i] = additionalRes->fullName[i];
			person->passport[i] = additionalRes->passport[i];
		}
		person->year = additionalRes->year;
		person->next = NULL;
		return person;
	}
	else {
		Resident *temp = new Resident;
		for (int i = 0; i < 100; i++) {
			temp->address[i] = additionalRes->address[i];
			temp->destination[i] = additionalRes->destination[i];
			temp->fullName[i] = additionalRes->fullName[i];
			temp->passport[i] = additionalRes->passport[i];
		}
		temp->year = additionalRes->year;
		temp->next = NULL;
		person->next = temp;
		return person->next;
	}	
}

Resident* DeleteRes(Resident **hashtable, char *passport, int hashKey)
{
	bool flag = 0;
	for (int i = 0; i < 100; i++) {
		while (hashtable[i]->passport != "") {
			if (strcmp(passport, hashtable[i]->passport) == 0) {
				strcpy_s(hashtable[i]->address, "");
				strcpy_s(hashtable[i]->destination, "");
				strcpy_s(hashtable[i]->fullName, "");
				strcpy_s(hashtable[i]->passport, "");
				hashtable[i]->year = 0;
				cout << "Удалено" << endl;
				flag = 1;
			}
			if (hashtable[i]->next == NULL) {
				break;
			}
			hashtable[i] = hashtable[i]->next;
		}
	}
	if (flag == 0) {
		cout << "Постоялец с таким паспортом не найден" << endl;
	}
	return hashtable[hashKey];
}

bool SearchRes(Resident **hashtable, char *passport, int hashKey)
{
	Resident *tmp = new Resident;
	tmp = hashtable[hashKey];
	while (tmp != 0) {
		if (strcmp(tmp->passport, passport) == 0) {
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}
void SearchResByFullName(Resident **hashtable, char *fullName)
{
	bool flag = 0;
	for (int i = 0; i < 100; i++) {
		while (hashtable[i]->passport != "") {
			if (strcmp(fullName, hashtable[i]->fullName) == 0) {
				cout << endl << "Постоялец найден: \nНомер паспорта: " << hashtable[i]->passport << endl << "ФИО: " << hashtable[i]->fullName 
					<< endl << "Год рождения: " << hashtable[i]->year << endl << "Цель приезда: " << hashtable[i]->destination << endl 
					<< "Адрес: " << hashtable[i]->address << endl;
				flag = 1;
			}
			if (hashtable[i]->next == NULL) {
				break;
			}				
			hashtable[i] = hashtable[i]->next;
		}
	}
	if (flag == 0) {
		cout << endl << "Постоялец не найден" << endl << endl;
	}
}
void SearchResByPassport(InOut *inOutSearch, Resident **hashtable, char *passport)
{
	InOut *tempInOut = new InOut;
	tempInOut = inOutSearch;
	bool flag = 0;
	for (int i = 0; i < 100; i++) {
		while (hashtable[i]->passport != "") {
			if (strcmp(passport, hashtable[i]->passport) == 0) {
				cout << endl << "Постоялец найден: \nНомер паспорта: " << hashtable[i]->passport << "\nФИО: " << hashtable[i]->fullName << endl
					<< "Год рождения: " << hashtable[i]->year << endl << "Цель приезда: " << hashtable[i]->destination << endl
					<< "Адрес проживания: " << hashtable[i]->address << endl;
				while (tempInOut) {
					if (strcmp(tempInOut->regPas, hashtable[i]->passport) == 0) {
						cout << "Постоялец проживает в номере: " << tempInOut->regNum << endl;
					}
					tempInOut = tempInOut->next;
				}
				flag = 1;
				cout << endl;
			}
			if (hashtable[i]->next == NULL) {
				break;
			}
			hashtable[i] = hashtable[i]->next;
		}
	}
	if (flag == 0) {
		cout << endl << "Постоялец с таким паспортом не найден" << endl << endl;
	}
}

void PrintRes(Resident **hashtable)
{
	for (int i = 0; i < 300; i++) {
		if (hashtable[i + 1] == NULL) {
			break;
		}
		if (hashtable[i]->year == 0) {
			cout << i + 1 << ": " << endl;
		}
		else {
			if (hashtable[i]->next == NULL) {
				cout << endl << i + 1 << ": паспорт: " << hashtable[i]->passport << ", ФИО: " << hashtable[i]->fullName << ", год рождения: " 
					<< hashtable[i]->year << ", цель приезда: " << hashtable[i]->destination << ", адрес: " << hashtable[i]->address << endl;
			}
			else {				
				cout << endl << i + 1 << ": паспорт: " << hashtable[i]->passport << ", ФИО: " << hashtable[i]->fullName << ", год рождения: "
					<< hashtable[i]->year << ", цель приезда: " << hashtable[i]->destination << ", адрес: " << hashtable[i]->address << endl
					<< endl << i + 2 << ": паспорт: " << hashtable[i]->next->passport << ", ФИО: " << hashtable[i]->next->fullName 
					<< ", год рождения: " << hashtable[i]->next->year << ", цель приезда: " << hashtable[i]->next->destination 
					<< ", адрес: " << hashtable[i]->next->address << endl;
				i++;				
			}
		}
		cout << endl;		
	}
	cout << endl;
}

void FreeRes(Resident **hashtable)
{
	for (int i = 0; i < 100; i++) {
		strcpy_s(hashtable[i]->address, "");
		strcpy_s(hashtable[i]->destination, "");
		strcpy_s(hashtable[i]->fullName, "");
		strcpy_s(hashtable[i]->passport, "");
		hashtable[i]->year = 0;
		hashtable[i]->next = NULL;
	}
}

InOut* AddResToInn(InOut *workingReg, InOut *join)
{	
	if (workingReg->regNum == "") {
		for (int i = 0; i < 12; i++) {
			workingReg->regPas[i] = join->regPas[i];
			workingReg->enteringDate[i] = join->enteringDate[i];
			workingReg->leavingDate[i] = join->leavingDate[i];
		}
		workingReg->regNum = join->regNum;
		workingReg->next = workingReg->prev = NULL;
	}
	else {
		InOut *tmp = new InOut;
		for (int i = 0; i < 12; i++) {
			tmp->regPas[i] = join->regPas[i];			
			tmp->enteringDate[i] = join->enteringDate[i];
			tmp->leavingDate[i] = join->leavingDate[i];
		}
		tmp->regNum = join->regNum;
		tmp->next = NULL;
		while (workingReg->next != NULL) {
			workingReg = workingReg->next;
		}
		workingReg->next = tmp;
	}
	return workingReg;
}

InOut* DelResFromInn(InOut *workingReg, InOut *join)
{
	InOut *pr = workingReg->prev;
	InOut *nx = workingReg->next;
	while (workingReg) {
		if (strcmp(workingReg->regPas, join->regPas) == 0) {
			if (nx == NULL && pr == NULL) {
				strcpy_s(workingReg->enteringDate, "");
				strcpy_s(workingReg->leavingDate, "");
				strcpy_s(workingReg->regPas, "");
				workingReg->regNum = "";
				workingReg = NULL;
				return workingReg;
			}
			else if (nx != NULL && pr == NULL) {
				nx->prev = workingReg->prev;
				delete workingReg;
				return nx;
			}
			else if (nx == NULL && pr != NULL) {
				pr->next = workingReg->next;
				delete workingReg;
				return pr;
			}
			else if (nx != NULL && pr != NULL) {
				nx->prev = workingReg->prev;
				nx->next = workingReg->next;
				delete workingReg;
				return nx;
			}
		}
		workingReg = workingReg->next;
	}
	return workingReg;
}

void RoomSwitchStatus(InOut *entrance, Inn *&workingRoom, string roomNum)
{
	if (workingRoom) {
		if (workingRoom->roomNum == roomNum) {
			if (workingRoom->seats == 1) {
				if (workingRoom->isRoomEmpty == 1) {
					workingRoom->isRoomEmpty = 0;
				}
				else {
					workingRoom->isRoomEmpty = 1;
				}
			}
			else {
				if (strcmp(entrance->enteringDate, "") == 0) {
					workingRoom->seats++;
				}
				else {
					workingRoom->seats--;
				}
			}
		}
		else if (workingRoom->roomNum > roomNum) {
			RoomSwitchStatus(entrance, workingRoom->left, roomNum);
		}
		else if (workingRoom->roomNum > roomNum) {
			RoomSwitchStatus(entrance, workingRoom->right, roomNum);
		}
	}
	else {
		return;
	}
}

string CheckRoomNum(string x)
{
	while ((stoi(x) <= 0) || (stoi(x) > 999)) {
		cout << "Номер комнаты не может быть меньше нуля или иметь больше трёх цифр" << endl;
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cin >> x;
	}
	return x;
}
char CheckPassport(char *x)
{
	while (strlen(x) != 11) {
		cout << "Неверно введён паспорт. Формат ввода: \"0000-000000\"" << endl;
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cin >> x;
	}
	for (int i = 0; i < 11; i++) {
		if ((x[4]) != '-') {
			cout << "Неверно введён паспорт. Формат ввода: \"0000-000000\"" << endl;
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cin >> x;
		}
		if ((i != 4) && (isdigit(x[i]) == 0)) {
			cout << "Неверно введён паспорт. Формат ввода: \"0000-000000\"" << endl;
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cin >> x;
		}
	}
	return *x;
}
char CheckDate(char *x)
{
	while (strlen(x) != 10) {
		cout << "Неверно введена дата. Формат ввода: \"ДД.ММ.ГГГГ\"" << endl;
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cin >> x;
	}
	for (int i = 0; i < 10; i++) {
		if ((x[2] != '.') || (x[5] != '.')) {
			cout << "Неверно введена дата. Формат ввода: \"ДД.ММ.ГГГГ\"" << endl;
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cin >> x;
		}
		else if ((i != 2 && i != 5) && isdigit(x[i]) == 0) {
			cout << "Неверно введена дата2. Формат ввода: \"ДД.ММ.ГГГГ\"" << endl;
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			cin >> x;
		}
	}
	return *x;
}
int CheckYear(int x)
{
	while (cin.fail()) {
		cout << "Значение должно быть числом" << endl;
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cin >> x;
	}
	return x;
}