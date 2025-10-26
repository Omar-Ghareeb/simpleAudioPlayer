#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
	// Add buttons
	for (auto* btn : { &loadButton, &playPauseButton, &goToStartButton ,&goToEndButton, &loopButton, &muteButton })
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
	}

	// Text labels
	title.setColour(juce::Label::textColourId, juce::Colours::black);
	artist.setColour(juce::Label::textColourId, juce::Colours::black);
	duration.setColour(juce::Label::textColourId, juce::Colours::black);
	addAndMakeVisible(title);
	addAndMakeVisible(artist);
	addAndMakeVisible(duration);

	// Volume slider
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.5);
	volumeSlider.addListener(this);
	addAndMakeVisible(volumeSlider);
	// PlayBack Speed Slider
	speedSlider.setRange(0.1, 2.0, 0.01);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
	addAndMakeVisible(speedSlider);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
	playerAudio.releaseResources();
}

void PlayerGUI::resized()
{
	int y = 20;
	loadButton.setBounds(20, y, 100, 40);
	playPauseButton.setBounds(140, y, 80, 40);
	goToStartButton.setBounds(240, y, 80, 40);
	goToEndButton.setBounds(340, y, 80, 40);
	loopButton.setBounds(440, y, 100, 40);
	muteButton.setBounds(560, y, 80, 40);
	title.setBounds(20, 80, 200, 30);
	artist.setBounds(240, 80, 200, 30);
	duration.setBounds(410, 80, 200, 30);
	volumeSlider.setBounds(20, 130, getWidth() - 40, 30);
	speedSlider.setBounds(20, 200, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
	if (button == &loadButton)
	{
		fileChooser = std::make_unique<juce::FileChooser>("Select audio files to play...",
			juce::File{},
			"*.wav;*.mp3;*.aiff");

		fileChooser->launchAsync(juce::FileBrowserComponent::openMode
			| juce::FileBrowserComponent::canSelectFiles,
			[this](const juce::FileChooser& fc)
			{
				auto files = fc.getResults();
				if (files.size() > 0 && files[0].existsAsFile())
				{
					playerAudio.loadFile(files[0]);

					auto metadata = playerAudio.metaData(fileChooser->getResult());
					title.setText("Titel: " + playerAudio.metaData(fileChooser->getResult())[0], juce::dontSendNotification);
					artist.setText("Artist: " + playerAudio.metaData(fileChooser->getResult())[1], juce::dontSendNotification);
					duration.setText("Duration: " + playerAudio.metaData(fileChooser->getResult())[2], juce::dontSendNotification);
				}
			});
	}
	else if (button == &playPauseButton)
	{
		bool isPlaying = !playPauseButton.getToggleState();
		if (isPlaying)
		{
			playerAudio.play();
			playPauseButton.setButtonText("Pause");
			playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
		}
		else
		{
			playerAudio.pause();
			playPauseButton.setButtonText("Play");
			playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
		}
	}
	else if (button == &goToStartButton)
	{
		playerAudio.goToStart();
	}
	else if (button == &goToEndButton)
	{
		playerAudio.goToEnd();
	}
	else if (button == &loopButton)
	{
		bool loop = !loopButton.getToggleState();
		loopButton.setToggleState(loop, juce::dontSendNotification);
		loopButton.setButtonText(loop ? "Loop: On" : "Loop: Off");
		playerAudio.setLooping(loop);
	}
	else if (button == &muteButton) {
		bool muted = !muteButton.getToggleState();
		muteButton.setToggleState(muted, juce::dontSendNotification);
		playerAudio.mute();
		muteButton.setButtonText(muted ? "Muted" : "Mute");
	}
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider)
	{
		playerAudio.setGain(slider->getValue());
	}
	else if (slider == &speedSlider) {
			playerAudio.setSpeed(slider->getValue());
	}
}

