#include <iostream>
#include <limits> //https://www.geeksforgeeks.org/how-to-validate-user-input-in-cpp/

using namespace std;

int eleccionTipoIngresante(){
    int opcionA;
    cout<<endl<<"Elegir una opcion: "<<endl;
    cout<<"1. Organizador"<<endl;
    cout<<"2. Participante"<<endl;
    cout<<"3. Cancelar"<<endl;
    cout<<endl<<"Opcion: ";
    if (!(cin>>opcionA)){
        cout<<"Entrada no valida, elija nuevamente"<<endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); //esto descarta la opcion no valida
        return eleccionTipoIngresante();
    }
    if (opcionA == 1){
        cout<<"Ingresando como organizador..."<<endl;
        return 1;
    } else if (opcionA == 2){
        cout<<"Ingresando como participante..."<<endl;
        return 2;
    } else if (opcionA == 3){
        cout<<"Tenga buen dia :)"<<endl;
        return 0;
    } else {
        cout<<"Opcion invalido, elija nuevamente"<<endl;
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
        cout<<"6. Salir"<<endl;
        cout<<"Opcion: ";
        cin>>opcionB;

        switch (opcionB){
        case 1:
            cout<<"Creador de eventos:"<<endl;
            break; //breaks por ahora
        case 2:
            cout<<"Lista de eventos:"<<endl;
            break;
        case 3:
            cout<<"Sus eventos:"<<endl;
            break;
        case 4:
            cout<<"Seleccionar evento a modificar:"<<endl;
            break;
        case 5:
            cout<<"Seleccionar evento a eliminar:"<<endl;
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
            break;
        case 2:
            cout<<"Sus eventos:"<<endl;
            break;
            break;
        case 3:
            cout<<"Seleccionar evento a retirarse:"<<endl;
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
    cout<<endl<<endl<<"Bienvenido al grandioso organizador de eventos!!!"<<endl;
    cout<<"      -Patrocinado por el grupo 5 y SuniGod B)"<<endl;
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
