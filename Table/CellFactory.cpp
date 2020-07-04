#include "CellFactory.h"
#include "RowElement.h"
#include "../SqlEngine/SqlEngine.h"


CellFactory::~CellFactory() {
}

CellFactory::CellFactory(int mask, string value) {
    if (!value.compare("")) {
        cell = nullptr;
    } else {
        switch (mask & 0b111) {
            case MASK_INT: {
                cell = new Cell<int>(stoi(value));
                break;
            }
            case MASK_FLOAT: {
                cell = new Cell<float>(stof(value));
                break;
            }
            case MASK_TEXT: {
                cell = new Cell<string>(value);
                break;
            }
            case MASK_CHAR: {
                cell = new Cell<char>((char) value.at(0));
                break;
            }
            case MASK_TIME:
            case MASK_DATE:
            default:
                break;
        }
    }
}


RowElement *CellFactory::getCell() {
    return this->cell;
}








