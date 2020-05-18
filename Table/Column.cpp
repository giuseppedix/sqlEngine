//
// Created by gdisi on 06/05/2020.
//

#include "Column.h"

int Column::getMask() const {
    return mask;
}

void Column::setMask(int mask) {
    Column::mask = mask;
}

const string &Column::getName() const {
    return name;
}

void Column::setName(const string &name) {
    Column::name = name;
}
