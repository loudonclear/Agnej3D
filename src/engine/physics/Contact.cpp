#include "Contact.h"

Contact::Contact(const ContactBasicData &data) : ContactBasicData(data), invalidFlag(false)
{
}

bool Contact::isValid() const {
    if (invalidFlag) return false;

    return true;
}
