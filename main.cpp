#include <iostream>

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


//----- FIN DE HASHES -----

//----- USO DE HEAPS -----


//----- FIN DE HEAPS -----

//----- USO DE AVL ó RED BLACK TREES -----


//----- FIN DE USO DE AVL ó RED BLACK TREES -----

int main(){
    cout << "Meow!" << endl;
    return 0;
}
