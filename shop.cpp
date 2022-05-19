#include "shop.h"
#include <iostream>
#include <cctype> //hint: you may use the isupper function in it
using std::cout;
using std::endl;

bool bugcheck = 0;

Shop::Shop(HashTable* fireTable, HashTable* iceTable, HashTable* lightningTable){
    profit = 0;
    this->fireTable = fireTable;
    this->iceTable = iceTable;
    this->lightningTable = lightningTable;
}

Shop::~Shop(){
    delete fireTable;
    fireTable = nullptr;
    delete iceTable;
    iceTable = nullptr;
    delete lightningTable;
    lightningTable = nullptr;
}

HashTable* Shop::getTable(string prefix) const{
    if(prefix == "Fire"){
        //if(bugcheck) cout<<"Fire"<<endl;
        return fireTable;
    }
    else if(prefix == "Ice"){
        //if(bugcheck) cout<<"Ice"<<endl;
        return iceTable;
    }
        //if(bugcheck) cout<<"Lightning"<<endl;
        return lightningTable;
    
}

bool Shop::stockUp(string name, int quantity, int price) const{
    string prefix;
    string suffix;
    int uppercase = 0;
    int i = 0;
    while(name[i] != '\0'){
        if(isupper(name[i])){
            uppercase++;
            //if(bugcheck) cout<<"name[i]"<<name[i]<<endl;
        }
        if(uppercase < 2){
            prefix += name[i];
        }
        else{
            suffix += name[i];
        }
        i++;
    }
    
    if(bugcheck) cout<<"prefix: "<<prefix<<endl;
    if(bugcheck) cout<<"suffix: "<<suffix<<endl;

    Magic* magic = getTable(prefix)->get(suffix);
    
    if(magic){
        if(bugcheck) cout<<"occp"<<endl;
        magic->quantity += quantity;
        if(magic->price != price){
            magic->price = price;
        }
        return true;
    }
    else{
        if(bugcheck) cout<<"empty"<<endl;
        Magic* new_magic = new Magic{prefix, suffix, price, quantity};
        bool value = getTable(prefix)->add(new_magic);
        if(value){
            return true;
        }
        else{
            delete new_magic;
            new_magic = nullptr; 
            return false;
        }  
    }
    return false;
}

bool Shop::sell(string name, int quantity){
    string prefix;
    string suffix;
    int uppercase = 0;
    int i = 0;
    while(name[i] != '\0'){
        if(isupper(name[i])){
            uppercase++;
            if(bugcheck) cout<<"name[i]"<<name[i]<<endl;
        }
        if(uppercase < 2){
            prefix += name[i];
        }
        else{
            suffix += name[i];
        }
        i++;
    }
    
    if(bugcheck) cout<<"prefix: "<<prefix<<endl;
    if(bugcheck) cout<<"suffix: "<<suffix<<endl;

    Magic* magic = getTable(prefix)->get(suffix);
    if(bugcheck) cout<<"magic: "<<magic<<endl;
    if(!magic){
        return false;
    }
    else{
        if(bugcheck) cout<<"magic->quantity: "<<magic->quantity<<endl;
        if(bugcheck) cout<<"quantity: "<<quantity<<endl;
        if(quantity <= magic->quantity){
            if(bugcheck) cout<<"quantity >= magic->quantity "<<endl;
            profit = magic->price * quantity;
            magic->quantity -= quantity;
                if(magic->quantity == 0){
                    getTable(prefix)->remove(suffix);
                }
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}