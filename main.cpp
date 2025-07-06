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
    int horaIngreso;
public:
    Persona(int d, string n, string a, string z, int p, int hI){
        dni = d;
        nombre = n;
        apellido = a;
        zona = z;
        prioridad = p;
        horaIngreso = hI;
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
    void setPriori(int newP){
        prioridad = newP
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
        cout<<"Rehashing completo. TamaÃ±o de nueva tabla: "<<tama<<endl;
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
    ifstream archivo(listaAtendientes); //toma lista de archivo
    ofstream archivoIDs("IDsRegistrados"); //nuevo archivo para validar atendientes en heaps

    if (!archivo.is_open()){
        cout<<"Error critico al abrir archivo "<<listaAtendientes<<". Revisar."<<endl;
        return;
    }
    if (!archivoIDs.is_open()){
        cout<<"Error critico en creacion de archivo."<<endl;
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
            archivoIDs<<dni<<endl; //solo id se guarda
        } else {
            delete per;
        }
    }
    archivo.close();
    archivoIDs.close();
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
class maxHeap{
private:
    Persona** heap;
    int tam;
    int ultPos;
public:
    maxHeap(int n){
        heap = new Persona*[n+1]; //porque se empieza de 1, no 0
        tam = n+1;
        ultPos = 0;
    }
    ~maxHeap(){
        delete[] heap;
    }
    bool vacioHP(){ //revisar si esta vacio, no confundir con el otro
        if(ultPos==0){
            return true;
        }
        return false;
    }

    void insertarHP(Persona* nuevo){
        if (ultPos+1 >= tam){
            cout<<"No se aceptan mas invitado"<<endl;
            return;
        }
        ultPos++;
        heap[ultPos] = nuevo;
        int i = ultPos;
        //int swapCount = 0;
        while(i>1&&heap[i]->getPriori() > heap[i/2]->getPriori()){
            swap(heap[i],heap[i/2]);
            i = i/2;
            //swapCount++; esto es para contar la cantidad de swaps xd
        }
        //cout<<"Usuario agregado"<<endl:
        //cout<<swapCount<<" cambios hechos"<<endl;
    }

    bool vaciarHP(){ //este es el otro, este vacia todo el heap, NO CONFUNDIR
            return ultPos==0;
    }

    void extraerMaximo(){ //sacar el primero del heap
        if (vacioHP()){
            cout<<"Heap vacio :("<<endl;
            return nullptr;
        }

        Persona* maximo = heap[1]; //para poder retornar un valor
        heap[1] = heap[ultPos];
        ultPos--;

        int i = 1;
        while (2*i <= ultPos){
            int hIzq = 2*i;
            int hDer = 2*i+1;
            int hMayor = i;

            if (hIzq<=ultPos && heap[hIzq]->getPriori() > heap[hMayor]->getPriori()){
                hMayor = hIzq;
            }
            if (hDer<=ultPos && heap[hDer]->getPriori() > heap[hMayor]->getPriori()){
                hMayor = hDer;
            }

            if (hMayor != i){
                swap(heap[i],heap[hMayor]);
                pos = hMayor;
            } else {
                break;
            }
        }
        return maximo; //el valor returnado
    }

    void actualizarPriori(int dni, int newPriori){
        int pos = -1;

        for (int i=1; i<=ultPos; i++){
            if (heap[i]->getDNI()==dni){
                pos = i;
                break;
            }
        }
        if (pos == -1){
            cout<<"Esta persona no se encuentra en la cola"<<endl;
            return;
        }
        int oldPriori = heap[pos]->getPriori();
        heap[pos]->setPriori(newPriori);

        if (newPriori > oldPriori){ //usamos pos y no i porque i se uso para el for
            while (pos>i&&heap[pos]->getPriori() > heap[pos/2]->getPriori()){
                swap(heap[pos],heap[pos/2]);
                pos = pos / 2;
            }
        } else if(newPriori<oldPriori){
            while (2*pos <= ultPos){
                int hIzq = 2*pos;
                int hDer = 2*pos+1;
                int hMayor = pos;

                if (hIzq<=ultPos && heap[hIzq]->getPriori() > heap[hMayor]->getPriori()){
                    hMayor = hIzq;
                }
                if (hDer<=ultPos && heap[hDer]->getPriori() > heap[hMayor]->getPriori()){
                    hMayor = hDer;
                }

                if (hMayor != i){
                    swap(heap[i],heap[hMayor]);
                    pos = hMayor;
                } else {
                    break;
                }
            }
        }
        cout<<"Prioridad actualizada"<<endl;
    }
};

void cargarPersonasAgainHeap(maxHeap* heap, TablaHash* tabla, const string& IDsRegistrados){
    ifstream archivo(IDsRegistrados);
    if (!archivo.is_open()){
        cout<<"Error al abrir el archivo de IDs para el heap"<<endl;
        return;
    }
    int dni;
    while (archivo>>dni){
        Persona* per = tabla->buscar(dni);
        if (per != nullptr){
            heap->insertarHP(per);
        } else {
            cout<<"DNI "<<dni<<" no fue encontrado en la tabla. Denegado."<<endl;
        }
    }
    archivo.close();
    cout<<"Verificacion completa"<<endl;
}

//----- FIN DE HEAPS -----

//----- USO DE AVL Ã³ RED BLACK TREES -----


//----- FIN DE USO DE AVL Ã³ RED BLACK TREES -----

int main()
{
    cout << "Meow!" << endl;
    return 0;
}
