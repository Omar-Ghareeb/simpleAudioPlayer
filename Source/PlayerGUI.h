#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::ComboBox::Listener,
	public juce::Timer,
	public juce::ListBoxModel,
	public juce::ChangeListener
{
public:
	PlayerGUI();
	~PlayerGUI() override;

	void resized() override;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	void comboBoxChanged(juce::ComboBox* box) override; // handels changes in comboBox
	void timerCallback() override ;
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
	void paint(juce::Graphics& g) override;

private:
	PlayerAudio playerAudio;

	// GUI elements
	juce::TextButton loadButton{ "Load Files" };
	juce::TextButton playPauseButton{ "Play" };
	juce::TextButton goToStartButton{ "Go to start" };
	juce::TextButton goToEndButton{ "Go to end" };
	juce::TextButton loopButton{ "Loop: Off" };
	juce::TextButton muteButton{ "Mute" };
	juce::TextButton abLoopButton{ "A-B Loop: Off" };
	juce::Label title{ "Title: " };
	juce::Label artist{ "Artist: " };
	juce::Label duration{ "Duration: " };
	juce::Slider volumeSlider;
	juce::Slider speedSlider;
	juce::Slider positionSlider;
	juce::Slider abLoopSlider;
	juce::TextButton addToPlaylistButton{ "Add to Playlist" };
	juce::TextButton removeFromPlaylistButton{ "Remove from Playlist" };
	juce::TextButton nextButton{ "Next" };
	juce::TextButton previousButton{ "Previous" };
	juce::ListBox playListTable;
	juce::TextButton forward10Button{ "+10s" };
	juce::TextButton rewind10Button{ "-10s" };
	juce::ComboBox Markers;
	juce::TextButton addMarker{ "Add Marker" };
	juce::AudioThumbnailCache thumbnailCache;
	juce::AudioThumbnail thumbnail;
	std::unique_ptr<juce::FileChooser> fileChooser;
	std::unique_ptr<juce::PropertiesFile> propertiesFile;

	// Event handlers
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
	int getNumRows() override;
	void saveSession();
	void loadSession();
	void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent& e) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
