#include "CellFactory.h"
#include "RowElement.h"
#include "../SqlEngine/SqlEngine.h"

//getCell(TIPO) -> Cell<tipo>
//RowElement *cell;
//Cell cell = new Cell()
//swith(TIPO){
//    case(INT):
//        cell = new Cell<tipo>()

CellFactory::CellFactory(int mask, string value) {
    if(mask & MASK_INT){
        cell = new Cell<int>(stoi(value));
    }else if(mask & MASK_FLOAT){
        cell = new Cell<float>(stof(value));
    }else if(mask & MASK_TEXT) {
        cell = new Cell<string>(value);
    } else if(mask & MASK_TIME) {
        //cell = new Cell<string>();
    }else if(mask & MASK_DATE){
        //cell = new Cell<string>();
    }else if(mask & MASK_CHAR){
        cell = new Cell<char>((char) value.c_str());
    }
}

CellFactory::~CellFactory() {
    //delete [] cell;
}

RowElement* CellFactory::getCell() {
    return this->cell;
}








