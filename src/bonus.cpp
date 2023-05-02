#include "bonus.h"

Bonus TypeTable::createBonus(int type_id, Vec3f position) {
    Type& specs = types[type_id];
    return Bonus {position, specs};
}
