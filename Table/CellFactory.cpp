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
    switch (mask) {
        case MASK_INT:
            cell = new Cell<int>(stoi(value));
            break;
        case MASK_FLOAT:
            cell = new Cell<float>(stof(value));
            break;
        case MASK_TEXT:
            cell = new Cell<string>(value);
            break;
        case MASK_CHAR:
            cell = new Cell<char>((char) value.at(0));
            break;
        case MASK_TIME:
        case MASK_DATE:
        default:
            break;
    }
}

CellFactory::~CellFactory() {
    //delete [] cell;
}

RowElement* CellFactory::getCell() {
    return this->cell;
}








