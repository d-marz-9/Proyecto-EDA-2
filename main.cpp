#include <iostream>
#include <limits> //https://www.geeksforgeeks.org/how-to-validate-user-input-in-cpp/
#include <string> //manipulacion de strings

using namespace std;

//----- CLASE PERSONA -----
class Persona{
private:
    int dni;
    string nombres;
    string apellidos;
public:
    Persona(int d, string n, string a){
        dni = d;
        nombres = n;
        apellidos = a;
    }

    int getDni(){
        return dni;
    }

    string getApeNombres(){
        return nombres+" "+apellidos;
    }

    void impPersona() {
        cout<<"Datos completos: "<<endl;
        cout<<"DNI: "<<dni<<endl<<"Nombres completos: "<<getApeNombres()<<endl;
    }
};


//----- USO DE HASHES -----


//----- FIN DE HASHES -----

//----- USO DE HEAPS PARA MANEJAR PERSONAS Y PRIORIDAD -----
class maxHeap{
private:
    int* heap;
    int tam;
    int ultPos;
public:
    maxHeap(int n){
        heap = new int[n];
        tam = n;
        ultPos = 0;
    }
    void insertarHP(int dato){
        if(ultPos+1 >= tam){
            cout<<"No se pueden agregar mas invitados :(("<<endl;
            return;
        }
        ultPos++;
        heap[ultPos] = dato;
        int i = ultPos;
        //int swapCount = 0;
        while(i>1&&heap[i] > heap[i/2]){
            swap(heap[i],heap[i/2]);
            i = i/2;
            //swapCount++;
        }
        //codigo para imprimir cuando se hace un swap
        /* cout<<"Insertando..."<<dato<<endl;
        cout<<swapCount<<" swaps hechos"<<endl; */
    }

    //impresion base, se tiene que modificar par los casos que pide el proyecto
    void imprimirHP(){
        for (int i=0; i<=ultPos; i++){
            cout<<heap[i]<<"  ";
        }
        cout<<endl;
    }

    //Revisa si esta vacio
    bool vacioHP(){
        if(ultPos==0){
            return true;
        }
        return false;
    }

    //Tamaño de heap
    int tamaHP(){
        if(ultPos!=0){
            return ultPos;
        }
    }
};

maxHeap* crearHeap(){
    int cantidad;
    cout<<"Insertar la cantidad maxima de participantes en este evento: ";
    cin>>cantidad;

    if(cantidad<=0){
        cout<<"Capacidad invalida, intentalo otra vez"<<endl;
        return crearHeap();
    }

    maxHeap* newHeap = new maxHeap(cantidad+1);
    char resp;
    cout<<"Heap para tu nuevo evento con "<<cantidad<<" personas. Es correcto? (Y/N)"<<endl;
    if(resp!='Y' || resp!='N'){
        cout<<"Respuesta invalida. Reiniciando creacion..."<<endl;
        delete newHeap;
        return crearHeap();
    }
    if(resp=='N'){
        cout<<"Entendido. Reiniciando..."<<endl;
        delete newHeap;
        return crearHeap();
    }
    return newHeap;
}



//----- FIN DE USO DE HEAPS -----

//----- USO DE AVL ó RED BLACK TREES -----


//----- FIN DE USO DE AVL ó RED BLACK TREES -----

int eleccionTipoIngresante(){
    int opcionA;
    cout<<endl<<"Elegir una opcion: "<<endl;
    cout<<"1. Organizador"<<endl;
    cout<<"2. Participante"<<endl;
    cout<<"3. Cancelar"<<endl;
    cout<<endl<<"Opcion: ";
    if (!(cin>>opcionA)){
        cout<<endl<<"Entrada no valida, elija nuevamente"<<endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); //esto descarta la opcion no valida
        return eleccionTipoIngresante();
    }
    if (opcionA == 1){
        cout<<endl<<"Ingresando como organizador..."<<endl;
        return 1;
    } else if (opcionA == 2){
        cout<<endl<<"Ingresando como participante..."<<endl;
        return 2;
    } else if (opcionA == 3){
        cout<<endl<<"Tenga buen dia :)"<<endl;
        return 0;
    } else {
        cout<<endl<<"Opcion invalido, elija nuevamente"<<endl;
        return eleccionTipoIngresante();
    }
    cout<<endl;
}

int eleccionEventos(int tipoIngresante){
    int opcionB;
    if (tipoIngresante == 1){
        cout<<"\nBienvenido Organizador.\n";
        cout<<"1. Crear evento"<<endl;
        cout<<"2. Ver eventos publicados"<<endl;
        cout<<"3. Ver sus eventos"<<endl; //los que organiza
        cout<<"4. Modificar un eventos"<<endl;
        cout<<"5. Eliminar un evento"<<endl;
        //posible opcion: retirarse de organizar un evento, en caso de hacerse, el siguiente organizador sera uno de los miembros VIP
        cout<<"6. Salir"<<endl;
        cout<<"Opcion: ";
        cin>>opcionB;

        switch (opcionB){
        case 1:
            cout<<"Creador de eventos:"<<endl;
            /*Funcion para crear un evento: Se ingresara el nombre del evento, cuantos participantes habran (especificar cuantos
             en cada prioridad), a que hora sera el evento (horario 24horas), */
            break; //breaks por ahora
        case 2:
            cout<<"Lista de eventos:"<<endl;
            //Funcion que muestra en una lista todos los eventos
            break;
        case 3:
            cout<<"Sus eventos:"<<endl;
            //Funcion que muestra eventos hechos por organizador
            break;
        case 4:
            cout<<"Seleccionar evento a modificar:"<<endl;
            /*Funcion para modificar hora de evento, nombre de evento, cantidad personas en evento (en caso de reducir cantidad
             de personas, los primeros en reducirse son los asistentes comunes)*/
            break;
        case 5:
            cout<<"Seleccionar evento a eliminar:"<<endl;
            //Funcion que elimina un evento, elimina todos los asistentes tambien
            break;
        case 6:
            cout<<"Saliendo..."<<endl;
            return eleccionTipoIngresante();
        default:
            cout<<"Opcion no valida";
            return eleccionEventos(tipoIngresante);
        }
    } else if (tipoIngresante == 2){
        cout<<"\nBienvenido Participante.\n";
        cout<<"1. Buscar eventos"<<endl;
        cout<<"2. Ver sus eventos"<<endl;
        cout<<"3. Retirarse de un evento"<<endl;
        cout<<"4. Salir"<<endl;
        cout<<"Opcion: ";
        cin>>opcionB;

        switch (opcionB){
        case 1:
            cout<<"Lista de eventos:"<<endl;
            //Funcion que muestra eventos y da prompt para ingresar a evento
            break;
        case 2:
            cout<<"Sus eventos:"<<endl;
            //Eventos a la que esta inscrito
            break;
            break;
        case 3:
            cout<<"Seleccionar evento a retirarse:"<<endl;
            //Funcion de retirarse de un evento
            break;
        case 4:
            cout<<"Saliendo..."<<endl;
            return eleccionTipoIngresante();
        default:
            cout<<"Opcion no valida";
            return eleccionEventos(tipoIngresante);
        }
    }

    return tipoIngresante;
}

int main()
{
    cout<<endl<<endl<<"Bienvenido al organizador de eventos!!!"<<endl;
    cout<<"      -Patrocinado por el grupo 5 y SuniCoito"<<endl;
    int tipo;
    do {
        tipo = eleccionTipoIngresante();
        if (tipo != 0){
            eleccionEventos(tipo);
        }
    } while (tipo != 0);
    cout<<"Disfrute el evento!"<<endl;

    return 0;
}
