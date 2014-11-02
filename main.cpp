//
//  main.cpp
//  contador-lineas-de-codigos
//
//  Created by Octavio Fuenzalida on 29-01-14.
//  Copyright (c) 2014 Octavio Fuenzalida. All rights reserved.
//
#include  <stdio.h>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <dirent.h>
#include  <sys/stat.h>

using namespace std;

string get_ext(string archivo);
long contar_lineas_codigo(string archivo);
string reemTexto(string temp, string bus, string por, int i);

int main(int argc, const char * argv[])
{
    
    // Directorio a examinar
    string sdirn;
    cout<<"Ingrese el nombre del directorio"<<endl;
    cin >>sdirn;
    
    DIR *dir = NULL;
    struct dirent *drnt = NULL;
    char* dirn;
    dirn = new char [sdirn.size()+1];
    
    strcpy(dirn, sdirn.c_str());
    
    string ext;
    long c_lineas = 0;
    int total = 0;
    
    //Recuperar archivos y carpetas del directorio indicado
    dir=opendir(dirn);
    if(dir){
        
        drnt = readdir(dir);
        while(drnt){
            ext = get_ext(drnt->d_name);
            if(ext.compare("php") == 0 or ext.compare("css") == 0){ // Aqui SOLO filtro archivos web: CSS / PHP
                total++;
                c_lineas = c_lineas + contar_lineas_codigo(sdirn + "/" + drnt->d_name); //Cuenta las lineas de cada archivo
                
            }
            drnt = readdir(dir);
        }
        closedir(dir);
    }else{
        cout<<"Error en el directorio: "<<dirn<<endl;
    }
    cout<<"Total Ficheros en "<<sdirn<<": "<<total<<endl;
    cout<<"Total Lineas: "<<c_lineas<<endl;
    
    return 0;
}

long contar_lineas_codigo(string archivo){
    //Cuenta las lineas del archivo indicado
    string linea;
    long c = 0;
    ifstream ifArchivo (archivo);
    if (ifArchivo.is_open()){
        while (! ifArchivo.eof() ){
            getline (ifArchivo,linea);
            linea = reemTexto(linea,"\t","",1); //Quita las tabulaciones reiteradamente
            linea = reemTexto(linea,"\t","",1);
            linea = reemTexto(linea,"\t","",1);
            linea = reemTexto(linea,"\t","",1);
            remove(linea.begin(), linea.end(), ' ');
            if(linea.length()>5){
                c++;
            }
        }
        ifArchivo.close();
    }
    else{ cout << "No se pudo abrir el archivo: "<<archivo<<endl;};
    return c;
}
string reemTexto(string temp, string bus, string por, int i){
    long posicion=-1;
    do{
        posicion = temp.find(bus,posicion + i);
        if (posicion!=string::npos){
            temp.replace(posicion,bus.size(),por);
        }
    }while(posicion!=string::npos);
    return temp;
}
string get_ext(string archivo){
    long pos = archivo.find_last_of(".");
    if(pos>1){
        return archivo.substr(pos+1);
    }else{
        return "";
    }
}