#include<iostream>
#include<string>
#include<fstream>
using namespace std;

const int MaxMov = 6;
const int MaxTec = 100;

typedef string tSec[MaxMov];

struct tSecuencia {
	tSec secuencia;
	int contador;
};

struct tTecnica {
	string nombre;
	int impacto = 0;
	int numeroMov = 0;
	tSecuencia secuencia;
};

typedef tTecnica tListaTecnicas[MaxTec];

struct tListaTec {
	tListaTecnicas tecnicas;
	int contador = 0;
};

bool cargarTecnicas(string nombreArchivo, tListaTec& listaTec);
void cargarSecuencias(istream& archivo, tSecuencia& secuencia);
int menu();
void mostrarTec(const tListaTec& listaTec);
void mostrarImp(const tListaTec& listaTec, int impacto);
void lucha(string nombreLucha, const tListaTec& listaTec);

int main() {
	tListaTec listaTec;
	int impacto = 0;
	int opcion = menu();
	while (opcion != 4) {
		switch (opcion) {
		case 0:
			cargarTecnicas("tecnicas.txt", listaTec);
			break;
		case 1:
			mostrarTec(listaTec);
			break;
		case 2:
			cout << "IMPACTO: ";
			cin >> impacto;
			mostrarImp(listaTec, impacto);
			break;
		case 3:
			lucha("lucha.txt", listaTec);
			break;
		}
		opcion = menu();
	}
	
}

int menu() {
	int eleccion = 0;
	cout << "0. CARGAR LISTA TECNICAS" << endl;
	cout << "1. MOSTRAR LISTA TECNICAS" << endl;
	cout << "2. MOSTRAR TECNICAS QUE CAUSAN DETERMINADO IMPACTO" << endl;
	cout << "3. PELEA" << endl;
	cout << "4. SALIR" << endl;
	cout << endl << "ELIGE UNA OPCION: ";
	cin >> eleccion;
	return eleccion;
}

bool cargarTecnicas(string nombreArchivo, tListaTec& listaTec) {
	ifstream tecnicasFile;
	tecnicasFile.open(nombreArchivo);
	bool aperturaArchivo = false;
	
	if (tecnicasFile.is_open()) {
		tecnicasFile >> listaTec.contador;
		for (int i = 0; i < listaTec.contador; i++) {
			tecnicasFile >> listaTec.tecnicas[i].nombre;
			tecnicasFile >> listaTec.tecnicas[i].impacto;
			cargarSecuencias(tecnicasFile, listaTec.tecnicas[i].secuencia);
		}
		aperturaArchivo = true;
	}
	return aperturaArchivo;
}

void cargarSecuencias(istream &archivo, tSecuencia &secuencia) {
	archivo >> secuencia.contador;
	for (int i = 0; i < secuencia.contador; i++) {
		archivo >> secuencia.secuencia[i];
	}
}

void mostrarTec(const tListaTec& listaTec) {
	cout << "LISTADO DE TECNICAS" << endl;
	for (int i = 0; i < listaTec.contador; i++) {
		cout << listaTec.tecnicas[i].nombre << " [" << listaTec.tecnicas[i].impacto << "]" << " -> ";
		for (int j = 0; j < listaTec.tecnicas[i].secuencia.contador; j++) {
			cout << listaTec.tecnicas[i].secuencia.secuencia[j] << " ";
		}
		cout << endl;
	}
}

void mostrarImp(const tListaTec&listaTec, int impacto) {
	cout << "LISTADO DE TECNICAS POR IMPACTO " << impacto << ":" << endl;
	for (int i = 0; i < listaTec.contador; i++) {
		if (listaTec.tecnicas[i].impacto == impacto) {
			cout << listaTec.tecnicas[i].nombre << " [" << listaTec.tecnicas[i].impacto << "]" << " -> ";
			for (int j = 0; j < listaTec.tecnicas[i].secuencia.contador; j++) {
				cout << listaTec.tecnicas[i].secuencia.secuencia[j] << " ";
			}
			cout << endl;
		}
	}
}

bool compararSecuencias(const tSecuencia& tecnica, const tSecuencia& secuencia) {
	bool comparacion = false;
	for (int i = 0; i < tecnica.contador; i++) {
		for (int j = 0; j < secuencia.contador; j++) {
			if (tecnica.secuencia[i] == secuencia.secuencia[j]) {
				comparacion = true;
			}
		}
	}

	return comparacion;

}

int calcularImpacto(const tTecnica & tecnica, const tListaTec&listaTec) {
	int impacto = 0;
	for (int i = 0; i < listaTec.contador; i++) {
		if (tecnica.nombre == listaTec.tecnicas[i].nombre &&  compararSecuencias(tecnica.secuencia, listaTec.tecnicas[i].secuencia)) {
			impacto = listaTec.tecnicas[i].impacto;
		}
	}
	return impacto;
}

void lucha(string nombreLucha, const tListaTec& listaTec) {
	ifstream peleas;
	peleas.open(nombreLucha);
	int contador = 0;
	tTecnica tecnica;
	string nombreJugador;
	string j1;
	string j2;
	int imp1 = 0;
	int imp2 = 0;
	int impactoAc = 0;
	bool bucle = true;
	

	if (peleas.is_open()) {
		peleas >> contador;
		for (int i = 0; i < contador; i++) {
			peleas >> nombreJugador;
			if (nombreJugador == j1 || bucle) {
				j1 = nombreJugador;
				peleas >> tecnica.nombre;
				peleas >> tecnica.secuencia.contador;
				cout << j1 << ": " << tecnica.nombre << " -> ";
				for (int j = 0; j < tecnica.secuencia.contador; j++) {
					peleas >> tecnica.secuencia.secuencia[j];
					cout << tecnica.secuencia.secuencia[j];
				}
				impactoAc = calcularImpacto(tecnica, listaTec);
				if (impactoAc != 0) {
					cout << " +" << impactoAc << endl;
					imp1 = imp1 + impactoAc;
				}
				else {
					cout << " Fallo!" << endl;
				}
				bucle = false;
			}
			else {
				j2 = nombreJugador;
				peleas >> tecnica.nombre;
				peleas >> tecnica.secuencia.contador;
				cout << j2 << ": " << tecnica.nombre << " -> ";
				for (int j = 0; j < tecnica.secuencia.contador; j++) {
					peleas >> tecnica.secuencia.secuencia[j];
					cout << tecnica.secuencia.secuencia[j];
				}
				impactoAc = calcularImpacto(tecnica, listaTec);
				if (impactoAc != 0) {
					cout << " +" << impactoAc << endl;
					imp2 = imp2 + impactoAc;
				}
				else {
					cout << " Fallo!" << endl;
				}
			}
		}
		if (imp1 < imp2) {
			cout << "HA GANADO EL JUGADOR 2" << endl;
		}
		else {
			cout << "HA GANADO EL JUGADOR 1" << endl;
		}
	}
	else {
		cout << "NO SE HA ENCONTRADO EL ARCHIVO DE PELEAS" << endl;
	}
}