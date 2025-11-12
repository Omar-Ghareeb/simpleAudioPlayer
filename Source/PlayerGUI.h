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
	void saveSession(const juce::String& playerId);
	void loadSession(const juce::String& playerId);
private:
	PlayerAudio playerAudio;

	// GUI elements
	juce::ImageButton loadButton;
	juce::Image loadImage;
	juce::ImageButton playPauseButton;
	juce::Image playImage;
	juce::Image pauseImage;
	juce::TextButton goToStartButton{ "Go to start" };
	juce::TextButton goToEndButton{ "Go to end" };
	juce::ImageButton loopButton;
	juce::Image loopImage;
	juce::ImageButton muteButton;
	juce::Image muteImage;
	juce::TextButton reverbButton;
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
	juce::ImageButton nextButton;
	juce::ImageButton previousButton;
	juce::Image nextImage;
	juce::Image previousImage;
	juce::ListBox playListTable;
	juce::ImageButton forward10Button;
	juce::ImageButton rewind10Button;
	juce::Image forward10Image;
	juce::Image rewind10Image;
	juce::ComboBox Markers;
	juce::ImageButton addMarker;
	juce::Image addMarkerImage;
	juce::AudioThumbnailCache thumbnailCache;
	juce::AudioThumbnail thumbnail;
	juce::File currentFile;
	std::unique_ptr<juce::FileChooser> fileChooser;
	std::unique_ptr<juce::PropertiesFile> propertiesFile1;
	std::unique_ptr<juce::PropertiesFile> propertiesFile2;

	std::unique_ptr<juce::Drawable> volumeIcon;
	std::unique_ptr<juce::Drawable> speedIcon;

	// Event handlers
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
	int getNumRows() override;
	
	void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent& e) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
