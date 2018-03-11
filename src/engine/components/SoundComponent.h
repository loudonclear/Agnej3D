#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H

#include "Component.h"
#include "qurl.h"

//#include <QSoundEffect>

class SoundComponent : public virtual Component
{
public:
    SoundComponent(GameObject *parent);

    virtual void init();

    void play();
    void stop();
    void loop();
    void setSource(QUrl source);
    void setVolume(qreal amt);

private:
    //QSoundEffect m_soundEffect;
};

#endif // SOUNDCOMPONENT_H
