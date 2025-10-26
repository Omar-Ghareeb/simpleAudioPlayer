#pragma once
#include <JuceHeader.h>

class PlayerAudio {
public:
	PlayerAudio();
	~PlayerAudio();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();

	bool loadFile(const juce::File& file);
	void play();
	void pause();
	void goToStart();
	void goToEnd();
	void setGain(float gain);
	void setPosition(double pos);
	double getPosition() const;
	double getLength() const;
	void setLooping(bool loop);
	void mute();
	std::vector<std::string> metaData(const juce::File& file);
	bool isLoopingEnabled() const;
	void setSpeed(double speed);

private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	std::unique_ptr<juce::ResamplingAudioSource> ResamplingAudio;
	juce::AudioTransportSource transportSource;

	bool isLooping = false;
	bool AmIMuted = false;
	float PastVolume = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
