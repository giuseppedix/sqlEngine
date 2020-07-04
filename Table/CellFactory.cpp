#include "CellFactory.h"
#include "RowElement.h"
#include "../SqlEngine/SqlEngine.h"
#include "../DateTime/Time.h"
#include "../DateTime/Date.h"


CellFactory::~CellFactory() {
}

CellFactory::CellFactory(int mask, string value) {

    try{
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
                case MASK_TIME:{
                    cell = new Cell<Time>(Time(value));
                    break;
                }
                case MASK_DATE:{
                    cell = new Cell<Date>(Date(value));
                    break;
                }

                default:
                    break;
            }
        }
    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}


RowElement *CellFactory::getCell() {
    return this->cell;
}








