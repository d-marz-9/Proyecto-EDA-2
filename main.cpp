#include <iostream>
#include <fstream> //para cargar desde un archivo txt
#include <string> //manipulacion de strings
using namespace std;

//----- CLASE PERSONA -----
class Persona{
private:
    int dni;
    string nombre;
    string apellido;
    string zona;
    int prioridad;
public:
    Persona(int d, string n, string a, string z, int p){
        dni = d;
        nombre = n;
        apellido = a;
        zona = z;
        prioridad = p;
    }
    int getDNI(){
        return dni;
    }
    int getPriori(){
        return prioridad;
    }
    string fullNombres(){
        return nombre+ " " +apellido;
    }
    string getZona(){
        return zona;
    }
    void datosPersona(){
        cout<<"Datos completos de: "<<endl;
        cout<<"DNI: "<<dni<<endl;
        cout<<"Nombres completos: "<<fullNombres()<<endl;
        cout<<"Nivel de prioridad: "<<prioridad<<endl;
    }
};

//----- USO DE HASHES -----
class TablaHash{
private:
    Persona** tabla; //array dinamico de punteros | https://www.geeksforgeeks.org/cpp/creating-array-of-pointers-in-cpp/
    int tama;
    int colis; //para contar colisiones
    int elems;
public:
    TablaHash(int t=25000){
        colis = 0; //contador inicializado
        tama = t;
        tabla = new Persona*[tama];
        elems = 0;
        for (int i=0; i<tama; i++){
            tabla[i] = nullptr;
        }
    }
    int funcionHash(int dni){
        int r = dni % tama;
        if (r<0)
            r = r+tama;
        return r;
    }

    ~TablaHash(){
        for (int i=0; i<tama; i++){
            delete tabla[i];
        }
        delete[] tabla;
    }

    bool insertarHS(Persona* per){
        int dni = per->getDNI();
        int pos = funcionHash(dni);
        int intentos = 0;

        while (tabla[pos]!=nullptr && intentos<tama){
            if (tabla[pos]->getDNI()==dni){
                return false; //saltarse duplicados, puede cambiarse en un futuro
            }
            pos = (pos+1)%tama;
            intentos++;
            //colis++; usar si se cuentan colisiones
        }
        if (intentos<tama){
            tabla[pos] = per;
            elems++;
            //rehashing stuff
            if (factorCarga()>0.7){
                cout<<"Factor de carga: "<<factorCarga()<<". Realizando rehashing de manera automatica..."<<endl;
                rehashing();
            }
            return true;
        } else {
            cout<<"Insersion fallida. Tabla llena."<<endl;
            return false;
        }
    }

    Persona* buscar(int dni){
        int pos = funcionHash(dni);
        int intentos = 0;

        while (tabla[pos]!=nullptr && intentos<tama){
            if (tabla[pos]->getDNI()==dni){
                return tabla[pos];
            }
            pos = (pos+1)%tama;
            intentos++;
        }
        cout<<"no encontrado"<<endl;
        return nullptr; //si no se encuentra
    }

    bool validar(int dni){
        return buscar(dni) != nullptr;
    }

    void rehashing(){
        int newTama = tama + 10000;
        Persona** newTabla = new Persona*[newTama];
        for (int i=0; i<newTama; i++){
            newTabla[i] = nullptr;
        }

        int oldTama = tama;
        Persona** oldTabla = tabla;

        tabla = newTabla;
        tama = newTama;
        elems = 0;

        for (int i=0; i<oldTama; i++){
            if (oldTabla[i]!=nullptr){
                insertarHS(oldTabla[i]);
            }
        }

        delete[] oldTabla;
        cout<<"Rehashing completo. Tamaño de nueva tabla: "<<tama<<endl;
    }

    float factorCarga(){
        return float(elems)/tama;
    }

    /*int colisiones(){
        return colis;
    }*/

    void printTabla(){
        for (int i=0; i<tama; i++){
            cout<<i<<": ";
            if(tabla[i]==nullptr){
                cout<<"Vacio";
            }else{
                cout<<tabla[i]->getDNI()<<" - "<<tabla[i]->fullNombres();
            }
            cout<<endl;
        }
    }

};

void cargaDeMuchasPersonas(TablaHash* tabla, const string& listaAtendientes){ //https://cplusplus.com/doc/tutorial/files/
    ifstream archivo(listaAtendientes);

    if (!archivo.is_open()){
        cout<<"Error critico al abrir archivo "<<listaAtendientes<<". Revisar."<<endl;
        return;
    }
    int dni, prioridad;
    string nombre, apellido;

    while (archivo>>dni>>nombre>>apellido>>prioridad){
        string zona;
        switch (prioridad){
            case 5:
                zona = "VIP";
                break;
            case 4:
                zona = "Personal Medico";
                break;
            case 3:
                zona = "Personal Seguridad";
                break;
            case 2:
                zona = "Discapacitados";
                break;
            case 1:
                zona = "Publico general";
                break;
            default:
                zona = "Unknown";
                break;
        }
        Persona* per = new Persona(dni, nombre, apellido, zona, prioridad);
        if (!tabla->insertarHS(per)){
            delete per;
        }
    }
    archivo.close();
    cout<<"Informacion de atendientes exitoso."<<endl;

    /* llamado a main, ya sabremos como se hace luego xdddd
    int main() {
        tablaHash* tabla = new tablaHash(3000);
        cargarDesdeArchivo(tabla, "invitados.txt");

        // Optional: test
        tabla->printTabla();

        delete tabla;
        return 0;
    }
    */
}

//----- FIN DE HASHES -----

//----- USO DE HEAPS -----


//----- FIN DE HEAPS -----

//----- USO DE AVL ó RED BLACK TREES -----


//----- FIN DE USO DE AVL ó RED BLACK TREES -----

int main()
{
    cout << "Meow!" << endl;
    return 0;
}
