#ifndef _CHIP8_STUDIO_SESSION_VIEW_H_
#define _CHIP8_STUDIO_SESSION_VIEW_H_

#include <defines.h>

#include "session.h"

class CHIP8_STUDIO_SESSION_VIEW
{
protected:

    CHIP8_STUDIO_SESSION* Session;

public:

    VOID SetSession( CHIP8_STUDIO_SESSION* Session )
    {
        this->Session = Session;
    }
};

#endif // _CHIP8_STUDIO_SESSION_VIEW_H_