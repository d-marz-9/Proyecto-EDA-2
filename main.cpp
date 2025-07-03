#include <iostream>
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
class tablaHash{
private:
    Persona** tabla; //array dinamico de punteros | https://www.geeksforgeeks.org/cpp/creating-array-of-pointers-in-cpp/
    int tama;
    int colis; //para contar colisiones
    int elems;
public:
    tablaHash(int t=3000){
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

    ~tablaHash(){
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

//----- FIN DE HASHES -----

//----- USO DE HEAPS -----


//----- FIN DE HEAPS -----

//----- USO DE AVL รณ RED BLACK TREES -----


//----- FIN DE USO DE AVL รณ RED BLACK TREES -----

int main()
{
    cout << "Meow!" << endl;
    return 0;
}
