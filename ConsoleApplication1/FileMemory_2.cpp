#include "stdafx.h"
#include "FileMemory_2.h"
#include <atlstr.h>

FileMemory_2::FileMemory_2()
{
	init();
}


FileMemory_2::~FileMemory_2()
{
}

void FileMemory_2::init()
{
	// Создаем или открываем семафоры

	hSem_put = createOrOpenSemaphore("SemPut_1", 1);
	hSem_get = createOrOpenSemaphore("SemGet_1", 0);
	createFile("File_1");
}

void FileMemory_2::put(string str)
{
	WaitForSingleObject(hSem_put, INFINITE);	// сюда можем зайти, только если в файле пусто

												// Сереализуем объект в строку
												// ...
	cout << " ща запишем : " << str << endl;
												/// Пишем строку в файл
	vector<CHAR> buffer(str.begin(), str.end());
	buffer.push_back(_T('\0'));
	CHAR* p = &buffer[0];

	SIZE_T cbData = (str.size() + 1) * sizeof(CHAR);

	//CopyMemory((PVOID)Buffer, param, (_tcslen(param) * sizeof(TCHAR)));
	CopyMemory((PVOID)Buffer, p, cbData);


	ReleaseSemaphore(hSem_get, 1, NULL);		// освобождаем семафор на чтение, теперь с файла можно читать, т.к. мы записали в него инфу
}


string FileMemory_2::get()
{
	WaitForSingleObject(hSem_get, INFINITE);		// 

													// Изымаем из файла данные
	std::string result((char*)Buffer);

	// Десериализуем объект из строки
	// ...

	ReleaseSemaphore(hSem_put, 1, NULL);			// 
	return result;
}

// Переводит строку в нужный формат для создания семафора
wstring FileMemory_2::s2ws(const std::string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

// Создает системный семафор с заданным именем
HANDLE FileMemory_2::createOrOpenSemaphore(string hSemName, int startStatus)
{
	HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, (s2ws(hSemName)).c_str());
	if (sem == NULL) {
		cout << "Семафор не открылся, Создаем " << endl;
		sem = CreateSemaphore(
			NULL,// нет атрибута
			startStatus,// начальное состояние
			1,// максимальное состояние
			(s2ws(hSemName)).c_str() //  имени
		);
	}
	else {
		cout << "Семафор открылся!  =) " << endl;
	}
	return sem;
}

// Создаем файл отображаемый на память
void FileMemory_2::createFile(string name)
{
	FileMem = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		// все права на файл, кроме FILE_MAP_EXECUTE
		false,     //  handle  не наследуется при CreateProcess
		(s2ws(name)).c_str());
	if (FileMem == NULL) {
		FileMem = CreateFileMapping(
			(HANDLE)0xFFFFFFFF,
			//   INVALID_HANDLE_VALUE --- СОЗДАЕМ НОВЫЙ
			NULL,  // LPSECURITY_ATTRIBUTES 
			PAGE_READWRITE,    //  вид доступа к данным
			0, 4096,   // размер
			(s2ws(name)).c_str());
	}

	if (FileMem == NULL)
		cout << "error: FileMem == NULL \n" << endl;

	char* buf = NULL;
	//  Буфер для записи  - чтения данных
	if (FileMem != NULL) {
		buf = (char*)MapViewOfFile(
			FileMem,   // Handle файла
			FILE_MAP_ALL_ACCESS,
			0, 0,  // смещение
			4096);   // длина данных
	}
	else {
		printf("error: FILE_MAP \n"); // Все плохо!!!!
	}

	this->Buffer = buf;
}
