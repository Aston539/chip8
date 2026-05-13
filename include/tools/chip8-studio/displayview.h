#ifndef _CHIP8_STUDIO_INTERPRETER_H_
#define _CHIP8_STUDIO_INTERPRETER_H_

#include <vector>

#include <defines.h>

#include <QWidget>

#include "sessionview.h"

class CHIP8_STUDIO_DISPLAY_VIEW : public QWidget, 
                                  public CHIP8_STUDIO_SESSION_VIEW
{
    Q_OBJECT

private:

protected:

    VOID paintEvent( QPaintEvent* PaintEvent ) OVERRIDE;
};

#endif // _CHIP8_STUDIO_INTERPRETER_H_