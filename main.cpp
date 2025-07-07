#include <iostream>
#include <fstream> //para cargar desde un archivo txt
#include <string> //manipulacion de strings
#include <ctime> //Esta librería sirve para registrar la hora de llegada de los asistentes.

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
    int minutosIngreso;
public:
    Persona(int d, string n, string a, string z, int p){
        dni = d;
        nombre = n;
        apellido = a;
        zona = z;
        prioridad = p;
        horaIngreso = -1;
        minutosIngreso = -1;
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
    int getHoraIngreso(){
        return horaIngreso;
    }
    void setHoraIngreso(int hora){
        horaIngreso = hora;
    }
    int getMinutosIngreso(){
        return minutosIngreso;
    }
    void setMinutosIngreso(int minutos){
        minutosIngreso = minutos;
    }
    void set_Hora(int h, int m){
        horaIngreso = h;
        minutosIngreso = m;
    }
    void datosPersona(){
        cout<<"Datos completos de: "<<endl;
        cout<<"DNI: "<<dni<<endl;
        cout<<"Nombres completos: "<<fullNombres()<<endl;
        cout<<"Zona: "<<zona<<endl;
        cout<<"Nivel de prioridad: "<<prioridad<<endl;
        cout<<"Hora de ingreso: ";
        if (horaIngreso != -1){
            cout << horaIngreso << ":" << (minutosIngreso < 10 ? "0" : "") << minutosIngreso << endl;
        }else{
            cout << "No asignada" << endl;
        }
    }
    void setPriori(int newP){
        prioridad = newP;
    }
};

//----- FUNCIÓN PARA OBTENER LA HORA DEL SISTEMA -----
void obtenerHoraYMinutos(int &hora, int &minutos) {
    time_t now = time(0);
    tm *local = localtime(&now);
    hora = local->tm_hour;
    minutos = local->tm_min;
}

//----- USO DE HASHES -----
class TablaHash{
private:
    Persona** tabla; //array dinamico de punteros | https://www.geeksforgeeks.org/cpp/creating-array-of-pointers-in-cpp/
    int tama;
    int elems;
public:
    TablaHash(int t=40000){
        tama = t;
        tabla = new Persona*[tama];
        elems = 0;
        for (int i=0; i<tama; i++){
            tabla[i] = nullptr;
        }
    }
    int funcionHash(int dni){
        int r = dni % tama;
        if (r<0){ //esto hace que acepte negativos
            r = r+tama;
        }
        return r;
    }

    ~TablaHash(){ //eliminar la tabla
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
                return false; //saltarse duplicados
            }
            pos = (pos+1)%tama;
            intentos++;
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
            cout<<"No se aceptan mas invitados :("<<endl;
            return;
        }
        ultPos++;
        heap[ultPos] = nuevo;
        int i = ultPos;
        //int swapCount = 0;
        while(i>1&&heap[i]->getPriori() > heap[i/2]->getPriori()){
            swap(heap[i],heap[i/2]);
            i = i/2;
        }
    }

    bool vaciarHP(){ //este es el otro, este vacia todo el heap, NO CONFUNDIR
            return ultPos==0;
    }

    Persona* extraerMaximo(){ //sacar el primero del heap
        if (vacioHP()){
            cout<<"Heap vacio :("<<endl;
            return nullptr;
        }

        Persona* maximo = heap[1]; //variable que recolecta valor para despues devolverlo
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
                i = hMayor;
            } else {
                break;
            }
        }
        return maximo; //el valor returnado
    }

    void actualizarPriori(int dni, int newPriori){
        int pos = -1; //usamos pos y no usamos i porque i se usa en el for de abajo y no me quiero confundir

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

        if (newPriori > oldPriori){ //
            while (pos>1&&heap[pos]->getPriori() > heap[pos/2]->getPriori()){
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

                if (hMayor != pos){
                    swap(heap[pos],heap[hMayor]);
                    pos = hMayor;
                } else {
                    break;
                }
            }
        }
        cout<<"Prioridad actualizada"<<endl;
    }

    // Método para mostrar los próximos 5 accesos
    void mostrarProximos5(){
        if (vacioHP()){
            cout<<"No hay personas en la cola de acceso"<<endl;
            return;
        }

        cout<<"=== PRÓXIMOS 5 ACCESOS ==="<<endl;
        int contador = 0;
        for (int i = 1; i <= ultPos && contador < 5; i++){
            cout<<contador+1<<". DNI: "<<heap[i]->getDNI()<<" - "<<heap[i]->fullNombres()<<" - Zona: "<<heap[i]->getZona()<<" - Prioridad: "<<heap[i]->getPriori()<<endl;
            contador++;
        }
        cout<<"=========================="<<endl;
    }
};

void cargaDeMuchasPersonas(TablaHash* tabla, const string& listaAtendientes){
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
        if (tabla->insertarHS(per)){
            archivoIDs<<dni<<endl; //meter a nuevo archivo para que heap verifique despues
        } else {
            delete per;
        }
    }
    archivo.close();
    archivoIDs.close();
    cout<<"Informacion de atendientes exitosa."<<endl;
}

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

//----- USO DE ÁRBOL AVL -----
struct NodoAVL {
    Persona* persona;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(Persona* p) {
        persona = p;
        izquierdo = nullptr;
        derecho = nullptr;
        altura = 1;
    }
};

class ArbolAVL {
private:
    NodoAVL* raiz;

    int obtenerAltura(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return nodo->altura;
    }

    int obtenerBalance(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho);
    }

    NodoAVL* rotacionDerecha(NodoAVL* y) {
        NodoAVL* x = y->izquierdo;
        NodoAVL* T2 = x->derecho;

        x->derecho = y;
        y->izquierdo = T2;

        y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
        x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;

        return x;
    }

    NodoAVL* rotacionIzquierda(NodoAVL* x) {
        NodoAVL* y = x->derecho;
        NodoAVL* T2 = y->izquierdo;

        y->izquierdo = x;
        x->derecho = T2;

        x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
        y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;

        return y;
    }

    NodoAVL* insertarAVL(NodoAVL* nodo, Persona* persona) {
        if (nodo == nullptr) {
            return new NodoAVL(persona);
        }

        if (persona->getHoraIngreso() < nodo->persona->getHoraIngreso()) {
            nodo->izquierdo = insertarAVL(nodo->izquierdo, persona);
        } else if (persona->getHoraIngreso() > nodo->persona->getHoraIngreso()) {
            nodo->derecho = insertarAVL(nodo->derecho, persona);
        } else {
            // Misma hora, ordenar por DNI
            if (persona->getDNI() < nodo->persona->getDNI()) {
                nodo->izquierdo = insertarAVL(nodo->izquierdo, persona);
            } else {
                nodo->derecho = insertarAVL(nodo->derecho, persona);
            }
        }

        nodo->altura = max(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho)) + 1;

        int balance = obtenerBalance(nodo);

        // Caso izquierda-izquierda
        if (balance > 1 && persona->getHoraIngreso() < nodo->izquierdo->persona->getHoraIngreso()) {
            return rotacionDerecha(nodo);
        }

        // Caso derecha-derecha
        if (balance < -1 && persona->getHoraIngreso() > nodo->derecho->persona->getHoraIngreso()) {
            return rotacionIzquierda(nodo);
        }

        // Caso izquierda-derecha
        if (balance > 1 && persona->getHoraIngreso() > nodo->izquierdo->persona->getHoraIngreso()) {
            nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
            return rotacionDerecha(nodo);
        }

        // Caso derecha-izquierda
        if (balance < -1 && persona->getHoraIngreso() < nodo->derecho->persona->getHoraIngreso()) {
            nodo->derecho = rotacionDerecha(nodo->derecho);
            return rotacionIzquierda(nodo);
        }

        return nodo;
    }

    void consultarPorFranjaHoraria(NodoAVL* nodo, int horaInicio, int horaFin) {
        if (nodo == nullptr) return;

        consultarPorFranjaHoraria(nodo->izquierdo, horaInicio, horaFin);

        if (nodo->persona->getHoraIngreso() >= horaInicio && nodo->persona->getHoraIngreso() <= horaFin) {
            cout<<"Hora: "<<nodo->persona->getHoraIngreso()<<":"<<nodo->persona->getMinutosIngreso()<<" - DNI: "<<nodo->persona->getDNI()<<" - "<<nodo->persona->fullNombres()<<" - Zona: "<<nodo->persona->getZona()<<endl;
        }

        consultarPorFranjaHoraria(nodo->derecho, horaInicio, horaFin);
    }

    void contarPorZona(NodoAVL* nodo, int* contadores) {
        if (nodo == nullptr) return;

        contarPorZona(nodo->izquierdo, contadores);

        string zona = nodo->persona->getZona();
        if (zona == "VIP") contadores[0]++;
        else if (zona == "Personal Medico") contadores[1]++;
        else if (zona == "Personal Seguridad") contadores[2]++;
        else if (zona == "Discapacitados") contadores[3]++;
        else if (zona == "Publico general") contadores[4]++;

        contarPorZona(nodo->derecho, contadores);
    }

    void destruirArbol(NodoAVL* nodo) {
        if (nodo != nullptr) {
            destruirArbol(nodo->izquierdo);
            destruirArbol(nodo->derecho);
            delete nodo;
        }
    }

public:
    ArbolAVL() {
        raiz = nullptr;
    }

    ~ArbolAVL() {
        destruirArbol(raiz);
    }

    void registrarAcceso(Persona* persona) {
        int h, m;
        obtenerHoraYMinutos(h, m);
        persona->set_Hora(h, m); // Asignación automática

        raiz = insertarAVL(raiz, persona);
        cout<<"Acceso registrado para "<<persona->fullNombres()<<" a las "<<h<<":"<<(m < 10 ? "0" : "")<<m<<endl;
    }

    void consultarFranjaHoraria(int horaInicio, int horaFin) {
        cout<<"=== ACCESOS ENTRE "<<horaInicio<<" Y "<<horaFin<<" HORAS ==="<<endl;
        consultarPorFranjaHoraria(raiz, horaInicio, horaFin);
        cout<<"==============================================="<<endl;
    }

    void mostrarZonaConMasEntradas() {
        int contadores[5] = {0}; // VIP, Medico, Seguridad, Discapacitados, Publico
        contarPorZona(raiz, contadores);

        string zonas[] = {"VIP", "Personal Medico", "Personal Seguridad", "Discapacitados", "Publico general"};
        int maxIndex = 0;

        for (int i = 1; i < 5; i++) {
            if (contadores[i] > contadores[maxIndex]) {
                maxIndex = i;
            }
        }

        cout<<"=== ESTADÍSTICAS POR ZONA ==="<<endl;
        for (int i = 0; i < 5; i++) {
            cout<<zonas[i]<<": "<<contadores[i]<<" personas"<<endl;
        }
        cout<<"Zona con más entradas: "<<zonas[maxIndex]<<" con "<<contadores[maxIndex]<<" personas"<<endl;
        cout<<"============================="<<endl;
    }
};

//----- FIN DE ÁRBOL AVL -----

//----- FUNCIONES AUXILIARES -----
void mostrarMenu() {
    cout<<"\n=== SISTEMA DE CONTROL DE ACCESOS ==="<<endl;
    cout<<"1. Registrar nuevo acceso"<<endl;
    cout<<"2. Mostrar próximos 5 accesos"<<endl;
    cout<<"3. Consultar accesos por franja horaria"<<endl;
    cout<<"4. Mostrar estadísticas por zona"<<endl;
    cout<<"5. Buscar persona por DNI"<<endl;
    cout<<"6. Actualizar prioridad de persona"<<endl;
    cout<<"7. Cargar datos desde archivo"<<endl;
    cout<<"8. Salir"<<endl;
    cout<<"Seleccione una opcion: ";
}

//----- FIN DE FUNCIONES AUXILIARES -----

int main()
{
    TablaHash* tabla = new TablaHash(40000);
    maxHeap* heap = new maxHeap(40000);
    ArbolAVL* arbol = new ArbolAVL();

    int opcion;
    int horaActual = 8; // Hora de inicio del evento

    do {
        mostrarMenu();

        // Validación de entrada para evitar bucle infinito
        if (!(cin >> opcion)) {
            cout << "Error: Por favor ingrese un número válido." << endl;
            cin.clear(); // Limpiar el estado de error
            cin.ignore(10000, '\n'); // Ignorar caracteres hasta el salto de línea
            continue;
        }

        switch(opcion) {
            case 1: {
                // Registrar nuevo acceso
                int dni;
                cout<<"Ingrese DNI de la persona: ";
                if (!(cin >> dni)) {
                    cout << "Error: Por favor ingrese un DNI válido." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }

                Persona* persona = tabla->buscar(dni);
                if (persona != nullptr) {
                    persona->setHoraIngreso(horaActual);
                    arbol->registrarAcceso(persona);
                    horaActual++; // Incrementar hora para el siguiente acceso
                } else {
                    cout<<"Persona no encontrada en el sistema"<<endl;
                }
                break;
            }
            case 2: {
                // Mostrar próximos 5 accesos
                heap->mostrarProximos5();
                break;
            }
            case 3: {
                // Consultar accesos por franja horaria
                int horaInicio, horaFin;
                cout<<"Ingrese hora de inicio: ";
                if (!(cin >> horaInicio)) {
                    cout << "Error: Por favor ingrese una hora válida." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                cout<<"Ingrese hora de fin: ";
                if (!(cin >> horaFin)) {
                    cout << "Error: Por favor ingrese una hora válida." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                arbol->consultarFranjaHoraria(horaInicio, horaFin);
                break;
            }
            case 4: {
                // Mostrar estadísticas por zona
                arbol->mostrarZonaConMasEntradas();
                break;
            }
            case 5: {
                // Buscar persona por DNI
                int dni;
                cout<<"Ingrese DNI a buscar: ";
                if (!(cin >> dni)) {
                    cout << "Error: Por favor ingrese un DNI válido." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                Persona* persona = tabla->buscar(dni);
                if (persona != nullptr) {
                    persona->datosPersona();
                } else {
                    cout<<"Persona no encontrada"<<endl;
                }
                break;
            }
            case 6: {
                // Actualizar prioridad
                int dni, nuevaPrioridad;
                cout<<"Ingrese DNI: ";
                if (!(cin >> dni)) {
                    cout << "Error: Por favor ingrese un DNI válido." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                cout<<"Ingrese nueva prioridad (1-5): ";
                if (!(cin >> nuevaPrioridad)) {
                    cout << "Error: Por favor ingrese una prioridad válida." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                heap->actualizarPriori(dni, nuevaPrioridad);
                break;
            }
            case 7: {
                // Cargar datos desde archivo
                string nombreArchivo;
                cout<<"Ingrese nombre del archivo: ";
                cin>>nombreArchivo;
                cargaDeMuchasPersonas(tabla, nombreArchivo);
                break;
            }
            case 8: {
                cout<<"Saliendo del sistema..."<<endl;
                break;
            }
            default: {
                cout<<"Opción no válida"<<endl;
                break;
            }
        }
    } while (opcion != 8);

    delete tabla;
    delete heap;
    delete arbol;

    return 0;
}
