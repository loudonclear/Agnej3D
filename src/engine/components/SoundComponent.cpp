#include "SoundComponent.h"
#include "engine/world/GameObject.h"

SoundComponent::SoundComponent(GameObject *parent) : Component(parent)
{

}

void SoundComponent::init() {

}

void SoundComponent::play() {
    //if (!m_soundEffect.isPlaying()) m_soundEffect.play();
}

void SoundComponent::stop() {
    //if (m_soundEffect.isPlaying()) m_soundEffect.stop();
}

void SoundComponent::loop() {
    //m_soundEffect.setLoopCount(QSoundEffect::Infinite);
}

void SoundComponent::setSource(QUrl source) {
    //m_soundEffect.setSource(source);
}

void SoundComponent::setVolume(qreal amt) {
    //m_soundEffect.setVolume(amt);
}
