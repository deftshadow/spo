#include <Windows.h>
#include <fstream>
#include <string>
#include <iostream>

using namespace std; 

#define NUMBER_OF_BYTES_IN_SECTOR_OFFSET 0x0B //по этому смещению находится размер сектора
#define NUMBER_OF_SECTORS_IN_SECTION_FIRST_OFFSET 0x13 //по этому смещению находится количество секторов если оно меньше 65535
#define NUMBER_OF_SECTORS_IN_SECTION_SECOND_OFFSET 0x20 //по этому смещению находится количество секторов, если их больше 65535

string drivePath = "\\\\.\\D:"; 


UINT getNumberOfBytesInSector() {
	short int numberOfBytes; //так как размер этого блока 2 байта

	ifstream infile(drivePath.c_str(), ios::in | ios::binary); // полкючаемся к файлу через поток, и указываем, что он бинарный(in - открыть для чтения)
	if (!infile) {
		cerr << "ERROR! Device input could not be set!\n";
	}

	infile.ignore(NUMBER_OF_BYTES_IN_SECTOR_OFFSET); //не читаем до того момента, пока не начнется блок с размером сектора?

	infile.read((char*)&numberOfBytes, sizeof(short int)); //читаем 2 байта инфы
	if (infile.gcount() == 0) {
		cerr << "Data read error!" << endl;
	}

	infile.close(); //stream close

	return (UINT)numberOfBytes;
}

UINT getNumberOfSectorsInSection() {
	short int numberOfSectors; //так как размер этого блока 2 байта
	UINT numberOfSectorsU;

	ifstream infile(drivePath.c_str(), ios::in | ios::binary); // полкючаемся к файлу через поток, и указываем, что он бинарный(in - открыть для чтения)
	if (!infile) {
		cerr << "ERROR! Device input could not be set!\n";
	}

	infile.ignore(NUMBER_OF_SECTORS_IN_SECTION_FIRST_OFFSET); // доходим до первого блока с количеством секторов

	infile.read((char*)&numberOfSectors, sizeof(short int)); //считываем 2 байта информации
	if (infile.gcount() == 0) {
		cerr << "Data read error!" << endl;
	}

	infile.close(); //stream close

	if (numberOfSectors != 0) { // если в первом блоке было пусто, то читаем вторйо блок
		return (UINT)numberOfSectors;
	} else {
		ifstream infile(drivePath.c_str(), ios::in | ios::binary); // полкючаемся к файлу через поток, и указываем, что он бинарный(in - открыть для чтения)
		if (!infile) {
			cerr << "ERROR! Device input could not be set!\n";
		}

		infile.ignore(NUMBER_OF_SECTORS_IN_SECTION_SECOND_OFFSET);// доходим до второго блока с количеством секторов

		infile.read((char*)&numberOfSectorsU, sizeof(UINT)); // считываем 4 байта информации
		if (infile.gcount() == 0) {
			cerr << "Data read error!" << endl;
		}

		infile.close(); //stream close

		return numberOfSectorsU;
	}
}

void getSectorDump(UINT numberOfBytes, UINT sectorNumber) {
	BYTE* dump = new BYTE[numberOfBytes];

	ifstream infile(drivePath.c_str(), ios::in | ios::binary); // полкючаемся к файлу через поток, и указываем, что он бинарный(in - открыть для чтения)
	if (!infile) {
		cerr << "ERROR! Device input could not be set!\n";
	}

	infile.ignore(static_cast<__int64>(numberOfBytes) * sectorNumber);

	infile.read((char*)dump, numberOfBytes);
	if (infile.gcount() == 0) {
		cerr << "Data read error!" << endl;
	}

	infile.close();

	for (int i = 0; i < numberOfBytes / 16; i++) {
		printf("%09X| ", numberOfBytes * sectorNumber + i * 16);
		for (int j = 0; j < 16; j++) {
			if (j == 8) {
				printf(" ");
			}
			printf("%02X ", dump[i * 16 + j]);
		}

		printf(" | ");

		for (int j = 0; j < 16; j++) {
			if (dump[i * 16 + j] != 0x0A && dump[i * 16 + j] != 0x0D && dump[i * 16 + j] != 0x09) {// проверка на невидимые символы по таблице ASCII
				printf("%c", dump[i * 16 + j]);
			}
		}
		printf("\n");
	}
}

int main() {
	UINT numberOfBytes;
	UINT numberOfSectors;

	numberOfBytes = getNumberOfBytesInSector();
	numberOfSectors = getNumberOfSectorsInSection();

	cout << "Number of bytes in sector: " << numberOfBytes << endl;
	cout << "Number of sectors in  section: " << numberOfSectors << endl;

	getSectorDump(numberOfBytes, 0);
}