#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}
PlayerAudio::~PlayerAudio()
{
    releaseResources();
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    ResamplingAudio.reset();
    ResamplingAudio = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
    ResamplingAudio->prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    ResamplingAudio->getNextAudioBlock(bufferToFill);
    //transportSource.getNextAudioBlock(bufferToFill);
    if (isLooping && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
    
}
void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}
bool PlayerAudio::loadFile(const juce::File& file)
{
    if (auto* reader = formatManager.createReaderFor(file))
    {
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        return true;
    }
    return false;
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::pause()
{
    transportSource.stop();
}

void PlayerAudio::goToStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::goToEnd()
{
    transportSource.setPosition(transportSource.getLengthInSeconds());
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getRelativePos()
{
    return transportSource.getLengthInSeconds()>0? transportSource.getCurrentPosition() / transportSource.getLengthInSeconds() : 0.0;
}

void PlayerAudio::setRelativePos(double pos)
{
    if (transportSource.getLengthInSeconds() > 0)
        transportSource.setPosition(pos * transportSource.getLengthInSeconds());
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

bool PlayerAudio::isLoopingEnabled() const
{
    return transportSource.isLooping();
}

void PlayerAudio::setLooping(bool loop)
{
    isLooping = loop;
}
void PlayerAudio::mute() {
    if (!AmIMuted) {
        PastVolume = transportSource.getGain();
        setGain(0.0);
        AmIMuted = true;
    }
    else {
        setGain(PastVolume);
        AmIMuted = false;
    }
}
std::vector<std::string> PlayerAudio::metaData(const juce::File& file)
{
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    std::vector<std::string> metadata;
    if (reader != nullptr) {

        juce::String title = reader->metadataValues.getValue("title", "");
        if (title.isEmpty())
            title = file.getFileNameWithoutExtension();
        juce::String artist = reader->metadataValues.getValue("artist", "Unknown Artist");
        double durationInSeconds = (reader->lengthInSamples / reader->sampleRate);
        int durationInMinutes = (durationInSeconds) / 60;
        juce::String duration = juce::String((float)durationInMinutes) + ':' + juce::String(((int)durationInSeconds % 60)) + " minutes";
        metadata.push_back(title.toStdString());
        metadata.push_back(artist.toStdString());
        metadata.push_back(duration.toStdString());
    }

    return metadata;
}
void PlayerAudio::setSpeed(double speed) {
    ResamplingAudio->setResamplingRatio(speed);

}
