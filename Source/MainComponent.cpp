#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    setSize(2000, 1500);
    setAudioChannels(0, 2);
    player1.loadSession("player1");
    player2.loadSession("player2");
}

MainComponent::~MainComponent()
{
    player1.saveSession("player1");
    player2.saveSession("player2");
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    tempBuffer1.setSize(2, samplesPerBlockExpected);
    tempBuffer2.setSize(2, samplesPerBlockExpected);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    juce::AudioSourceChannelInfo tempInfo1(&tempBuffer1, 0, bufferToFill.numSamples);
    tempBuffer1.clear();
    player1.getNextAudioBlock(bufferToFill);

    juce::AudioSourceChannelInfo tempInfo2(&tempBuffer2, 0, bufferToFill.numSamples);
    tempBuffer2.clear();
    player2.getNextAudioBlock(tempInfo2);

    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        bufferToFill.buffer->addFrom(channel, bufferToFill.startSample, tempBuffer1, channel, 0, bufferToFill.numSamples);
        bufferToFill.buffer->addFrom(channel, bufferToFill.startSample, tempBuffer2, channel, 0, bufferToFill.numSamples);
    }
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    player1.setBounds(area.removeFromTop(area.getHeight() / 2));
    player2.setBounds(area);
    //player1.setBounds(20, 20, getWidth() - 40, getHeight() - 50);
}

void MainComponent::buttonClicked(juce::Button* button) {}
void MainComponent::sliderValueChanged(juce::Slider* slider) {}