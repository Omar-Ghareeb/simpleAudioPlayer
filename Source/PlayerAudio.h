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
	double getRelativePos();
	void setRelativePos(double pos);
	void setabLoop(double start, double end);
	void clearabLoop();
	void checkabLoop();
	void addToPlayList(const juce::File& file);
	void removeFromPlayList(int index);
	void playFromPlayList(int index);
	void nextTrack();
	void previousTrack();
	int getCurrentPlayListIndex() const;
	juce::File getCurrentFile() const;
	int getPlayListSize() const;
	juce::File getPlayListFileAt(int index) const;
	void forward10Seconds();
	void rewind10Seconds();
	void addMarker();
	void moveToMarker(int id);
	int getSize() const;
	void clearMarkers();
	int findIndex() const;
	void setIndex(int index);
	void reverbOn(bool state);
	juce::AudioFormatManager& getForamt();
	juce::String getCurrentFilePath() const;
private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	std::unique_ptr<juce::ResamplingAudioSource> ResamplingAudio;
	juce::AudioTransportSource transportSource;
	std::vector<juce::File> playList;
	std::vector<double> markers;
	juce::File currentFile;
	juce::Reverb reverb;
	juce::Reverb::Parameters param;

	bool reverbworking = false;
	bool isLooping = false;
	bool AmIMuted = false;
	float PastVolume = 0.0;
	bool isabLooping = false;
	double abLoopStart = 0.0;
	double abLoopEnd = 0.0;
	int currentPlayListIndex = -1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
