#include "CellFactory.h"
#include "RowElement.h"
#include "../SqlEngine/SqlEngine.h"

//getCell(TIPO) -> Cell<tipo>
//RowElement *cell;
//Cell cell = new Cell()
//swith(TIPO){
//    case(INT):
//        cell = new Cell<tipo>()

CellFactory::CellFactory() {
    cell = nullptr;
}

CellFactory::~CellFactory() {
    delete [] cell;
    cell = nullptr;
}

vector<RowElement*> CellFactory::getCell(vector<int> masks) {
    vector<RowElement*> ret;

    for (int i = 0; i < masks.size(); i++){
        if(masks[i] & MASK_INT){
            cell = new Cell<int>();
            ret.push_back(cell);
        }else if(masks[i] & MASK_FLOAT){
            cell = new Cell<float>();
            ret.push_back(cell);
        }else if(masks[i] & MASK_TEXT) {
            cell = new Cell<string>();
            ret.push_back(cell);
        } else if(masks[i] & MASK_TIME) {
            cell = new Cell<string>();
            ret.push_back(cell);
        }else if(masks[i] & MASK_DATE){
            cell = new Cell<string>();
            ret.push_back(cell);
        }else if(masks[i] & MASK_CHAR){
            cell = new Cell<char>();
            ret.push_back(cell);
        }
    }


    return ret;
}








